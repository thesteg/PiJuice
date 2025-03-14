/*
 * fuel_gauge_lc709203f.c
 *
 *  Created on: 06.12.2016.
 *      Author: milan
 */

#include "main.h"

#include "osloop.h"
#include "system_conf.h"

#include "time_count.h"

#include "adc.h"
#include "analog.h"
#include "charger_bq2416x.h"
#include "power_source.h"
#include "execution.h"
#include "nv.h"

#include "i2cdrv.h"
#include "crc.h"
#include "util.h"

#include "fuel_gauge_lc709203f.h"
#include "fuelgauge_conf.h"
#include "fuelgauge_tables.h"


#define RSOC_TEMP_TABLE_MIN 		-127
#define RSOC_TEMP_MAX_COMPENSATE	21
#define RSOC_TEMP_STEP_COUNT		(RSOC_TEMP_MAX_COMPENSATE - RSOC_TEMP_TABLE_MIN)

// ----------------------------------------------------------------------------
// Function prototypes for functions that only have scope in this module:

static void FUELGAUGE_I2C_Callback(const I2CDRV_Device_t * const p_i2cdrvDevice);
static bool FUELGAUGE_WriteWord(const uint8_t cmd, const uint16_t word);
static bool FUELGAUGE_ReadWord(const uint8_t cmd, uint16_t *const word);
static bool FUELGAUGE_IcInit(void);
static bool FUELGAUGE_CalculateDischargeRate(const uint16_t previousRSoc,
												const uint16_t newRsoc,
												const uint32_t timeDeltaMs);

static void FUELGAUGE_BuildSocTables(void);
static uint32_t FUELGAUGE_GetSOCFromOCV(const uint16_t batteryMv);
static void FUELGAUGE_UpdateCalculateSOC(const uint16_t batteryMv,
											const int16_t batteryTemp,
											const int32_t timeDeltaMs);

// ----------------------------------------------------------------------------
// Variables that only have scope in this module:

static uint16_t m_fuelgaugeI2CReadResult;
static bool m_fuelgaugeI2CSuccess;
static FUELGAUGE_Status_t m_fuelgaugeIcStatus;
static uint16_t m_batteryMv;
static uint8_t m_icInitState;
static uint16_t m_lastSocPt1;
static uint32_t m_lastSocTimeMs;
static int16_t m_batteryTemperaturePt1;
static RsocMeasurementConfig_T m_rsocMeasurementConfig = RSOC_MEASUREMENT_FROM_IC;
static BatteryTempSenseConfig_T m_tempSensorConfig = BAT_TEMP_SENSE_CONFIG_AUTO_DETECT;
static uint8_t m_temperatureMode = FUEL_GAUGE_TEMP_MODE_THERMISTOR;
static int16_t m_dischargeRate;
static uint16_t m_fuelgaugeIcId;
static bool m_thermistorGood;
static uint32_t m_lastFuelGaugeTaskTimeMs;
static bool m_updateBatteryProfile;
static bool m_initBatterySOC;


// ----------------------------------------------------------------------------
// Variables that only have scope in this module that are persistent through reset:

static uint16_t m_ocvSocTable[256u] __attribute__((section("no_init")));
static uint16_t m_rSocTable[256u] __attribute__((section("no_init")));
static uint8_t m_rSocTempCompensateTable[256u] __attribute__((section("no_init")));
static uint16_t m_c0 __attribute__((section("no_init")));
static int32_t m_soc __attribute__((section("no_init")));


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// CALLBACK HANDLERS
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ****************************************************************************
/*!
 * FUELGAUGE_I2C_Callback interfaces with the i2cdrv module, lets this module
 * 			know when the transaction is complete and provides a pointer to the
 * 			i2cdriver internal data which will contain information regarding the
 * 			transaction.
 * @param	p_i2cdrvDevice		pointer to the i2cdrv internals
 * @retval	none
 */
