# PiJuice Software

## Software installation
### Automated process
At the command line simply type:
```bash
sudo apt-get install pijuice-gui
```
PiJuice depends on other libraries to be present, the package is designed to raise them and let apt-get resolve them.

If you wish to install just the light version of PiJuice with no GUI:
```bash
sudo apt-get install pijuice-base
```
This is particularly indicated for Raspbian Lite or an headless installation.

Note: The v1.6 debian packages are available for Buster and Stretch. The latest packages in the repo for Jessie are v1.4.
Jessie users can install the v1.6 .deb packages manually from [here](https://github.com/PiSupply/PiJuice/tree/master/Software/Install) however v1.6 has not been tested under Jessie.


### Manual process

Install the dependency python urwid package:

`sudo apt-get install python3-urwid`

For the light version:

`sudo dpkg -i ./pijuice-base_1.6_all.deb`

For example for the full version with GUI:

`sudo dpkg -i ./pijuice-gui_1.6_all.deb`

**Note:** You must install the PiJuice base version prior to installing the GUI.

Should the installation complain about missing dependencies you need to sort them first and try with the installation once again.

You will need to reboot at this point so that the system tray app is refreshed.

To remove PiJuice you'll need to run:

`sudo dpkg -r pijuice-gui`

#### Build DEB-packages manually
`./pckg-pijuice.sh`

*Note: You will need python-stdeb, dh-systemd and debhelper in order to be able to build.*

#### OSMC/Kodi Installations

When installing the pijuice-base package the setup procedure assumes that the default user, which is normally 'pi' on Raspbian, has the privilege to sudo to another user 'pijuice'. On Raspbain user 'pi' has this privilege by default. To overcome this you need to edit the sudoers file with the following:

```bash
sudo visudo
```
Add the following line:
```bash
osmc ALL=(pijuice) ALL
```

## GUI Menus

We have also taken a LOT of screenshots of all the different menu options etc to show you the full software. So lets get stuck in:

![Raspberry Pi Menu Entry for PiJuice Settings Software](https://user-images.githubusercontent.com/16068311/35343402-7a362126-0122-11e8-9961-0b7013453f3f.png "Raspberry Pi Menu Entry for PiJuice Settings Software")

We have compiled the source code into a .deb Debian package file so it is super easy to install. Once installation is complete the software appears in the system menu under Menu -> Preferences -> PiJuice Settings as you can see in the above image.

### System Tray

![System Tray](https://user-images.githubusercontent.com/16068311/33845725-43e1d0e6-de9c-11e7-921e-64c9cceb2c96.png "System Tray")

Once you load the software, you will see the PiJuice icon appear in the system tray, as above. This icon shows you the status of the PiJuice - charging from Pi, charging from PiJuice, running on battery as you have in a normal laptop computer. Additionally you can hover over it to tell you the charge level of the battery.

*Note that it is not possible to detect battery not present when powered through on board USB micro, so it might show 0% only.*

![bat-100](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/bat-90.png)
![bat-50](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/bat-50.png)
![bat-0](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/bat-0.png)

![bat-in-100](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/bat-in-90.png)
![bat-in-50](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/bat-in-50.png)
![bat-in-0](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/bat-in-0.png)

![bat-rpi-90](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/bat-rpi-90.png)
![bat-rpi-50](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/bat-rpi-50.png)
![bat-rpi-0](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/bat-rpi-0.png)

![no-bat-in-0](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/no-bat-in-0.png)
![no-bat-rpi-0](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/no-bat-rpi-0.png)

![connection-rror](https://raw.githubusercontent.com/PiSupply/PiJuice/master/Software/Source/data/images/connection-error.png)

You can also right click on this icon to load the configuration menu, instead of having to go to the menu as in the previous image.

## PiJuice Settings

### Main software menu

![Main software menu, with battery attached](https://user-images.githubusercontent.com/1197294/59568742-2d7e2980-907f-11e9-8e36-c2b32b60f5f1.png "Main software menu, with battery attached")

This picture is how the PiJuice Settings software looks when it loads up. This also shows some basic information about the battery charge, battery voltage, and where it is charging from....here is it showing 97% and a high voltage on the battery. You can also see that it is charging from the Pi GPIO (meaning it is plugged in to the Pis microUSB) and it also shows the rail voltages and current draw over the GPIO pins. Below that is the PiJuice microUSB and as you can see in this screenshot that is plugged in. There is a fault checker, a system switch state and also a link to a HAT config menu (more on that later! - see PiJuice HAT Configuration Menu screenshot).

- **Battery:**
  - Battery Charge - 0-100%
  - Battery Voltage - Current battery voltage level
  - Battery Status:
    - **NOT_PRESENT** - Battery is not detected or not installed
    - **CHARGING_FROM_IN** - Battery is charging from PiJuice USB power connector
    - **CHARGING_FROM_5V_IO** - Battery is charging from GPIO pin (Will occur if powered through Raspberry Pi power connector)
    - **NORMAL** - Battery is present but not charging
- **GPIO power input:**
  - Voltage provided/received from the GPIO pins
  - Amperage provided/received from the GPIO pins. Negative current is from Raspberry Pi to PiJuice. Positive current is from PiJuice to Raspberry Pi.
  - GPIO Input Status (Powered from Raspberry Pi)
    - **NOT_PRESENT** - No power supply connected to GPIO pins (i.e Raspberry Pi)
    - **BAD** - GPIO power is bad, find an alternative power supply with a higher rating
    - **WEAK** - GPIO power is weak i.e. power supply cannot charge the PiJuice and provide power to the Raspberry Pi. DPM is active, see - [https://github.com/PiSupply/PiJuice/tree/master/Hardware#usb-micro-input](https://github.com/PiSupply/PiJuice/tree/master/Hardware#usb-micro-input)
    - **PRESENT** - GPIO power is good
- **USB Micro power input:** - PiJuice Micro USB input status
  - **NOT_PRESENT** - Power supply is not connected to the PiJuice micro USB connector
  - **BAD** - Power supply is connected but is not providing enough power
  - **WEAK** - Power supply is connected but is weak i.e. power supply cannot charge the PiJuice and provide power to the Raspberry Pi. DPM is active, see - [https://github.com/PiSupply/PiJuice/tree/master/Hardware#usb-micro-input](https://github.com/PiSupply/PiJuice/tree/master/Hardware#usb-micro-input)
  - **PRESENT** - Power supply is connected and is providing good power to the PiJuice
  - **FAULT:** - Displays any faults
- **System Switch:** for use with VSYS on J3 to provide power to external devices
  - **off** - VSYS pin is off
  - **500mA** - VSYS pin provides up to 500mA of power
  - **2100mA** - VSYS pin provides up to 2100mA of power

**Note:** If the MCU loses power the system switch state reverts to its power up state.

### Wakeup Alarm Menu

<img src="https://user-images.githubusercontent.com/1197294/40877047-6eef9f0e-667b-11e8-91a7-df09da12b48c.png" width="478">

In this screenshot we have moved over to the Wakeup alarm tab of the config menu and as you can see this is an area where you can set schedules for the Pi to automatically wake up. This is useful for remote monitoring applications.

This feature will only work if you are either plugged in to the PiJuice microUSB / running on battery. If the battery is low and you are plugged in via the Raspberry Pis GPIO the only way to enable this feature is by soldering the optional "spring pin" that comes with the PiJuice HAT (See the [hardware section](https://github.com/PiSupply/PiJuice/tree/master/Hardware#unpopulated) for further details).

**Note:** As of Firmware v1.3_2019_01_15 pogo/spring pin is no longer required to wakeup the Raspberry Pi when powering from the Pi micro USB.

**NOTE:** Minutes-period is for recurring wakeups. E.g. when set at 5 it will wakeup at 0, 5, 10, 15, ..., 55 past the hour. The largest meaningful value is 30

#### Repeated Wakeup

When setting the Wakeup alarm for a repeated wakeup, after the initial reboot the Wakeup enabled capability is disabled due to the Raspbian RTC clock initialisation resetting the bit in the PiJuice firmware. To overcome this you will need to run a script to re-enable the wakeup-enable capability. The script is located in PiJuice > Software > Test > wakeup_enable.py

```python
#!/usr/bin/python3
# This script is started at reboot by cron
# Since the start is very early in the boot sequence we wait for the i2c-1 device

import pijuice, time, os

while not os.path.exists('/dev/i2c-1'):
    time.sleep(0.1)

pj = pijuice.PiJuice(1, 0x14)

pj.rtcAlarm.SetWakeupEnabled(True)
```

To run the script every time the Raspberry Pi reboots you will need to add it to the cron table. From the command line run `crontab -e` to edit the cron table.

The first time you run `crontab` you will be prompted to select an editor; if you are not sure which one to use, choose `nano` by pressing Enter.

Add the following to the cron table:

```bash
@reboot /usr/bin/python3 /home/pi/PiJuice/Software/Test/wakeup_enable.py
```

To test the script simply disable the wakeup alarm in the GUI then reboot your Raspberry Pi. You should see that the wakeup alarm checked again.

**Note: When using ID_EEPROM 0x52 the HAT configuration is not read by Raspbian when processing the device-tree. Therefore wakeup-enabled capability is still enabled after a reboot.**

### System Task Menu

![System Task Menu](https://user-images.githubusercontent.com/17354856/46268663-4ea6e600-c501-11e8-9a17-eccfb32dedaa.png "System Task Menu")

Here we have the system task menu tab. This enables you to set the external watchdog timer - useful for remote applications where you can't come and do a hard-reset yourself if the Pi crashes or hangs. The PiJuice essentially monitors for a "heart beat" from the software - if it does not sense it after a defined period of time it automatically resets the Raspberry Pi. You can also set here wakeup on charge levels, minimum battery levels and voltages.

The watchdog timer has a configurable time-out. It defines the time after which it will power cycle if it doesn't receive a heartbeat signal. The time step is in minutes so the minimum time-out period is one minute and the maximum is 65535 minutes. The number can be any whole number between one and 65535. If you set the time to zero the watchdog timer will be disabled.

* **System task enabled** - Check this box to enable one or more of the following options:
* **Watchdog** - You can set a delay here in minutes (maximum of 65535) as to when to power cycle the Raspberry Pi when a heartbeat signal is not detected anymore. Usually this would occur when the system has crashed.
* **Wakeup on charge** - Set a percentage battery value when to wakeup the Raspberry Pi whilst on charge. Usually this value would be high, between 90-100%. This is usually used in-conjunction with "Minimum charge".
* **Minimum charge** - Set a minimum battery percentage level to safely shutdown the Raspberry Pi when the battery is below this value. Low values should be typically between 5-10%. NOTE: The type of system shutdown can be set under "System Events" under "Low charge" menu.
* **Minimum battery voltage** - Set a minimum battery voltage level to safely shutdown the Raspberry Pi when below the set level. Note: The type of system shutdown can be set under "System Events" under "Low battery voltage" menu.
* **Software Halt Power Off** - You can set a delay here in seconds (maximum of 65535) as to when to cut power to the Raspberry Pi when a software shutdown has occurred external to a button press. Usually this would occur when a user has run sudo halt, sudo shutdown -h now, or sudo poweroff in the terminal.

**NOTE: Software Halt Power Off** will cut power at the end of the delay period. Provide an above normal amount of time for the OS to complete shutdown or the SD card may be CORRUPTED.  **RECOMMEND 30 or more seconds.**

### System Events Menu

![System Events Menu](https://user-images.githubusercontent.com/16068311/35161235-7d31d544-fd37-11e7-92b4-dc0ccab55c56.png "System Events Menu")

This is the system events menu tab. It allows you to trigger events for certain scenarios such as low charge, low voltage and more. Each parameter has a couple of preset options to choose from, and also you can select options from the "user scripts" tab which allows you to trigger your own custom scripts when certain system events occur for maximum flexibility.

* **Low charge**. System task monitors charge level and generates this event when charge drops below configurable threshold as set in "System Task"
* **Low battery voltage**. This event is generated when battery voltage drops below configurable threshold as set in "System Task"
* **No power**. System task generates this event when power source disappears and system is powered only with energy from battery.
* **Watchdog reset**. If watchdog reset happened for some reason PiJuice raises watchdog reset fault flag that system task can detect immediately after boot.
* **Button power off**. This event is raised after boot if there was power off triggered by button press.
* **Forced power off**. If there was forced power off caused by loss of energy (battery voltage approached cut-off threshold), PiJuice raises forced power off fault flag that system task can detect immediately after boot.
* **Forced sys power off**. This event is raised if there was forced system switch turn off caused by loss of energy.

#### System Functions

* **NO_FUNC** - Do nothing when system event is triggered.
* **SYS_FUNC_HALT** - System is halted
* **SYS_FUNC_HALT_POW_OFF** - System halts and 5V power regulator and system switch are set to off
* **SYS_FUNC_SYS_OFF_HALT** - System is halted and system switch is set to off and system halts
* **SYS_FUNC_REBOOT** - System reboots
* **USER_EVENT** - Script will not be processed by system task
* **USER_FUNCX** - Run a custom script as set in "User Scripts"

**NOTE:** SYS_FUNC_HALT_POW_OFF still provides power to the Raspberry Pi for a further 60 seconds after shutdown

**NOTE:** When PiJuice is in stand-by mode and 5V power has been removed, total current draw is 0.5mA from the battery

### User Scripts menu

<img src="https://user-images.githubusercontent.com/1197294/40877048-6f17bef8-667b-11e8-85f5-8c296cb4bfd1.png" width="478">

This is the user scripts menu tab as we mentioned in the above screenshot description where you can add paths to custom scripts that you can trigger on events.

User scripts can be assigned to user functions called by system task when configured event arise. This should be non-blocking callback function that implements customised system functions or event logging.

User functions are 4 digit binary coded and have 15 combinations, code 0 is USER_EVENT meant that it will not be processed by system task, but left to user and python API to manage it. The GUI initially only shows 8. Clicking the "Show more" button will show all 15.

**NOTE:** In order for your user script to run you must make sure that it is executable and that system task is enabled in the **System Task** menu. If you are also assigning a user function to a button then you must also assign the user function under the **Buttons** tab in **PiJuice HAT Configuration**. To make your script executable you can do so from the command line with the following command:

```bash
chmod +x user_script.py
```
**NOTE:** Scripts executed by the pijuice.service will run as the "owner" of the script i.e. pi and must belong to the pijuice user group. By default the pijuice install script adds user `pi` to user group `pijuice`.

Scripts now must determine where the interpreter is located as a shebang line so the pijuice service knows that type of script it is:

```bash
#!/usr/bin/python
```

```bash
#!/usr/bin/python3
```

```bash
#!/usr/bin/env bash
```


## PiJuice Configuration

### PiJuice HAT General Config Menu

![PiJuice HAT General Config Menu](https://user-images.githubusercontent.com/16068311/35161230-7caa54d4-fd37-11e7-88cb-a76b2891af4d.png "PiJuice HAT General Config Menu")

In the first config menu screenshot, we mentioned a button in the image that said "Configure HAT" - if you were to click on that button it would bring you to this PiJuice HAT general configuration menu. It allows you to configure a lot of hardware settings on the PiJuice HAT itself (as opposed to the previous menus which were actually configuring the software - hopefully that is not too confusing!)

This is the general tab, which allows you to select whether you have installed the spring pin / run pin and also the I2C addresses of the HAT and the RTC as well as changing the write protect on the eeprom and changing the actual physical I2C address of the eeprom. These eeprom features can be very useful if you want to stack another HAT on top of the PiJuice but still have that other HAT auto-configure itself.

* **Inputs precedence**: Selects what power input will have precedence for charging and supplying VSYS output when both are present, HAT USB Micro Input, GPIO 5V Input. 5V_GPIO selected by default.
* **GPIO Input Enabled**: Enables/disables powering HAT from 5V GPIO Input. Enabled by default.
* **USB Micro current limit**: Selects maximum current that HAT can take from USB Micro connected power source. 2.5A selected by default.
* **USB Micro IN DPM**: Selects minimum voltage at USB Micro power input for Dynamic Power Management Loop. 4.2V set by default.
* **No battery turn on**: If enabled pijuice will automatically power on 5V rail and trigger wake-up as soon as power appears at USB Micro Input and there is no battery. Disabled by default.
* **Power regulator mode**: Selects power regulator mode. POWER_SOURCE_DETECTION by default.


*Note: Using the "Reset to default configuration" will restore the board to its default settings and for a short while the GUI will report "COMMUNICATION_ERROR"*

### PiJuice HAT Config Buttons Menu

![PiJuice HAT Config Buttons Menu](https://user-images.githubusercontent.com/16068311/35161227-7c2194be-fd37-11e7-90ee-521a7d65813f.png "PiJuice HAT Config Buttons Menu")

Next we have the buttons menu - this configures the actions of the buttons on the PiJuice HAT (there are three surface mount buttons, one of which also has a 2 pin 2.54mm header so you can break out a button on a cable to the edge of a case or wherever you fancy).

There are a number of preset behaviours for the buttons - startup/shutdown etc and this menu also ties in to the "User Scripts" menu shown above meaning you can actually trigger your own custom scripts and events based on the press of one of these buttons very easily.

You can even trigger different events for a press, release, single press, double press and two lengths of long press - you can even configure the length of time these long presses would take before triggering the event. As you can see the first button is already configured for system power functionality and we would highly recommend that at least one of the buttons is configured to these settings or you may have issues turning your PiJuice on and off :-)

**NOTE:** The parameter is set in milliseconds for each button press

#### Button events:
* **PRESS**. Triggered immediately after button is pressed
* **RELEASE**: Triggered immediately after button is released
* **SINGLE PRESS**: Triggered if button is released in time less than configurable timeout after button press.
* **DOUBLE PRESS**: Triggered if button is double pressed in time less than configurable timeout.
* **LONG PRESS 1**: Triggered if button is hold pressed hold for configurable time period 1.
* **LONG PRESS 2**: Triggered if button is hold pressed hold for configurable time period 2.

Button events can be configured to trigger predefined or user functions.

#### User Functions

* **NO_FUNC** - Do nothing when system event is triggered.
* **SYS_FUNC_HALT** - System is halted
* **SYS_FUNC_HALT_POW_OFF** - System halts and 5V power regulator and system switch are set to off
* **SYS_FUNC_SYS_OFF_HALT** - System is halted and system switch is set to off and system halts
* **SYS_FUNC_REBOOT** - System reboots
* **USER_EVENT** - Script will not be processed by system task
* **USER_FUNCX** - Run a custom script as set in "User Scripts"

**NOTE:** SYS_FUNC_HALT_POW_OFF still provides power to the Raspberry Pi for a further 60 seconds after shutdown

**NOTE:** When PiJuice is in stand-by mode and 5V power has been removed, total current draw is 0.5mA from the battery

#### Hardware functions
* **POWER ON**: This function will wake-up system. 5V regulator (5V GPIO rail) will be turned on if was off.
* **POWER OFF**: 5V regulator (5V GPIO rail) turns off.
* **RESET**: If run pin is installed then reset is triggered by run signal activation. If run pin is not installed rest is done by power circle at 5V GPIO rail if power source is not present.


### PiJuice HAT Config LEDs Menu

![PiJuice HAT Config LEDs Menu](https://user-images.githubusercontent.com/1197294/59568782-d88ee300-907f-11e9-976b-34abbe6f96b1.png "PiJuice HAT Config LEDs Menu")

Perhaps our favourite options menu is the LEDs menu - as with the buttons we have made these super versatile. They can have standard functions as displayed above, they can have preset functions or you can define custom ways for them to behave.

Each LED can be assigned to predefined predefined function or configured for user software control as User LED.
* **CHARGE STATUS**. LED is configured to signal current charge level of battery. For level <= 15% red with configurable brightness. For level > 15% and level <=50% mix of red and green with configurable brightness. For level > 50% green with configurable brightness. When battery is charging blinking blue with configurable brightness is added to current charge level color. For full buttery state blue component is steady on.

**Red** - R parameter defines color component level of red below 15%\
**Green** - G parameter defines color component charge level over 50%\
**Blue** - B parameter defines color component for charging (blink) and fully charged states (constant)

**NOTE:** Red LED and Green LED will show the charge status between 15% - 50%

* **USER LED**. When LED is configured as User LED it can be directly controlled with User software via command interface. Initial PiJuice power on User LED state is defined with R, G, and B brightness level parameters.

### PiJuice HAT Config Battery Menu

![PiJuice HAT Config Battery Menu](https://user-images.githubusercontent.com/1197294/59568790-2ad00400-9080-11e9-8769-1aaaf6b44e73.png "PiJuice HAT Config Battery Menu")

The battery menu is a very important one. It basically allows you to set charge profiles for the PiJuice charge chip in order to correctly and efficiently charge the battery, correctly monitor the charge percentages and more. We have got a number of built in presets such as the ones that will come with the PiJuice by default (the BP7X) and all of the other ones we will supply. But as promised, there is also the ability to add your own custom charge profiles and even your own battery temperature sensor in order to increase the safety and efficiency of charging your batteries.

* **Capacity**. Charge capacity of battery.
* **Charge current**. [550mA – 2500mA]. Defines constant current that PiJuice battery is charged in current regulation phase of charging process.
* **Termination current**. [50mA – 400mA]. When charging current drops below termination current threshold in voltage regulation phase charging process terminates.
* **Regulation voltage**. [3500mV – 4440mV]. Defines constant voltage to which voltage over battery is regulated in voltage regulation phase of charging process.
* **Cut-off voltage**. [0mV – 5100mV]. Defines minimum voltage at which battery is fully discharged.
* **Cold temperature**. Defines temperature threshold according to JEITA standard below which charging is suspended.
* **Cool temperature**. Defines temperature threshold according to JEITA standard below which charge current is reduced to half of programmed charge current. This threshold should be set above cold temperature.
* **Warm temperature**. Defines temperature threshold according to JEITA standard above which the battery regulation voltage is reduced by 140mV from the programmed regulation voltage. This threshold should be set above cool temperature.
* **Hot temperature**.  Defines temperature threshold according to JEITA standard above which charging is suspended. This threshold should be set above warm temperature.
* **NTC  B constant**. Defines thermistor B constant of NTC temperature sensor if it is integrated with battery.
* **NTC resistance**. Defines nominal thermistor resistance at 25°C of NTC temperature sensor if it is integrated with battery.

As previously mentioned, some of these are even hard coded into the firmware on the PiJuice which enables you to actually select profiles using the PiJuices on board DIP switch.

More information on the default profiles and how to created additional ones can be found in the [Hardware Section](https://github.com/PiSupply/PiJuice/tree/master/Hardware#battery-profiles)

**Available Battery Profiles:**
- BP6X_1400
- BP7X_1820
- SNN5843_2300
- [PjLIPO_12000](https://uk.pi-supply.com/products/pijuice-12000mah-battery)
- [PjLIPO_5000](https://uk.pi-supply.com/products/pijuice-5000mah-battery)
- [PjBP7X_1600](https://uk.pi-supply.com/products/1600-mah-smartphone-battery-compatible-with-pijuice)
- [PjSNN5843_1300](https://uk.pi-supply.com/products/1300-mah-smartphone-battery-compatible-with-pijuice)
- [PjZERO_1200](https://uk.pi-supply.com/products/pijuice-zero-1200mah-battery)
- [PjZERO_1000](https://uk.pi-supply.com/products/pijuice-zero-1000mah-battery)
- [PjLIPO600](https://uk.pi-supply.com/products/pijuice-zero-600mah-battery)
- [PjLIPO_500](https://uk.pi-supply.com/products/pijuice-zero-500mah-battery)

**Note:** Pre-defined battery profile parameters will only be shown when you apply the changes.


### PiJuice HAT Config IO Menu
![PiJuice HAT Config IO Menu](https://user-images.githubusercontent.com/1197294/59568816-adf15a00-9080-11e9-842e-4b1c54a0e47a.png "PiJuice HAT Config IO Menu")

This Tab provides configuration of two pins IO port provided from HAC microcontroller at P3 Header.
Modes selection box provides to program IO pin to one of predefined modes:
* **NOT_USED**: Set IO pin in neutral configuration (passive input).
* **ANALOG_IN**: Set IO pin in analog to digital converter mode. In this mode Value can be read with status function GetIoAnalogInput(). Pull has no effect in this mode.
* **DIGITAL_IN**: Set IO pin in digital input mode. Pull in this mode can be set to NO_PULL, PULLDOWN or PULLUP. Use status function SetIoDigitalOutput() to read input value dynamically.
* **DIGITAL_OUT_PUSHPULL**: Set IO pin in digital output mode with push-pull driver topology. Pull in this mode should be set to NO_PULL. Initial value can be set to 0 or 1. Use status function SetIoDigitalOutput() to control output value dynamically.
* **DIGITAL_IO_OPEN_DRAIN**: Set IO pin in digital output mode with open-drain driver topology. Pull in this mode can be set to NO_PULL, PULLDOWN or PULLUP. Initial value can be set to 0 or 1. Use status function SetIoDigitalOutput() to control output value dynamically.
* **PWM_OUT_PUSHPULL**: Set IO pin to PWM output mode with push-pull driver topology. Pull in this mode should be set to NO_PULL. Period [us] box sets period in microseconds in range [2, 131072] with 2us resolution. Set initial duty_circle in range [0, 100]. Use status function SetIoPWM() to control duty circle dynamically.
* **PWM_OUT_OPEN_DRAIN**: Set IO pin to PWM output mode with open-drain driver topology. Pull in this mode can be set to NO_PULL, PULLDOWN or PULLUP. Period [us] box sets period in microseconds in range [2, 131072] with 2us resolution. Set initial duty_circle in range [0, 100]. Use status function SetIoPWM() to control duty circle dynamically.

Click Apply button to save new settings.

**IO2** can be set as a digital wake-up interrupt on the Raspberry Pi from a peripheral device connected to header P3. The wakeup can be set as one of the following:
* **NO_WAKEUP** - Wakeup function disabled
* **FALLING_EDGE** - High to Low
* **RISING_EDGE** - Low to High


### PiJuice HAT Config Firmware Menu

<img src="https://user-images.githubusercontent.com/1878314/64279139-47286200-cf46-11e9-877e-5062b60e8b44.png" width="654">

Last but very much not least is the firmware menu. This allows you to update the firmware on the PiJuice MCU chip as and when necessary meaning we can actively improve the firmware and any updates or improvements we make in the future can be retrospectively applied to all PiJuice HATs!

*Note: that the PiJuice package you installed comes with a default firmware located at the path below:*
```text
/usr/share/pijuice/data/firmware/
```

the filename would look like `PiJuice-V1.2-2018_05_02.elf.binary`

If you want to use the GUI to update the firmware to a more recent version you will have to override this file with the new one that you can download from our [Firmware section](https://github.com/PiSupply/PiJuice/tree/master/Firmware).

*Remember though that the firmware we provide in the software package you've obtained from either APT or Github is generally the only one you should ever use for that specific version of Software release, therefore only update the firmware if the GUI reports that the firmware is not up to date or if we instruct you to do so.*

During the update the window may become unresponsive. **Wait until the update is finished** before you continue with anything else.

## PiJuice CLI

For those users who would rather run their Raspberry Pi using a Lite version of Raspbian or has remote access to the Pi via SSH, we have also designed a handy little command line interface tool, which ultimately replicates the Graphical Interface for PiJuice software.

To launch the PiJuice CLI simply open up the Terminal or from the command line type in the following command:

`pijuice_cli`

**Note:** Previous versions before v1.4 must run `pijuice_cli.py`
**Note:** 'pijuice_cli' must be run as user Pi

![pijuice cli](https://user-images.githubusercontent.com/1878314/64280401-f23a1b00-cf48-11e9-9a3f-21e7211a72e1.png)

The PiJuice command line interface is an extension of the PiJuice HAT Configuration GUI with the exception that System Task, System Events and User Scripts cannot be configurable from the command line interface. To configure these options you will need to directly modify the JSON file as shown in the next section.

To scroll the menu simply use your arrow keys on your keyboard and press enter to select options or navigate through the menu system.

### Status
This menu shows the current status of the PiJuice including battery levels and charging input method.

![pijuice cli status](https://user-images.githubusercontent.com/1878314/64280524-39281080-cf49-11e9-9779-e19f7499e241.png)

**Note: It is recommended to use a power supply with a current rating of 2.5A such as the official Raspberry Pi power supply.**

**Battery** - This section displays the current battery level as a percentage from 0-100% as well as the battery voltage. Next to this is the current charging status of the battery; NOT_PRESENT battery is not installed, CHARGING_FROM_IN battery is charging from the PiJuice micro USB connector, CHARGING_FROM_5V_IO battery is charging from the GPIO pins via the micro USB connector on the Raspberry Pi, NORMAL battery is not charging but is operating as normal.

**GPIO power input** - This section displays the power state of the GPIO pins as the PiJuice can provide power to the Raspberry Pi through the GPIO pins as well as receive power from the Raspberry Pi if powered through the Raspberry Pi’s micro USB connector.
This section displays the GPIO voltage(V) and current (A) that the battery either provides to the Pi or receives from the Pi as well as the input status;
Negative current is from the Raspberry Pi to PiJuice. Positive current is from the PiJuice to the Raspberry Pi.
Input status NOT_PRESENT no power supply is connected to the Raspberry Pi, BAD power supply is connected to the Raspberry Pi but is not providing enough power to the Raspberry Pi and PiJuice, WEAK power is connected to the Raspberry Pi but not enough to power the Raspberry Pi and charge the PiJuice, PRESENT power supply is connected to the Raspberry Pi and is stable.

**USB Micro power input** - This section displays the status of the PiJuice micro USB power supply. NOT_PRESENT no power supply is connected to the PiJuice, BAD power supply is connected but cannot charge the PiJuice or power the Raspberry Pi, WEAK power supply is connected but not providing enough power, PRESENT power supply is connected and is stable.

**Fault** - This section displays any faults with the power system of the PiJuice.

**System switch** - This section is used to configure the VSYS pin on header J3, which is on the underside of the PiJuice board and on header P3. This pin can be used to provide power to external devices such as the PiBot. You can configure the pin to limit its current output; Off VSYS is switched off, 500mA provides a maximum of 500mA output current, 2100mA provides a maximum of 2100mA output current. To change the System Switch state hit enter on Change Power Switch and then select one of the options in the menu.

To get new data from the PiJuice you can select Refresh to update the values.

### General

In the General menu it allows you to configure a lot of the hardware settings for the PiJuice HAT. The only difference with this menu and the GUI is that you cannot change the I2C address for the PiJuice HAT and the built-in RTC.

![pijuice cli general](https://user-images.githubusercontent.com/1878314/64280607-61177400-cf49-11e9-8969-41314e6694c4.png)

**GPIO Input Enabled** enables/disables powering the PiJuice HAT from 5V GPIO Input (Raspberry Pi). Enabled by default.

**EEPROM write unprotect** allows you to reconfigure the EEPROM on the PiJuice HAT. Selecting this option makes it available for you to write EEPROM data.

**Charging enabled** is by default checked and allows the charging of the battery. If you wish to disable this option then you can deselect this option.

**No battery turn on** If enabled, pijuice will automatically power on 5V rail and trigger wake-up as soon as power appears at USB Micro Input and there is no battery. Disabled by default.

The **Run pin** configuration allows you to set whether the pogo pin is installed on the PiJuice. The pogo pin is required for for use with the wakeup function on the Raspberry Pi.

**Inputs precedence** selects which power input will have precedence for charging and supplying VSYS output when both are present, HAT USB Micro Input, GPIO 5V Input (Raspberry Pi). 5V_GPIO is selected by default.

**USB Micro current** limit selects maximum current that the PiJuice HAT can take from USB Micro connected power source. 2.5A selected by default.

**USB Micro IN DPM** Selects minimum voltage at USB Micro power input for Dynamic Power Management Loop. 4.2V set by default.

**Power regulator mode** selects the type of power regulator from POWER_SOURCE_DETECTION, LDO and DCDC. POWER_SOURCE_DETECTION is selected by default.

Don’t forget that when you make changes to any of the hardware settings you must Apply the settings from the menu, otherwise they will not be saved.

### Buttons
There are a total of three, user configurable buttons, that you can program to trigger certain events.

The default button functions are:

- SW1/J5 is power button by default:
  - Single press to power on (release in less than 800 ms)
  - Long press of at least 10 seconds to halt
  - Long press of at least 20 seconds to cut power

- SW2 is user button by default, configured to trigger user scripts:
  - Single press in less than 400ms to invoke “USER_FUNC1”
  - Double press within 600ms to invoke “USER_FUNC2”

- SW3 is user button by default, configured to trigger user scripts:
  - Press will invoke “USER_FUNC3”
  - Release will invoke “USER_FUNC4”

There are a number of preset behaviours for the buttons - startup/shutdown etc and this menu also ties in to the "User Scripts" menu shown, meaning you can actually trigger your own custom scripts and events based on the press of one of these buttons very easily.
The buttons also have some special functions, which help when debugging the PiJuice and to reset the hardware when things don’t go your way:

- Dual long press of SW1 and SW2 for 20 seconds will reset PiJuice HAT configuration to default. This applies to the MCU configuration only.
- Holding pressed SW3 while powering up PiJuice will initiate the bootloader. This is used only in cases when ordinary initiation through I2C does not work because of damaged firmware.

You can even trigger different events for a press, release, single press, double press and two lengths of long press - you can even configure the length of time these long presses would take before triggering the event. As you can see the first button is already configured for system power functionality and we would highly recommend that at least one of the buttons is configured to these settings or you may have issues turning your PiJuice on and off.

![pijuice buttons](https://user-images.githubusercontent.com/3359418/75063866-b8040c00-54dd-11ea-8d0f-73f234b81831.jpg)

![pijuice cli buttons menu](https://user-images.githubusercontent.com/1878314/64280672-873d1400-cf49-11e9-9c42-4e93602be8b2.png)

In order to program one of the buttons, first select which button you would like to use to trigger and event. In the next menu you can program the type of button press you would like to use from the following settings:

![pijuice cli buttons options](https://user-images.githubusercontent.com/1878314/64280721-9e7c0180-cf49-11e9-838c-db0d578a378b.png)

- **PRESS** - Triggered immediately after button is pressed\
- **RELEASE** - Triggered immediately after button is released\
- **SINGLE PRESS** - Triggered if button is released in time less than configurable timeout after button press\
- **DOUBLE PRESS** - Triggered if button is double pressed in time less than configurable timeout\
- **LONG PRESS 1** - Triggered if button is hold pressed hold for configurable time period 1\
- **LONG PRESS 2** - Triggered if button is hold pressed hold for configurable time period 2

In the next menu option if you select one of the above you can change the type of user function that you wish to trigger when the button is pressed.

![pijuice cli buttons functions](https://user-images.githubusercontent.com/1878314/64280915-fd417b00-cf49-11e9-88e0-785af47a9fc0.png)

- **NO_FUNC** - Does nothing\
- **HARD_FUNC_POWER_ON** - Switches on the Raspberry Pi by applying power to the GPIO pins\
- **HARD_FUNC_POWER_OFF** - Switches off power to the Raspberry Pi by cutting the power to the GPIO pins (Not recommend)\
- **HARD_FUNC_RESET** - Resets power to the Raspberry Pi forcing a reboot\
- **SYS_FUNC_HALT** - Halts the system\
- **SYS_FUNC_HALT_POW_OFF** - Halts the system then power off the 5V power regulator and system switch is set to off\
- **SYS_FUNC_SYS_OFF_HALT** - System is halted and system switch is set to off and system halts\
- **SYS_FUNC_REBOOT** - performs a reboot\
- **USER_EVENT** - Runs a custom script that it not processed by system task\
- **USER_FUNC1** - Runs a custom user script

**NOTE: SYS_FUNC_HALT_POW_OFF still provides power to the Raspberry Pi for a further 60 seconds after shutdown**

**NOTE:** When PiJuice is in stand-by mode and 5V power has been removed, total current draw is 0.5mA from the battery

To set the Parameter of the button press simply enter its value in milliseconds to the Parameter field.

### LEDs

Just like the buttons, the LEDS are also configurable on the PiJuice and there are two of them in total. The LEDs are surface mount RGB LEDs and can be configured in a number of different colours aside from the preset primary colours in use.

![pijuice cli leds](https://user-images.githubusercontent.com/3359418/75063935-d9fd8e80-54dd-11ea-8fdf-8addb6aac532.jpg)

![pijuice cli leds](https://user-images.githubusercontent.com/1878314/64281033-311ca080-cf4a-11e9-876d-4f2dc10336a7.png)

Each LED can be assigned to a predefined function or configured for user software control as User LED. There are two user functions that are available:

- **CHARGE STATUS** - LED is configured to signal current charge level of the battery. For level <= 15% red with configurable brightness. For level > 15% and level <=50% mix of red and green with configurable brightness. For level > 50% green with configurable brightness. When battery is charging blinking blue with configurable brightness is added to current charge level color. For full buttery state blue component is steady on. Current D1 LED values:

R: 60\
G: 60\
B: 100

- **USER LED** - When LED is configured as User LED it can be directly controlled with User software via command interface. Initial PiJuice power, on User LED state is defined with R, G, and B brightness level parameters.

Example:

To change the values of LED D2, select D2 from the menu. Then select the USER_LED function and then add your own values to RGB.

![pijuice cli rgb](https://user-images.githubusercontent.com/1878314/64281082-45609d80-cf4a-11e9-97c0-1d7ca7f19ff8.png)

Once finished making the changes go Back and then select Apply Settings for the changes to take effect.

![pijuice cli save changes](https://user-images.githubusercontent.com/1878314/64281190-80fb6780-cf4a-11e9-859e-c26cf1491755.png)

### Battery Profile

The Battery profile menu allows you to set up the battery profile, which includes all the battery characteristics such as capacity, voltages and temperature. It is very important that the battery profile is set correctly in order for your battery to operate efficiently and correctly monitor the voltage and battery levels.

There are a number of built-in battery profile presets such as the ones that will come with the PiJuice by default (BP7X) and all of the other ones that will be supplied. These default battery profiles can be set using the hardware DIP switch on the reverse side of the PiJuice board, so no software would be required.

![pijuice battery dip switches](https://user-images.githubusercontent.com/16068311/34769251-25c7c3b6-f5f5-11e7-971f-e93f5d4d3cc0.jpg)

To change the battery profile you will need to select **Profile** option and then select from one of the following battery profiles. If you have connected your own battery to to the PiJuice then you will need to select **Custom** from the profile options.

![pijuice cli battery menu](https://user-images.githubusercontent.com/1878314/64281292-bb650480-cf4a-11e9-90ff-1ef684971fe8.png)

* **Capacity**. Charge capacity of battery.
* **Charge current**. [550mA – 2500mA]. Defines constant current that PiJuice battery is charged in current regulation phase of charging process.
* **Termination current**. [50mA – 400mA]. When charging current drops below termination current threshold in voltage regulation phase charging process terminates.
* **Regulation voltage**. [3500mV – 4440mV]. Defines constant voltage to which voltage over battery is regulated in voltage regulation phase of charging process.
* **Cut-off voltage**. [0mV – 5100mV]. Defines minimum voltage at which battery is fully discharged.
* **Cold temperature**. Defines temperature threshold according to JEITA standard below which charging is suspended.
* **Cool temperature**. Defines temperature threshold according to JEITA standard below which charge current is reduced to half of programmed charge current. This threshold should be set above cold temperature.
* **Warm temperature**. Defines temperature threshold according to JEITA standard above which the battery regulation voltage is reduced by 140mV from the programmed regulation voltage. This threshold should be set above cool temperature.
* **Hot temperature**.  Defines temperature threshold according to JEITA standard above which charging is suspended. This threshold should be set above warm temperature.
* **NTC  B constant**. Defines thermistor B constant of NTC temperature sensor if it is integrated with battery.
* **NTC resistance**. Defines nominal thermistor resistance at 25°C of NTC temperature sensor if it is integrated with battery.

Before you can start changing the values of the battery profile you will need to make sure that you select **Custom** from the **Battery Settings** menu, when it is selected you will see a cross in the box.

![pijuice cli battery profile](https://user-images.githubusercontent.com/1878314/64281387-f1a28400-cf4a-11e9-8b59-2d013bdac583.png)

There is an option at the end of the menu which allows you to set weather your battery had a built-in NTC temperature sensor. An NTC temperature sensor will allow you to monitor the battery temperature for charging and is ultimately there for safety reasons. If your battery does not have an NTC temperature sensor then you can disable this option from the menu **Temperature sense**.

![pijuice cli battery NTC](https://user-images.githubusercontent.com/1878314/64281502-30383e80-cf4b-11e9-9889-2ce61fd3bd66.png)

There is also an alternative method for measuring the charge of the battery by the MCU directly in case the fuel gauge IC is not available or is faulty.

![pijuice cli rsoc](https://user-images.githubusercontent.com/1878314/64281975-498dba80-cf4c-11e9-8d1c-e7c4f6b3d115.png)

Any changes made to the battery profile must be saved using the Apply Settings option.

**Available Battery Profiles:**
- BP6X_1400
- BP7X_1820
- SNN5843_2300
- [PjLIPO_12000](https://uk.pi-supply.com/products/pijuice-12000mah-battery)
- [PjLIPO_5000](https://uk.pi-supply.com/products/pijuice-5000mah-battery)
- [PjBP7X_1600](https://uk.pi-supply.com/products/1600-mah-smartphone-battery-compatible-with-pijuice)
- [PjSNN5843_1300](https://uk.pi-supply.com/products/1300-mah-smartphone-battery-compatible-with-pijuice)
- [PjZERO_1200](https://uk.pi-supply.com/products/pijuice-zero-1200mah-battery)
- [PjZERO_1000](https://uk.pi-supply.com/products/pijuice-zero-1000mah-battery)
- [PjLIPO600](https://uk.pi-supply.com/products/pijuice-zero-600mah-battery)
- [PjLIPO_500](https://uk.pi-supply.com/products/pijuice-zero-500mah-battery)

**Note:** Pre-defined battery profile parameters will only be shown when you apply the changes.

### IO

The microcontroller on the PiJuice board had a number of available Input/Output pins that we can use in our projects. These pins are populated on a female header P3, which is next to the battery.

![pijuice cli io menu](https://user-images.githubusercontent.com/1878314/64285360-8b6e2f00-cf53-11e9-9020-805727918e69.png)

This menu option provides configuration for two IO port pin, IO1 and IO2. You can configure the pins to one of the pre-defined modes below:

- **NOT_USED:** Set IO pin in neutral configuration (passive input)
- **ANALOG_IN:** Set IO pin in analog to digital converter mode. In this mode Value can be read with status function GetIoAnalogInput(). Pull has no effect in this mode.
- **DIGITAL_IN:** Set IO pin in digital input mode. Pull in this mode can be set to NO_PULL,
PULLDOWN or PULLUP. Use status function SetIoDigitalOutput() to read input value dynamically.
- **DIGITAL_OUT_PUSHPULL:** Set IO pin in digital output mode with push-pull driver topology. Pull in this mode should be set to - NO_PULL. Initial value can be set to 0 or 1. Use status function SetIoDigitalOutput() to control output value dynamically.
- **DIGITAL_IO_OPEN_DRAIN:** Set IO pin in digital output mode with open-drain driver topology. Pull in this mode can be set to NO_PULL, PULLDOWN or PULLUP. Initial value can be set to 0 or 1. Use status function SetIoDigitalOutput() to control output value dynamically.
- **PWM_OUT_PUSHPULL:** Set IO pin to PWM output mode with push-pull driver topology. Pull in this mode should be set to NO_PULL. Period [us] box sets period in microseconds in range [2, 131072] with 2us resolution. Set initial duty_circle in range [0, 100]. Use status function SetIoPWM() to control duty cycle dynamically.
- **PWM_OUT_OPEN_DRAIN:** Set IO pin to PWM output mode with open-drain driver topology. Pull in this mode can be set to NO_PULL, PULLDOWN or PULLUP. Period [us] box sets period in microseconds in range [2, 131072] with 2us resolution. Set initial duty_circle in range [0, 100]. Use status function SetIoPWM() to control duty cycle dynamically.

In the menu you can select from two options, **Mode** and **Pull**.

![pijuice cli io options](https://user-images.githubusercontent.com/1878314/64285654-d720d880-cf53-11e9-8aa1-0e967d4d66fd.png)

**Mode** provides a list of modes that you can select as mentioned previously above.

![pijuice cli io modes](https://user-images.githubusercontent.com/1878314/64285704-fc154b80-cf53-11e9-9d8f-271ce1f4eb4b.png)

**Pull**, allows you to select the type of resistor pull for that particular pin. A **PULLUP** configuration will always default to 5V and a **PULLDOWN** will always default to GND.

![pijuice cli io pull](https://user-images.githubusercontent.com/1878314/64285763-1e0ece00-cf54-11e9-910c-e00f8118d91c.png)

**IO2** when configured as **DIGITAL_IN** can be set as a digital wake-up interrupt on the Raspberry Pi from a peripheral device connected to header P3.

![pijuice cli io2 digital in](https://user-images.githubusercontent.com/1197294/65392411-f782cc80-dd74-11e9-90cc-69cfcdfd22d0.png)

The wakeup can be set as one of the following:
* **NO_WAKEUP** - Wakeup function disabled
* **FALLING_EDGE** - High to Low
* **RISING_EDGE** - Low to High

![pijuice cli wakeup options](https://user-images.githubusercontent.com/1197294/65392430-3022a600-dd75-11e9-9be6-001d7610cb58.png)

### Wakeup Alarm

In this menu you can set the Raspberry Pi to automatically wakeup according to a schedule. This menu can be particular useful for remote monitoring application where the Raspberry Pi will wakeup, run a script and then go to sleep again until next time.

![pijuice cli wakeup](https://user-images.githubusercontent.com/1878314/64286304-1ef42f80-cf55-11e9-9387-d3dbc271e253.png)

First thing you will need to do it set the system time, which will synchronise and update the internal clock. This is important to make sure you Raspberry Pi will wakeup at the correct time/day set. Simply select the Set system time option.

Next you can enable/disable the wakeup function by checking the Wakeup enabled box under the current time and date field. When the alarm goes off it will check if this box is enabled and then either wakeup the Raspberry Pi from a power down state or do nothing.

**Note: When the Raspberry Pi wakes up using this method the Wakeup enabled will be disabled by default.**

In the options following this you can set your alarm by entering the Day, Hour, minute, second and also if you wish to wake up the Raspberry Pi on a particular weekday or every day or hour. Once you have made the changes you will then need to make sure that you select the Set alarm option to save the alarm settings.

### Firmware

From time to time we are constantly improving the software and firmware on the PiJuice HAT board. In this menu you can check if there is an Firmware update available and if there is then you can update to the latest version.

![pijuice cli firmware](https://user-images.githubusercontent.com/1878314/64286346-39c6a400-cf55-11e9-97f8-6ea280c434e2.png)

### System Task

![pijuice cli system task](https://user-images.githubusercontent.com/1197294/65378623-63086380-dcbb-11e9-99f4-226d262dec19.png)

Here we have the system task menu tab. This enables you to set the external watchdog timer - useful for remote applications where you can't come and do a hard-reset yourself if the Pi crashes or hangs. The PiJuice essentially monitors for a "heart beat" from the software - if it does not sense it after a defined period of time it automatically resets the Raspberry Pi. You can also set here wakeup on charge levels, minimum battery levels and voltages.

The watchdog timer has a configurable time-out. It defines the time after which it will power cycle if it doesn't receive a heartbeat signal. The time step is in minutes so the minimum time-out period is one minute and the maximum is 65535 minutes. The number can be any whole number between one and 65535. If you set the time to zero the watchdog timer will be disabled.

* **System task enabled** - Check this box to enable one or more of the following options:
* **Watchdog** - You can set a delay here in minutes (maximum of 65535) as to when to power cycle the Raspberry Pi when a heartbeat signal is not detected anymore. Usually this would occur when the system has crashed.
* **Wakeup on charge** - Set a percentage battery value when to wakeup the Raspberry Pi whilst on charge. Usually this value would be high, between 90-100%. This is usually used in-conjunction with "Minimum charge".
* **Minimum charge** - Set a minimum battery percentage level to safely shutdown the Raspberry Pi when the battery is below this value. Low values should be typically between 5-10%. NOTE: The type of system shutdown can be set under "System Events" under "Low charge" menu.
* **Minimum battery voltage** - Set a minimum battery voltage level to safely shutdown the Raspberry Pi when below the set level. Note: The type of system shutdown can be set under "System Events" under "Low battery voltage" menu.
* **Software Halt Power Off** - You can set a delay here in seconds (maximum of 65535) as to when to cut power to the Raspberry Pi when a software shutdown has occurred external to a button press. Usually this would occur when a user has run sudo halt, sudo shutdown -h now, or sudo poweroff in the terminal.

**NOTE: Software Halt Power Off** will cut power at the end of the delay period. Provide an above normal amount of time for the OS to complete shutdown or the SD card may be CORRUPTED.  **RECOMMEND 30 or more seconds.**

### System Events Menu

![System Events Menu](https://user-images.githubusercontent.com/1878314/64287275-1ef52f00-cf57-11e9-965c-ade4f0dd6176.png "System Events Menu")

This is the system events menu. It allows you to trigger events for certain scenarios such as low charge, low voltage and more. Each parameter has a couple of preset options to choose from, and also you can select options from the "user scripts" tab which allows you to trigger your own custom scripts when certain system events occur for maximum flexibility.

* **Low charge**. System task monitors charge level and generates this event when charge drops below configurable threshold as set in "System Task"
* **Low battery voltage**. This event is generated when battery voltage drops below configurable threshold as set in "System Task"
* **No power**. System task generates this event when power source disappears and system is powered only with energy from battery.
* **Watchdog reset**. If watchdog reset happened for some reason PiJuice raises watchdog reset fault flag that system task can detect immediately after boot.
* **Button power off**. This event is raised after boot if there was power off triggered by button press.
* **Forced power off**. If there was forced power off caused by loss of energy (battery voltage approached cut-off threshold), PiJuice raises forced power off fault flag that system task can detect immediately after boot.
* **Forced sys power off**. This event is raised if there was forced system switch turn off caused by loss of energy.

#### System Functions

* **NO_FUNC** - Do nothing when system event is triggered.
* **SYS_FUNC_HALT** - System is halted
* **SYS_FUNC_HALT_POW_OFF** - System halts and 5V power regulator and system switch are set to off
* **SYS_FUNC_SYS_OFF_HALT** - System is halted and system switch is set to off and system halts
* **SYS_FUNC_REBOOT** - System reboots
* **USER_EVENT** - Script will not be processed by system task
* **USER_FUNCX** - Run a custom script as set in "User Scripts"

**NOTE:** SYS_FUNC_HALT_POW_OFF still provides power to the Raspberry Pi for a further 60 seconds after shutdown

**NOTE:** When PiJuice is in stand-by mode and 5V power has been removed, total current draw is 0.5mA from the battery

### User Scripts menu

<img width="668" alt="Screen Shot 2019-09-04 at 21 01 50" src="https://user-images.githubusercontent.com/1878314/64287319-3fbd8480-cf57-11e9-9454-011466c9a216.png">

This is the user scripts menu as we mentioned in the above screenshot description where you can add paths to custom scripts that you can trigger on events.

User scripts can be assigned to user functions called by system task when configured event arise. This should be non-blocking callback function that implements customised system functions or event logging.

User functions are 4 digit binary coded and have 15 combinations, code 0 is USER_EVENT meant that it will not be processed by system task, but left to user and python API to manage it. The GUI initially only shows 8. Clicking the "Show more" button will show all 15.

**NOTE:** In order for your user script to run you must make sure that it is executable and that system task is enabled in the **System Task** menu. If you are also assigning a user function to a button then you must also assign the user function under the **Buttons** tab in **PiJuice HAT Configuration**. To make your script executable you can do so from the command line with the following command:

```bash
chmod +x user_script.py
```
**NOTE:** Scripts executed by the pijuice.service will run as the "owner" of the script i.e. pi and must belong to the pijuice user group. By default the pijuice install script adds user `pi` to user group `pijuice`.

Scripts now must determine where the interpreter is located as a shebang line so the pijuice service knows that type of script it is:

```bash
#!/usr/bin/python
```

```bash
#!/usr/bin/python3
```

```bash
#!/usr/bin/env bash
```


## PiJuice RTC

The PiJuice MCU has a built-in RTC, which is then kept alive by the PiJuice battery. This RTC is primarily used to wakeup the Raspberry Pi at a specific time/date set by you in the PiJuice configuration settings. By default when the PiJuice has an ID EEPROM address of 0x50 the RTC driver is loaded when the Raspberry Pi boots and this can be confirmed by running 'i2cdetect -y 1' from the command line where you will see 'UU' at address 68.

```
pi@rpi-stretch-full:~ $ i2cdetect -y 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- --
10: -- -- -- -- 14 -- -- -- -- -- -- -- -- -- -- --
20: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
60: -- -- -- -- -- -- -- -- UU -- -- -- -- -- -- --
70: -- -- -- -- -- -- -- --                         
pi@rpi-stretch-full:~ $
```

If your ID EEPROM address is set to 0x52 (Which you will need to change if stacking another HAT) then you will need to manually load the driver in the `/boot/config.txt` by adding the following line:

```dtoverlay=i2c-rtc,ds1339```

### Setting the System Clock & RTC

There are two ways in which you can set your system time and date providing that you have set the timezone in Raspbian; automatically using timesync or manually in the command line. If you have an internet connection then the time will automatically be synched after boot and this will also sync with the RTC time.

Manually you can set the time with the `date` command from the command line:

```sudo date -s '2018-10-29 12:30:46' ```

After setting the date and time you must then copy the system time to the RTC with the command:

```sudo hwclock -w```

You can check this with:

```sudo hwclock -r```

### Sync RTC time at boot

When the Raspberry Pi shutsdown and then reboots you must copy the RTC time back to the system clock at boot and you can do this in `/etc/rc.local` with `sudo hwclock -s`.

**Note:** This assumes that your PiJuice has sufficient power from the battery to keep the simulated RTC running in the PiJuice microcontroller while the Pi is shut down.


## JSON configuration file
Changes made on tabs "System Task", "System Events" and "User Scripts" on the main windows will be saved on a JSON file.

`/var/lib/pijuice/pijuice_config.JSON`

here is an example of a configuration.

```text
{
  "system_events": {
    "low_battery_voltage": {
      "function": "SYS_FUNC_HALT",
      "enabled": true
    },
    "low_charge": {
      "function": "NO_FUNC",
      "enabled": true
    },
    "button_power_off": {
      "function": "USER_FUNC1",
      "enabled": true
    },
    "forced_power_off": {
      "function": "USER_FUNC2",
      "enabled": true
    },
    "no_power": {
      "function": "SYS_FUNC_HALT_POW_OFF",
      "enabled": true
    },
    "forced_sys_power_off": {
      "function": "USER_FUNC3",
      "enabled": true
    },
    "watchdog_reset": {
      "function": "USER_EVENT",
      "enabled": true
    }
  },
  "user_functions": {
    "USER_FUNC8": "",
    "USER_FUNC1": "/home/pi/user-script.sh",
    "USER_FUNC2": "",
    "USER_FUNC3": "",
    "USER_FUNC4": "",
    "USER_FUNC5": "",
    "USER_FUNC6": "",
    "USER_FUNC7": ""
  },
  "system_task": {
    "watchdog": {
      "enabled": true,
      "period": "60"
    },
    "min_bat_voltage": {
      "threshold": "1",
      "enabled": true
    },
    "min_charge": {
      "threshold": "1",
      "enabled": true
    },
    "enabled": true,
    "wakeup_on_charge": {
      "enabled": true,
      "trigger_level": "1"
    },
    "ext_halt_power_off": {
      "enabled": true,
      "period": "30"
    }
  }
}
```

For the light version of PiJuice changes can be done directly on the JSON file.
Here is a list of accepted values for the various fields above.
* **system_events**:
    - low_battery_voltage low_charge no_power:
        * enabled: true, false
        * function:
            - NO_FUNC
            - SYS_FUNC_HALT
            - SYS_FUNC_HALT_POW_OFF
            - SYS_FUNC_SYS_OFF_HALT
            - SYS_FUNC_REBOOT
            - USER_EVENT
            - USER_FUNC1 .. USER_FUNC15
    - button_power_off, forced_power_off, forced_sys_power_off, watchdog_reset
        * enabled: true, false
        * function:
            - NO_FUNC
            - USER_EVENT
            - USER_FUNC1 .. USER_FUNC15
* **system_task**:
    - enabled: true, false
    - watchdog
        - enabled: true, false
        - period (minutes): 1..65535
    - min_bat_voltage
        - enabled: true, false
        - threshold (V): 0..10
    - min_charge
        - enabled: true, false
        - threshold (%): 0..100
    - wakeup_on_charge
        - enabled: true, false
        - trigger_level (%): 0..100
    - ext_halt_power_off
        - enabled: true, false
        - period (seconds): 10..65535
* **user_functions**:
    - absolute path to user defined script

**NOTE: ext_halt_power_off will cut power at the end of the period. Provide above normal amount of time for OS to complete shutdown or the SD card may be CORRUPTED.  RECOMMEND 30 or more seconds.**

### Adding USER_FUNC from 9 to 15

The user functions section of the JSON file looks like the following. To add USER_FUNC from 9 to 15 simply append them to the existing ones.

```text
 "user_functions": {    
    "USER_FUNC1": "",
    "USER_FUNC2": "",
    "USER_FUNC3": "",
    "USER_FUNC4": "",
    "USER_FUNC5": "",
    "USER_FUNC6": "",
    "USER_FUNC7": "",
    "USER_FUNC8": "",
    "USER_FUNC9": "",
    ...
    "USER_FUNC15": ""
  },

```
When you manually edit the JSON file and save the settings, the changes will not take effect until the pijuice.service has been restarted. You can restart it by sending a SIGHUP signal to the pijuice.service using the following commands from the terminal window:

1. First you need to find the process ID of the pijuice_sys.py by typing in the following command sin the terminal:

`ps ax | grep pijuice_sys | grep -v grep`

2. You should see something like the following:

 `372 ?        Ss     0:12 /usr/bin/python /usr/bin/pijuice_sys.py`

3. Now to restart the process send the following command, which will restart and then loads the new JOSN configuration changes:

`sudo kill -SIGHUP 372`

**Note:** You process ID will differ from the above example

## I2C Command API
PiJuice HAT provides control, status and configuration of supported features through I2C Command API. Read/write commands are based on I2C block read/write transfers where messages carrying data are exchanged with Master. Message starts with one byte command code, followed by data payload and with checksum byte at the end of message. Checksum is 8-bit XOR calculated over all data payload bytes.

### Command Abstraction Layer
In order to facilitate communication with PiJuice HAT using I2C Command API there is abstraction layer that encapsulates commands into more intuitive interface to configure, control and retrieve status of PiJuice features. This layer is implemented as python script module pijuice.py. Different types of interface function are encapsulated in next set of classes:
* **PiJuiceInterface**. Functions for low level message exchange end error checking through I2C bus.
* **PiJuiceStatus** Functions for dynamically controlling and reading status of PiJuice features.
* **PiJuiceRtcAlarm** Functions for setting-up real time clock and wake-up alarm.
* **PiJuicePower** Power management functions.
* **PiJuiceConfig** Functions for static configuration that mostly involves non-volatile data that saves in EEPROM.
All the function classes are encapsulated in top level object PiJuice(bus, address), where bus presents I2C bus identifier and address presents PiJuice HAT I2C slave address.
Usage example:
```python
#!/usr/bin/python3
from pijuice import PiJuice # Import pijuice module
pijuice = PiJuice(1, 0x14) # Instantiate PiJuice interface object
print(pijuice.status.GetStatus()) # Read PiJuice status.
```
Commands are encapsulated with two type of functions, Setters that writes configuration and control data to PiJuice and Getters that reads status or current configuration/control data.
Every function returns object of dictionary type containing communication error status:
```python
{
'error':error_status
}
```
Where error_staus value can be NO_ERROR in case data are exchanged with no communication errors or value that describers error in cases where communication fails. In case of Getter functions additions additional data object is returned in case of successful data read with value that presents returned data:
```python
{
'error':error_status,
'data':data
}
```
**Note:** All user scripts that import the pijuice module must now be run as Python3

#### PiJuice Status

**GetStatus()**

Gets basic PiJuice status information about power inputs, battery and events.
Returns:
```python
{'data':{
'isFault':is_fault,
'isButton':is_button,
'battery':battery_status,
'powerInput':power_input_status,
'powerInput5vIo':5v_power_input_status
}}
```
Where:
* `is_fault` is `True` if there are faults or fault events waiting to be read or `False` if there are no faults and no fault events.
* `is_button` is `True` if there are button events, `False` if not.
* `battery_status` is a string constant that describes the current battery status, one of four: `'NORMAL'`, `'CHARGING_FROM_IN'`, `'CHARGING_FROM_5V_IO'`, `'NOT_PRESENT'`.
* `power_input_status` is a string constant that describes current status of USB Micro power input, one of four: `'NOT_PRESENT'`, `'BAD'`, `'WEAK'`, `'PRESENT'`.
* `5v_power_input_status`: is a string constant that describes current status of the 5V GPIO power input, one of four: `'NOT_PRESENT'`, `'BAD'`, `'WEAK'`, `'PRESENT'`.

Example:
```python
print(pijuice.status.GetStatus())
```
Returns:
```python
{'data': {'battery': 'CHARGING_FROM_5V_IO', 'powerInput5vIo': 'PRESENT', 'isFault': False, 'isButton': False, 'powerInput': 'NOT_PRESENT'}, 'error': 'NO_ERROR'}
```

**GetChargeLevel()**

Gets the current charge level percentage.
Returns:
```python
{'data':charge_level}
```
Where charge_level is the percentage of charge, [0 - 100]%.

Example:
```python
print(pijuice.status.GetChargeLevel())
```
Returns:
```python
{'data': 57, 'error': 'NO_ERROR'}
```

**GetFaultStatus()**

Get the current fault status of PiJuice HAT.

Example:
```python
print(pijuice.status.GetFaultStatus())
```
Returns:
```python
{'data': {'button_power_off': True, 'forced_power_off': True}, 'error': 'NO_ERROR'}
```
Where data contains:
* `button_power_off`
* `forced_power_off`
* `forced_sys_power_off`
* `watchdog_reset`
* `battery_profile_invalid`: ['NORMAL', 'SUSPEND', 'COOL', 'WARM']

**GetButtonEvents()**

Gets the events generated by PiJuice buttons presses.
Returns:
```python
{'data': {'SW1':event, 'SW2':event, 'SW3':event}}
```
Where event is the detected event name for corresponding button and can be one of: `'PRESS'`, `'RELEASE'`, `'SINGLE_PRESS'`, `'DOUBLE_PRESS'`, `'LONG_PRESS1'`, `'LONG_PRESS2'` if an event is generated or `'NO_EVENT'` if event is absent.
Example:
```python
print(pijuice.status.GetButtonEvents())
```
Returns:
```python
{'data': {'SW1': 'NO_EVENT', 'SW3': ' SINGLE_PRESS', 'SW2': 'NO_EVENT'}, 'error': 'NO_ERROR'}
```

**AcceptButtonEvent(button)**

Clears generated button event.
Arguments:
* `button`: button designator, one of: `'SW1'`, `'SW2'`, `'SW3'`.
Example:
```python
print(pijuice.status.AcceptButtonEvent('SW2'))
```

**GetBatteryTemperature()**

Returns:
```python
{'data': 44, 'error': 'NO_ERROR'}
```
The returned value is the temperature in Celsius.

**GetBatteryVoltage()**

Returns:
```python
{'data': 3562, 'error': 'NO_ERROR'}
```
The returned value is Voltage in millivolts (mV).

**GetBatteryCurrent()**

Returns:
```python
{'data': 2256, 'error': 'NO_ERROR'}
```
The returned value is Current in milliamps (mA).

**GetIoVoltage()**

Returns:
```python
{'data': 5009, 'error': 'NO_ERROR'}
```
Value returned is voltage supplied from the GPIO power output from the PiJuice or when charging, voltage value returned is supplied voltage. Value returned is in millivolts (mV).

**GetIoCurrent()**

Returns:
```python
{'data': 1396, 'error': 'NO_ERROR'}
```
Value returned is current supplied from the GPIO power output from the PiJuice or when charging, current value returned is supplied current. Value returned is in milliamps (mA).

**SetLedState(led, rgb)**

Sets red, green and blue brightness levels for LED configured as “User LED”.

Arguments:
* `led`: LED designator, one of: `'D1'`, `'D2'`.
* `rgb`: [r, g, b] - array of brightness levels of LED components, where r, g and b, are in range [0 – 255].

Example:
```python
print(pijuice.status.SetLedState(‘D2’, [127, 0, 200]))
```

**GetLedState(led)**

Gets current brightness levels for LED configured as “User LED”.

Arguments:
* `led`: LED designator, one of: 'D1', 'D2'.
Returns:
```python
{'data':[r, g, b]}
```
where [r, g, b] is array of brightness levels of LED components, where r, g and b, are in range [0 – 255].

Example:
```python
print(pijuice.status.GetLedState('D1'))
```
Returns:
```python
{'data': [127, 0, 200], 'error': 'NO_ERROR'}
```

**SetLedBlink(led, count, rgb1, period1, rgb2, period2)**

Plays blink pattern on LED configured as “User LED”.

Arguments:
* `led`: LED designator, one of: 'D1', 'D2'.
* `count`: number of blinks for count in range [1 - 254], blink indefinite number of times for count = 255.
* `rgb1`: [r, g, b] is array of brightness levels of LED components in first period of blink, where r, g and b, are in range [0 – 255].
* `period1`: duration of first blink period in range [10 – 2550] milliseconds.
* `rgb2`: [r, g, b] is array of brightness levels of LED components in second period of blink, where r, g and b, are in range [0 – 255].
* `period2`: duration of second blink period in range [10 – 2550] milliseconds.

Example:
```python
pijuice.status.SetLedBlink('D2', 10, [0,200,100], 1000, [100, 0, 0], 500)
```

Note: `SetLedBlink` sends the command to the microprocessor on the PiJuice board, which then executes it. Therefore, you have to wait in your Python program before you send the next `SetLedBlink`.

i.e.
```python
pijuice.status.SetLedBlink('D2', 1,(244, 66, 104), 0.3, (0, 0, 52), 0.1)
pijuice.status.SetLedBlink('D2', 1,(255, 33, 52), 0.5, (0, 0, 0), 0.75)
```
must be written as:

```python
pijuice.status.SetLedBlink('D2', 1,[244, 66, 104], 300, [0, 0, 52], 100)
sleep(0.4)
pijuice.status.SetLedBlink('D2', 1,[255, 33, 52], 500, [0, 0, 0], 750)
sleep(1.25)
```

**GetLedBlink(led)**

Gets current settings of blink pattern for LED configured as “User LED”.

Arguments:
* `led`: LED designator, one of: `'D1'`, `'D2'`.

Returns:
```python
{'data': {
'count':count,
'rgb1':rgb1,
'period1':period1,
'rgb2':rgb2,
'period2':period2
}}
```
Example:
```python
print(pijuice.status.GetLedBlink('D2'))
```
Returns:
```python
{'data': {'count': 10, 'period2': 500, 'rgb2': [100, 0, 0], 'rgb1': [0, 200, 100], 'period1': 1000}, 'error': 'NO_ERROR'}
```

**GetIoDigitalInput(pin)**

Gets state at IO pin configured as digital input.

Arguments:
* `pin`: IO pin designator, 1 for IO1, 2 for IO2.

Returns:
```python
{'data':input_state}
```
Where `input_state` is 0 for low input state, 1 for high.
Example:
```python
print(pijuice.status.GetIoDigitalInput(1))
```
Returns:
```python
{'data': 0, 'error': 'NO_ERROR'}
```

**SetIoDigitalOutput(pin, value)**

Sets state at IO pin configured as digital output.

Arguments:
* `pin`: IO pin designator, 1 for IO1, 2 for IO2.
* `value`: output state to set, 0 for low output state, 1 for high.

Example:
```python
print(pijuice.status.SetIoDigitalOutput(1, 1))
```

**GetIoDigitalOutput(pin)**

Gets current output state at IO pin configured as digital output.

Arguments:
* `pin`: IO pin designator, 1 for IO1, 2 for IO2.

Returns:
```python
{'data':output_state}
```
Where `output_state` is 0 for low output state, 1 for high.
Example:
```python
print(pijuice.status.GetIoDigitalOutput(1))
```
Returns:
```python
{'data': 1, 'error': 'NO_ERROR'}
```

**GetIoAnalogInput(pin)**

Gets voltage in millivolts at IO pin configured as analog input.

Arguments:
* `pin`: IO pin designator, 1 for IO1, 2 for IO2.

Returns:
```python
{'data':analog_value}
```
where `analog_value` is voltage in millivolts measured at analog input.
Example:
```python
print(pijuice.status.GetIoAnalogInput(1))
```
Returns:
```python
{'data': 2222, 'error': 'NO_ERROR'}
```

**SetIoPWM(pin, dutyCircle)**

Sets PWM duty circle at IO pin configured as PWM output.

Arguments:
* `pin`: IO pin designator, 1 for IO1, 2 for IO2.
* `dutyCircle`: pulse width as percentage of period, [0 - 100]%.

Example:
```python
print(pijuice.status.SetIoPWM(2, 35.6))
```

**GetIoPWM(pin)**

Gets current PWM duty circle at IO pin configured as PWM output.

Arguments:
* `pin`: IO pin designator, 1 for IO1, 2 for IO2.

Returns:
```python
{'data': duty_circle}
```
where `duty_circle` is pulse width as percentage of period.

Example:
```python
print(pijuice.status.GetIoPWM(2))
```
Returns:
```python
{'data': 35.59984130375072, 'error': 'NO_ERROR'}
```

#### PiJuice Rtc Alarm

PiJuice RTC Alarm functions allow you to get the current status of the alarm flag as well as setting the alarm and enabling the wakeup function. You can also get current time set in the RTC as well as setting the time. `pijuice.rtcAlarm.xxxxx()`

**GetControlStatus()**
Returns:
```python
{'data': {'alarm_wakeup_enabled': False, 'alarm_flag': False}, 'error': 'NO_ERROR'}
```

Functions checks to see if wakeup has been enabled and if the wakeup alarm flag has been set.

**ClearAlarmFlag()**
Returns:
```python
{'error': 'NO_ERROR'}
```

Use this to clear RTC alarm flags.

**SetWakeupEnabled(status)**
Status is equal to `True` or `False`.
Returns:
```python
{'error': 'NO_ERROR'}
```

When the alarm goes off it will check to see if wakeup enabled is True.

**GetTime()**
Returns current time and date set in RTC:
```python
{'data': {'second': 34, 'minute': 49, 'hour': 19, 'weekday': 2, 'day': 7, 'month': 10, 'year': 2019, 'subsecond': 0, 'daylightsaving': 'NONE', 'storeoperation': False}, 'error': 'NO_ERROR'}
```

**SetTime(dateTime)**
Sets time and date as per values set in arguments.
Arguments:
```python
{'second':second, 'minute':minute, 'hour':hour, 'weekday':(weekday()+1) % 7 + 1, 'day':day, 'month':month, 'year':year, 'subsecond':microsecond//1000000}
```

**GetAlarm()**
Get values for current alarm that has been set.
Returns:
```python
{'data': {'second': 0, 'minute': 0, 'hour': 'EVERY_HOUR', 'day': 'EVERY_DAY'}, 'error': 'NO_ERROR'}
```

**SetAlarm(alarm)**
Set the alarm based on arguments set.
Arguments:
```python
{'second': 0, 'minute': 0, 'hour': 'EVERY_HOUR', 'day': 'EVERY_DAY'}
```

#### PiJuice Power Functions

**SetPowerOff(delay)**

Removes power from the PiJuice to the GPIO pins i.e Raspberry Pi. Delay is used here to give the Raspberry Pi enough time to do a safe shutdown before power has been removed. Delay value is should be written in seconds.

**GetPowerOff()**


**SetWakeUpOnCharge(percentage)**
Used to wakeup the Raspberry Pi when the battery charge level reaches a certain percentage as set in the passed argument. Value should set as 0 - 100 percentile.

**GetWakeUpOnCharge()**
Used to get the current state of the Wakeup on charge function and return its value set.
Returns:
```python
{'data': 127, 'error': 'NO_ERROR'}
```

**SetWatchdog(minutes)**
The watchdog timer has a configurable time-out. It defines the time after which it will power cycle if it does not receive a heartbeat signal. The time step is in minutes so the minimum timeout period is one minute and the maximum is 65535 minutes. The number can be any whole number between one and 65535. If you set the time to zero the watchdog timer will be disabled.

**GetWatchdog()**
Gets current watchdog status as well as returning the current value set in minutes.
Returns:
```python
{'data': 2, 'error': 'NO_ERROR'}
```

**SetSystemPowerSwitch(state)**
Sets state of System switch.
Arguments:
```python
' state':state
```
where state is desired current limit in milliampere (two options available, 500 and 2100), or switch off if 0.
Example:
```python
print(pijuice.power.SetSystemPowerSwitch(500))
```

**GetSystemPowerSwitch()**
Gets current state of System switch.
Returns:
```python
'data': state
```
where state is current limit in milliampere or 0 if switch is off.
Example:
```python
print(pijuice.power.GetSystemPowerSwitch())
```
Returns:
```python
{'data': 500, 'error': 'NO_ERROR'}
```

#### PiJuice Config

**SetChargingConfig(config, True)**
Used to enable or disable charging of the PiJuice battery.
Config:
```python
{'charging_enabled':True}
Charing enabled value can be True (Enabled) or False (Disabled).
```

**GetChargingConfig()**
Used to get current status of charging enabled for the PiJuice HAT.
Returns:
```python
{'data': {'charging_enabled': True}, 'non_volatile': True, 'error': 'NO_ERROR'}
```

**SetBatteryProfile(profile)**
Allows you to set the battery profile from a list of already pre-configured profiles or set as DEFAULT or CUSTOM.
Available Profiles:
```python
'BP6X_1400', 'BP7X_1820', 'SNN5843_2300', 'PJLIPO_12000', 'PJLIPO_5000', 'PJBP7X_1600', 'PJSNN5843_1300', 'PJZERO_1200', 'PJZERO_1000', 'PJLIPO_600', 'PJLIPO_500'
```

Setting battery profile to DEFAULT will automatically configure the profile based on the DIP switch set on the PiJuice HAT. The default DIP switch setting is set to use the BP7X battery profile.

**GetBatteryProfileStatus()**
Gets current battery profile status.
Returns:
```python
{'data': {'validity': 'VALID', 'source': 'DIP_SWITCH', 'origin': 'PREDEFINED', 'profile': 'BP7X_1820'}, 'error': 'NO_ERROR'}
```
Battery Profile Sources:
* HOST
* DIP_SWITCH
* RESISTOR

**GetBatteryProfile()**
This command is used to get all set values of the current selected battery profile.
Returns:
```python
{'data': {'capacity': 1820, 'chargeCurrent': 925, 'terminationCurrent': 50, 'regulationVoltage': 4180, 'cutoffVoltage': 3000, 'tempCold': 1, 'tempCool': 10, 'tempWarm': 45, 'tempHot': 59, 'ntcB': 3380, 'ntcResistance': 10000}, 'error': 'NO_ERROR'}
```

**SetCustomBatteryProfile(profile)**
Arguments passed:
```python
{'capacity': 1820, 'chargeCurrent': 925, 'terminationCurrent': 50, 'regulationVoltage': 4180, 'cutoffVoltage': 3000, 'tempCold': 1, 'tempCool': 10, 'tempWarm': 45, 'tempHot': 59, 'ntcB': 3380, 'ntcResistance': 10000}
```

**GetBatteryTempSenseConfig()**
This function will return one of the following settings:
* NOT_USED - No temperature sensor will be used
* NTC - Use batteries built-in NTC as per battery NTC terminal
* ON_BOARD - Use temperature sensor on MCU
* AUTO_DETECT - Let the PiJuice software determine which method to use

The settings above determine how the battery temperature is taken. It is highly recommended that any battery used with the PiJuice HAT or PiJuice Zero has a built-in NTC or battery temperature should be taken by on-board MCU.

**SetBatteryTempSenseConfig(config)**
Arguments passed:
* NOT_USED - No temperature sensor will be used
* NTC - Use batteries built-in NTC as per battery NTC terminal
* ON_BOARD - Use temperature sensor on MCU
* AUTO_DETECT - Let the PiJuice software determine which method to use

## Command Line Utility

There is a command line utility `pijuice_util.py` which you can find at [Software/Source/Utilities](https://github.com/PiSupply/PiJuice/tree/master/Software/Source/Utilities). The intent of the script is to get stats from the command line as well as to perform other basic functions such as dumping the PiJuice settings to a file, then loading those same settings into a new PiJuice. The functionality is as folllows:

* `--dump > dumpfile.js` to dump the settings to a file.
* `--load < dumpfile.js` to load the settings from a file.
* `--enable-wakeup` to enable the wakeup flag.
* `--get-time` to print the RTC time.
* `--get-alarm` to print the RTC alarm.
* `--get-status` to print the pijiuce status.
* `--get-config` to print the pijiuce config.
* `--get-battery` to print the pijiuce battery status.
* `--get-input` to print the pijiuce input status.

So, for example to use this you would navigate to the `pijuice_util.py` file location and then run the following on the command line:

`pijuice_util.py --dump > dumpfile.js`