// ****************************************************************************
static void FUELGAUGE_I2C_Callback(const I2CDRV_Device_t * const p_i2cdrvDevice)
{
	if (p_i2cdrvDevice->event == I2CDRV_EVENT_RX_COMPLETE)
	{
		crc_t crc = crc_8_init(FUELGAUGE_I2C_ADDR);
		// crc includes address, mem address, address | 0x01, data
		crc = crc_8_update(crc, p_i2cdrvDevice->data, 4u);

		if (crc == p_i2cdrvDevice->data[4u])
		{
			m_fuelgaugeI2CReadResult = (uint16_t)p_i2cdrvDevice->data[2u] | (p_i2cdrvDevice->data[3u] << 8u);

			m_fuelgaugeI2CSuccess = true;
		}
		else
		{
			m_fuelgaugeI2CSuccess = false;
		}
	}
	else if (p_i2cdrvDevice->event == I2CDRV_EVENT_TX_COMPLETE)
	{
		m_fuelgaugeI2CSuccess = true;
	}
	else
	{
		m_fuelgaugeI2CSuccess = false;
	}
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// FUNCTIONS WITH GLOBAL SCOPE
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ****************************************************************************
/*!
 * FUELGAUGE_Init configures the module to a known initial state
 * @param	none
 * @retval	none
 */
// ****************************************************************************
void FUELGAUGE_Init(void)
{
	const uint32_t sysTime = HAL_GetTick();

	uint16_t tempU16;
	uint8_t config;

	if (NV_READ_VARIABLE_SUCCESS == NvReadVariableU8(FUEL_GAUGE_CONFIG_NV_ADDR, &config))
	{
		m_tempSensorConfig = (uint8_t)(config & 0x7u);
		m_rsocMeasurementConfig = (uint8_t)(config >> 4u) & 0x3u;
	}

	if (EXECUTION_STATE_NORMAL != executionState)
	{
		// FuelGaugeDvInit();
	}

	// Try and talk to the fuel gauge ic
	// Note: SOC might not be correctly evaluated if the battery is being charged or discharged
	if (true == FUELGAUGE_IcInit())
	{
		m_fuelgaugeIcStatus = FUELGAUGE_STATUS_ONLINE;

		if (true == FUELGAUGE_ReadWord(FG_MEM_ADDR_ITE, &tempU16))
		{
			m_lastSocPt1 = tempU16;

			MS_TIMEREF_INIT(m_lastSocTimeMs, sysTime);
		}

		if (true == FUELGAUGE_ReadWord(FG_MEM_ADDR_CELL_MV, &tempU16))
		{
			m_batteryMv = tempU16;
		}
	}
	else
	{
		m_fuelgaugeIcStatus = FUELGAUGE_STATUS_OFFLINE;
		m_lastSocPt1 = 0u;
		m_batteryMv = 0u;
	}

	//if (RSOC_MEASUREMENT_DIRECT_DV == m_rsocMeasurementConfig)
	{
		FUELGAUGE_BuildSocTables();
		m_initBatterySOC =  true;
	}

	MS_TIMEREF_INIT(m_lastFuelGaugeTaskTimeMs, sysTime);

}


// ****************************************************************************
/*!
 * FUELGAUGE_Task performs periodic updates for this module
 *
 * @param	none
 * @retval	none
 */
// ****************************************************************************
void FUELGAUGE_Task(void)
{
	const uint32_t sysTime = HAL_GetTick();
	const int16_t mcuTemperature = ANALOG_GetMCUTemp();
	const uint16_t battMv = ANALOG_GetBatteryMv();

	uint16_t tempU16;
	int16_t tempS16;
	uint32_t socTimeDiff;

	if (MS_TIMEREF_TIMEOUT(m_lastFuelGaugeTaskTimeMs, sysTime, FUELGAUGE_TASK_PERIOD_MS))
	{
		MS_TIMEREF_INIT(m_lastFuelGaugeTaskTimeMs, sysTime);

		if (battMv < FUELGAUGE_MIN_BATT_MV)
		{
			// If the battery voltage is less than the device operating limit then don't even bother!
			m_fuelgaugeIcStatus = FUELGAUGE_STATUS_OFFLINE;
			m_lastSocPt1 = 0u;
			m_batteryMv = 0u;

			return;
		}

		if ( (RSOC_MEASUREMENT_DIRECT_DV == m_rsocMeasurementConfig) && (true == m_updateBatteryProfile) )
		{
			FUELGAUGE_BuildSocTables();
			m_initBatterySOC =  true;
		}

		if ( (FUELGAUGE_STATUS_OFFLINE == m_fuelgaugeIcStatus) || (true == m_updateBatteryProfile) )
		{
			m_updateBatteryProfile = false;

			if (true == FUELGAUGE_IcInit())
			{
				m_fuelgaugeIcStatus = FUELGAUGE_STATUS_ONLINE;
			}
		}

		// If battery just inserted or there has been a profile change, work out the SOC
		// Should not run if soc is used from fuel gauge ic.
		if (m_initBatterySOC)
		{
			m_initBatterySOC = false;

			//m_currentSocTableIdx = FUELGAUGE_GetSocTableIdxFromOCV(battMv);
			m_soc = FUELGAUGE_GetSOCFromOCV(battMv);
			MS_TIMEREF_INIT(m_lastSocTimeMs, sysTime);

		}


		if (FUELGAUGE_STATUS_ONLINE == m_fuelgaugeIcStatus)
		{
			if (true == FUELGAUGE_ReadWord(FG_MEM_ADDR_CELL_MV, &tempU16))
			{
				m_batteryMv = tempU16;
			}

			if (m_temperatureMode == FUEL_GAUGE_TEMP_MODE_THERMISTOR)
			{
				if (true == FUELGAUGE_ReadWord(FG_MEM_ADDR_CELL_TEMP, &tempU16))
				{
					tempS16 = ((int16_t)tempU16 - CELL_TEMP_OFS);

					// Check for a sane number
					if (tempS16 < (int16_t)(mcuTemperature - 10))
					{
						m_batteryTemperaturePt1 = mcuTemperature * 10;
					}
					else
					{
						m_batteryTemperaturePt1 = tempS16;
					}
				}
			}
			else
			{
				m_batteryTemperaturePt1 = mcuTemperature * 10;
				FUELGAUGE_WriteWord(0x08, m_batteryTemperaturePt1 + CELL_TEMP_OFS);
			}

			socTimeDiff = MS_TIMEREF_DIFF(m_lastSocTimeMs, HAL_GetTick());

			if (RSOC_MEASUREMENT_DIRECT_DV == m_rsocMeasurementConfig)
			{
				FUELGAUGE_UpdateCalculateSOC(battMv, m_batteryTemperaturePt1 / 10, socTimeDiff);

				MS_TIMEREF_INIT(m_lastSocTimeMs, sysTime);
			}
			else
			{
				if (true == FUELGAUGE_ReadWord(FG_MEM_ADDR_ITE, &tempU16))
				{
					if (FUELGAUGE_CalculateDischargeRate(m_lastSocPt1, tempU16,	socTimeDiff))
					{
						// Log soc and time for next update
						m_lastSocPt1 = tempU16;

						MS_TIMEREF_INIT(m_lastSocTimeMs, sysTime);
					}
				}
			}
		}
		else
		{
			m_batteryMv = battMv;

			socTimeDiff = MS_TIMEREF_DIFF(m_lastSocTimeMs, HAL_GetTick());

			if (RSOC_MEASUREMENT_DIRECT_DV == m_rsocMeasurementConfig)
			{
				FUELGAUGE_UpdateCalculateSOC(battMv, m_batteryTemperaturePt1 / 10, socTimeDiff);

				MS_TIMEREF_INIT(m_lastSocTimeMs, sysTime);
			}
		}
	}
}


// ****************************************************************************
/*!
 * FUELGAUGE_InitBatterySOC should be called when the battery has been inserted
 * so that the module can calculate the initial SOC based on the terminal voltage.
 *
 * @param	none
 * @retval	none
 */
// ****************************************************************************
void FUELGAUGE_InitBatterySOC(void)
{
	if (RSOC_MEASUREMENT_DIRECT_DV == m_rsocMeasurementConfig)
	{
		m_initBatterySOC = true;
	}
}


// ****************************************************************************
/*!
 * FUELGAUGE_UpdateBatteryProfile tells the module to update its configuration
 * based on the current battery profile. Generally called when the battery profile
 * is changed.
 *
 * @param	none
 * @retval	none
 */
// ****************************************************************************
void FUELGAUGE_UpdateBatteryProfile(void)
{
	m_updateBatteryProfile = true;
}


// ****************************************************************************
/*!
 * FUELGAUGE_SetConfigData sets the current configuration data
 *
 * @param	p_data		pointer to buffer where the config data is held
 * @param	len			length of config data
 * @retval	none
 */
// ****************************************************************************
void FUELGAUGE_SetConfigData(const uint8_t * const p_data, const uint16_t len)
{
	const uint8_t newTempConfig = p_data[0u] & 0x07u;
	const uint8_t newRsocConfig = (uint8_t)((p_data[0u] & 0x30u) >> 4u);

	uint8_t config;

	if ( (newTempConfig >= BAT_TEMP_SENSE_CONFIG_TYPES)
			|| (newRsocConfig >= RSOC_MEASUREMENT_CONFIG_TYPES)
			)
	{
		return;
	}

	NV_WriteVariable_U8(FUEL_GAUGE_CONFIG_NV_ADDR, p_data[0u]);

	if (NvReadVariableU8(FUEL_GAUGE_CONFIG_NV_ADDR, &config) == NV_READ_VARIABLE_SUCCESS)
	{
		m_rsocMeasurementConfig = (RsocMeasurementConfig_T)newRsocConfig;
		m_tempSensorConfig = (BatteryTempSenseConfig_T)newTempConfig;
	}
	else
	{
		m_tempSensorConfig = BAT_TEMP_SENSE_CONFIG_AUTO_DETECT;
		m_rsocMeasurementConfig = RSOC_MEASUREMENT_FROM_IC;
	}
}


// ****************************************************************************
/*!
 * FUELGAUGE_GetConfigData populates a buffer with the current fuel gauge configuration
 *
 * @param	p_data		pointer to buffer where the config data is to be placed
 * @param	p_len		length of status data
 * @retval	none
 */
// ****************************************************************************
void FUELGAUGE_GetConfigData(uint8_t * const p_data, uint16_t * const p_len)
{
	p_data[0u] = (m_tempSensorConfig & 0x07u) | ( (m_rsocMeasurementConfig & 0x03u) << 4u);
	*p_len = 1u;
}


// ****************************************************************************
/*!
 * FUELGAUGE_GetIcId returns the id of the fuel gauge IC. Think this is just its
 * address!
 *
 * @param	none
 * @retval	uint16_t		id of the fuel gauge IC
 */
// ****************************************************************************
uint16_t FUELGAUGE_GetIcId(void)
{
	return m_fuelgaugeIcId;
}


// ****************************************************************************
/*!
 * FUELGAUGE_IsNtcOK returns the state of the NTC thermister, if faulty the routine
 * will return false, true if it appears to be ok.
 *
 * @param	none
 * @retval	bool		false = thermister faulty
 * 						true = thermister appears ok
 */
// ****************************************************************************
bool FUELGAUGE_IsNtcOK(void)
{
	return m_thermistorGood;
}


// ****************************************************************************
/*!
 * FUELGAUGE_IsOnline returns the status of the fuel gauge IC communications. If
 * the battery is not inserted then the IC itself does not get any power and will
 * not be communicating.
 *
 * @param	none
 * @retval	bool		false = fuel gauge IC is not powered
 * 						true = fuel gauge IC is powered
 */
// ****************************************************************************
bool FUELGAUGE_IsOnline(void)
{
	return FUELGAUGE_STATUS_ONLINE == m_fuelgaugeIcStatus;
}


// ****************************************************************************
/*!
 * FUELGAUGE_GetBatteryTemperature returns the battery temperature. If the battery
 * does not have a thermister or the thermister value is out of range then this
 * returns the temperature of the processor.
 *
 * @param	none
 * @retval	int8_t		temperature in degrees of the battery (or processor!)
 */
// ****************************************************************************
int8_t FUELGAUGE_GetBatteryTemperature(void)
{
	return m_batteryTemperaturePt1 / 10;
}


// ****************************************************************************
/*!
 * FUELGAUGE_GetSocPt1 returns the state of charge as determined by the module
 * in 0.1% resolution. The calculation is either done in software or by the fuel
 * gauge IC depending on the configuration.
 *
 * @param	none
 * @retval	uint16_t	battery soc in 0.1% steps
 */
// ****************************************************************************
uint16_t FUELGAUGE_GetSocPt1(void)
{
	return m_lastSocPt1;
}


// ****************************************************************************
/*!
 * FUELGAUGE_GetBatteryMaHr returns the discharge or charge rate of the battery
 * as determined by the capacity and difference in SOC during a period of time.
 *
 * @param	none
 * @retval	int16_t		battery discharge/charge rate
 */
// ****************************************************************************
int16_t FUELGAUGE_GetBatteryMaHr(void)
{
	return m_dischargeRate;
}


// ****************************************************************************
/*!
 * FUELGAUGE_GetBatteryMv returns the battery terminal voltage, this can be either
 * from the ADC directly or the fuel gauge ic depending on whether the IC is online.
 *
 * @param	none
 * @retval	uint16_t		Battery voltage in mV
 */
// ****************************************************************************
uint16_t FUELGAUGE_GetBatteryMv(void)
{
	return m_batteryMv;
}


// ****************************************************************************
/*!
 * FUELGAUGE_GetBatteryTempSensorCfg returns the current configuration for the
 * battery temperature sensor
 *
 * @param	none
 * @retval	BatteryTempSenseConfig_T	battery temperature configuration
 */
// ****************************************************************************
BatteryTempSenseConfig_T FUELGAUGE_GetBatteryTempSensorCfg(void)
{
	return m_tempSensorConfig;
}


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// FUNCTIONS WITH LOCAL SCOPE
// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
// ****************************************************************************
/*!
 * FUELGAUGE_IcInit initialises the fuel gauge IC with the battery information.
 *
 * @param	none
 * @retval	bool
 */
// ****************************************************************************
static bool FUELGAUGE_IcInit(void)
{
	const BatteryProfile_T * currentBatProfile = BATTERY_GetActiveProfileHandle();
	uint16_t tempU16;

	m_icInitState = 0u;

	// Check to see if the device is online
	if (FUELGAUGE_ReadWord(0x11, &tempU16))
	{
		m_fuelgaugeIcId = tempU16;

		m_icInitState++;

		// Set operational mode
		if (false == FUELGAUGE_WriteWord(FG_MEM_ADDR_POWER_MODE, POWER_MODE_OPERATIONAL))
		{
			return false;
		}

		m_icInitState++;

		// set APA
		if (false == FUELGAUGE_WriteWord(FG_MEM_ADDR_APA, 0x36u))
		{
			return false;
		}

		m_icInitState++;

		// set change of the parameter
		if (false == FUELGAUGE_WriteWord(FG_MEM_ADDR_PARAM_NO_SET,
				(currentBatProfile->chemistry == BAT_CHEMISTRY_LIPO_GRAPHENE) ?
						BATT_PROFILE_0 :
						BATT_PROFILE_1)
				)
		{
			return false;
		}

		m_icInitState++;

		// set APT
		if (false == FUELGAUGE_WriteWord(FG_MEM_ADDR_APT, 0x3000u))
		{
			return false;
		}

		m_icInitState++;

		if ( (NULL != currentBatProfile) && (0xFFFFu != currentBatProfile->ntcB) )
		{
			// Set NTC B constant
			if (false == FUELGAUGE_WriteWord(FG_MEM_ADDR_THERMB, currentBatProfile->ntcB))
			{
				return false;
			}
		}

		m_icInitState++;

		// Set NTC mode
		if (false == FUELGAUGE_WriteWord(FG_MEM_ADDR_THERM_TYPE, THERM_TYPE_NTC))
		{
			return false;
		}

		m_temperatureMode = FUEL_GAUGE_TEMP_MODE_THERMISTOR;

		// IC only calculates for LIPO chemistry, override the setting
		if ( (currentBatProfile->chemistry != BAT_CHEMISTRY_LIPO) && (currentBatProfile->chemistry != BAT_CHEMISTRY_LIPO_GRAPHENE) )
		{
			m_rsocMeasurementConfig = RSOC_MEASUREMENT_DIRECT_DV;
		}

		m_icInitState++;

		return true;
	}

	m_fuelgaugeIcId = 0u;

	return false;

}


// ****************************************************************************
/*!
 * FUELGAUGE_ReadWord reads a single register word from the fuel gauge IC, the
 * routine returns true if the read was successful and the value is valid. The
 * crc of the returned value is checked in the callback to ensure correctness.
 *
 * @param	cmd			address of device register to read
 * @param	p_word		pointer to value destination
 * @retval	bool		false = read unsuccessful
 * 						true = read successful
 */
// ****************************************************************************
static bool FUELGAUGE_ReadWord(const uint8_t cmd, uint16_t * const p_word)
{
	const uint32_t sysTime = HAL_GetTick();
	bool transactGood;

	m_fuelgaugeI2CSuccess = false;

	transactGood = I2CDRV_Transact(FUELGAUGE_I2C_PORTNO, FUELGAUGE_I2C_ADDR, &cmd, 3u,
						I2CDRV_TRANSACTION_RX, FUELGAUGE_I2C_Callback,
						1000u, sysTime
						);

	if (false == transactGood)
	{
		return false;
	}

	while(false == I2CDRV_IsReady(FUELGAUGE_I2C_PORTNO))
	{
		// Wait for transfer
	}

	*p_word = m_fuelgaugeI2CReadResult;

	return m_fuelgaugeI2CSuccess;
}


// ****************************************************************************
/*!
 * FUELGAUGE_WriteWord writes to a word register within the fuel gauge device.
 * A transfer buffer is created and the crc is calculated and appended to the
 * end of the buffer. The routine will wait until the transfer is complete and
 * upon a successful write will return true.
 *
 * @param	memAddress		device memory address to write to
 * @param	value			uint16 value to write to the memory address
 * @retval	bool			false = write unsuccessful
 * 							true = write successful
 */
// ****************************************************************************
static bool FUELGAUGE_WriteWord(const uint8_t memAddress, const uint16_t value)
{
	const uint32_t sysTime = HAL_GetTick();
	bool transactGood;
	uint8_t writeData[4u] = { memAddress, (uint8_t)(value & 0xFFu), (uint8_t)(value >> 8u), 0u};

	crc_t crc = crc_8_init(FUELGAUGE_I2C_ADDR);
	crc = crc_8_update(crc, writeData, 3u);

	writeData[3u] = (uint8_t)crc;

	m_fuelgaugeI2CSuccess = false;

	transactGood = I2CDRV_Transact(FUELGAUGE_I2C_PORTNO, FUELGAUGE_I2C_ADDR, writeData, 4u,
						I2CDRV_TRANSACTION_TX, FUELGAUGE_I2C_Callback,
						1000u, sysTime
						);

	if (false == transactGood)
	{
		return false;
	}

	while(false == I2CDRV_IsReady(FUELGAUGE_I2C_PORTNO))
	{
		// Wait for transfer
	}

	return m_fuelgaugeI2CSuccess;
}


// ****************************************************************************
/*!
 * FUELGAUGE_GetSocFromOCV finds the SOC from the index of the calculated SOC
 * voltage table. It is then multiplied up to perform further calculations at an
 * increased resolution.
 *
 * Note: The result is not temperature compensated, should this be implemented?
 *
 * @param	batteryMv	battery voltage in millivolts
 * @retval	uint32_t	index of SOC table * 2^23
 */
// ****************************************************************************
static uint32_t FUELGAUGE_GetSOCFromOCV(const uint16_t batteryMv)
{
	uint8_t i = 0u;

	while ( (m_ocvSocTable[i] < batteryMv) && (i < 255u) )
	{
		i++;
	}

	return i * (1u << 23u);
}


// ****************************************************************************
/*!
 * FUELGAUGE_CalculateSOCInit initialises the battery SOC calculation tables from
 * using the extended parameter settings for the battery internal DC resistance
 * characteristics and the preset voltage curves. The parameters originate from
 * the python script in the pijuice software folder.
 *
 * @param	none
 * @retval	none
 */
// ****************************************************************************
static void FUELGAUGE_BuildSocTables(void)
{
	const BatteryProfile_T * const p_currentBatProfile = BATTERY_GetActiveProfileHandle();
	const int16_t * ocvSocTableRef = ocvSocTableNormLipo;

	uint16_t i;
	int16_t tempStep;
	uint16_t ocv50 = 3800u;
	uint16_t ocv10 = 3649u;
	uint16_t ocv90 = 4077u;

	uint16_t r50 = 1.82 * 156u;
	uint16_t r10 = 1.82 * 160u;
	uint16_t r90 = 1.82 * 155u;

	uint32_t ocvRef50 = 3791u;
	uint32_t ocvRef10 = 3652u;
	uint32_t ocvRef90 = 4070u;

	int32_t k1;
	uint32_t OCVdSoc50;
	int32_t k2;
	int32_t d1;
	int32_t d2;

	m_c0 = 1820u;

	if (NULL == p_currentBatProfile)
	{
		return;
	}

	if (BAT_CHEMISTRY_LIFEPO4 == p_currentBatProfile->chemistry)
	{
		ocvSocTableRef = ocvSocTableNormLifepo4;
		ocvRef50 = 3243;
		ocvRef10 = 3111;
		ocvRef90 = 3283;
	}
	else
	{
		ocvSocTableRef = ocvSocTableNormLipo;
		ocvRef50 = 3791;
		ocvRef10 = 3652;
		ocvRef90 = 4070;
	}


	m_c0 = p_currentBatProfile->capacity;

	ocv50 = (0xFFFF != p_currentBatProfile->ocv50) ?
				p_currentBatProfile->ocv50 :
				((uint16_t)p_currentBatProfile->regulationVoltage + 175u + p_currentBatProfile->cutoffVoltage) * 10u;

	ocv10 = (0xFFFF != p_currentBatProfile->ocv10) ?
				p_currentBatProfile->ocv10 :
				0.96322f * ocv50;

	ocv90 = (0xFFFF != p_currentBatProfile->ocv90) ?
				p_currentBatProfile->ocv90 :
				1.0735f * ocv50;

	if (0xFFFFu != p_currentBatProfile->r50)
	{
		r50 = ((uint32_t)m_c0 * p_currentBatProfile->r50) / 100000u;
	}

	r10 = (0xFFFFu != p_currentBatProfile->r10) ?
			((uint32_t)m_c0 * p_currentBatProfile->r10) / 100000u :
			r50;

	r90 = (0xFFFFu != p_currentBatProfile->r90) ?
			((uint32_t)m_c0 * p_currentBatProfile->r90) / 100000u :
			r50;

	// Calculate first slope increase rate
	k1 = ((ocvRef90 - ocvRef50) * (230 - 26) * (ocv50 - ocv10)) / 1024u;

	// Build first slope in OCV table (lowest soc)
	for (i = 0u; i < 26u; i++)
	{
		m_ocvSocTable[i] = ocv50 - ((k1 * ocvSocTableRef[i]) / 65536); //ocv50 - (((((int32_t)(4070-3791)*(230-26.0)*dOCV10)>>8)*ocvSocTableNorm[i])>>10);
		m_rSocTable[i] = 65535 / (r50 + (r50 - r10) * (i - 128) / (128 - 26));
	}

	OCVdSoc50 = (((uint32_t)ocv50) * 8196u) / (230u - 26u);

	k1 = ((ocvRef90 - ocvRef50) * (ocv50 - ocv10)) / 16;
	k2 = ((ocvRef50 - ocvRef10) * (ocv90 - ocv50)) / 16;

	// Build mid slope in OCV table (mid soc)
	for (i = 26u; i < 128u; i++)
	{
		d1= (OCVdSoc50 - ((k1 * ocvSocTableRef[i]) / 512)) * (230 - i); //ocv50/(230-26.0) - (((((int32_t)(4070-3791)*dOCV10))*ocvSocTableNorm[i])>>18);
		d2= (OCVdSoc50 - ((k2 * ocvSocTableRef[i]) / 512)) * (i - 26u); //ocv50/(230-26.0) + (((((int32_t)(3652-3791)*dOCV90))*ocvSocTableNorm[i])>>18);
		m_ocvSocTable[i] = (d2 + d1) / 8192;
		m_rSocTable[i] = 65535 / (r50 + (r50 - r10) * (i - 128) / (128 - 26));
	}

	k2 = ((ocvRef50 - ocvRef10) * (230 - 26) * (ocv90 - ocv50)) / 1024;

	// Build high slope in OCV table (highest soc)
	for (i = 128u; i < 256u; i++)
	{
		m_ocvSocTable[i] = ocv50 - ((k2 * ocvSocTableRef[i]) / 65536); //ocv50 + (((((int32_t)(3652-3791)*(230-26.0)*dOCV90)>>8)*ocvSocTableNorm[i])>>10);
		m_rSocTable[i] = 65535 / (r50 + (r50 - r90) * (i - 128) / (128 - 230));
	}

	// Build a table of 16bit (8.8) fixed point multipliers to adjust the value of rsoc during the estimation update
	tempStep = RSOC_TEMP_TABLE_MIN;

	for (i = 0u; i < sizeof(m_rSocTempCompensateTable); i++, tempStep++)
	{
		m_rSocTempCompensateTable[i] = (tempStep < RSOC_TEMP_MAX_COMPENSATE) ?
											(255ul * 32) / (32 + (2 * ((RSOC_TEMP_MAX_COMPENSATE - 1) - tempStep))) :
											255;
	}

	m_dischargeRate = 0;
}


// ****************************************************************************
/*!
 * FUELGAUGE_UpdateCalculateSOC attempts to determine the change in SOC from the
 * battery voltage using the extended parameter settings for the battery internal
 * DC resistance characteristics.
 *
 * @param	batteryMv		current voltage of battery in mV
 * @param	dt				period in mS since the last update
 * @retval	none
 */
// ****************************************************************************
static void FUELGAUGE_UpdateCalculateSOC(const uint16_t batteryMv,
											const int16_t batteryTemp,
											const int32_t timeDeltaMs)
{
	const uint32_t currentSocTableIdx = m_soc >> 23u;
	const int32_t deltaBatteryMv = (int32_t)m_ocvSocTable[currentSocTableIdx] - batteryMv;

	// Fix point multiply rsoc by temp compensation lookup
	uint16_t rsoc;

	if (batteryTemp < RSOC_TEMP_MAX_COMPENSATE)
	{

		rsoc = UTIL_FixMul_U16_U16(m_rSocTempCompensateTable[batteryTemp - RSOC_TEMP_TABLE_MIN],
									m_rSocTable[currentSocTableIdx]
												);
	}
	else
	{
		rsoc = UTIL_FixMul_U16_U16(255u, m_rSocTable[currentSocTableIdx]);
	}

	const int32_t deltaSoc = ((deltaBatteryMv * timeDeltaMs * ((596l * rsoc) / 256))) / 256;

	m_dischargeRate = (deltaBatteryMv * (((uint32_t)m_c0 * rsoc) / 64)) / 1024;

	m_soc -= deltaSoc;
	m_soc = (m_soc <= 2139095040) ? m_soc : 2139095040;

	m_soc = (m_soc > 0) ? m_soc : 0;

	// Divide SOC by 2147483.648 to get batterySOC?
	m_lastSocPt1 = ((m_soc >> 7) * 125) >> 21;
}


// ****************************************************************************
/*!
 * FUELGAUGE_CalculateDischargeRate looks at the change in SOC and works out how
 * much capacity has been taken over the amount of time the change occurred and
 * converts that value into mA per hour. If no change in SOC has occurred or time
 * delta is 0 then no action is taken and the routine returns false.
 *
 * @param	previousRSoc		soc reading at start of time period
 * @param	newRsoc				soc reading at end of time period
 * @param	timeDeltaMs			time elapsed between readings
 * @retval	bool				false = no discharge rate was calculated
 * 								true = discharge rate was calculated
 */
// ****************************************************************************
// TODO - check this calculation against original code
static bool FUELGAUGE_CalculateDischargeRate(const uint16_t previousRSoc,
												const uint16_t newRsoc,
												const uint32_t timeDeltaMs)
{
	const BatteryProfile_T * currentBatProfile = BATTERY_GetActiveProfileHandle();
	const int16_t socDelta = previousRSoc - newRsoc;
	const int32_t deltaFactor = (int32_t)socDelta * timeDeltaMs * currentBatProfile->capacity;

	// Check to make sure there is a change in both time and SOC
	if (deltaFactor != 0)
	{
		m_dischargeRate = UTIL_FixMul_U32_S32(FUELGAUGE_SOC_TO_I_K, deltaFactor);

		return true;
	}

	return false;
}
