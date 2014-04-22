HM-10 Breakout Board
====================

<img src="top.png" width="200px" title="Top" />
<img src="bottom.png" width="200px" title="Bottom" />

This is a slight modification of [Ladvien's board](https://github.com/Ladvien/HM-10). It provides breadboard-pitch access to the HM-10 BLE module. 
The HM-10's on board TI CC2540 is fully programmable with CC-Debugger or SmartRF04EB.

### Who is This Board For?

Two groups of people: Those wanting to make the HM-10 breadboard-friendly and those wanting a cheaper, more flexible alternative to TI's CC2541 dev kits. All pins on the HM-10 are broken out, even those listed in the datasheet as having no connection.

There is no space for a USB connector, however the D+ and D- minus pins are broken out and one can be added in the breadboard with little difficulty.

##Bill of Materials

If you are planning on ordering the PCB through OSHPark, you should note that you will receive three copies of the board. You may want to go ahead and order three of each component you intend to use. Digi-Key numbers are provided as a courtesy; check ebay or any other preferred supplier for better pricing. As long as the component matches the packaging a specifications, it should be fine.

At a bare minimum, the PCB and the HM-10 module are required. You'll probably want to use the UART connection in your project, so you'll want to populate those components too.

Part (Digi-Key #) | Quantity | Optional (Required For)
:---------------- | :-------:| :----------------------
[Breakout PCB](https://oshpark.com/shared_projects/mgt9vH1j)  | 1 | No
[HM-10 Module](http://www.fasttech.com/products/0/10004051/1292002-ti-cc2540-bluetooth-40-ble-2540-transparent-serial) | 1 | No
[0805 20kΩ Resistor](http://www.digikey.com/product-detail/en/RMCF0805JT20K0/RMCF0805JT20K0CT-ND/1942580) (RMCF0805JT20K0CT-ND) | 1 | Yes (UART level conversion)
[0805 10kΩ Resistor](http://www.digikey.com/product-detail/en/RMCF0805FT10K0/RMCF0805FT10K0DKR-ND/1943341) (RMCF0805FT10K0CT-ND) | 3 | Yes (UART level conversion)
[CC Debugger Header](http://www.digikey.com/product-detail/en/20021121-00010C4LF/609-3695-1-ND/2209147) (609-3695-1-ND)  | 1 | Yes (CC Debug capability)
[0603 LED](http://www.digikey.com/product-detail/en/LTST-C191KRKT/160-1447-1-ND/386836) (160-1447-1-ND) | 1 | Yes (LEDs)
[0603 LED (>3v drop)](http://www.digikey.com/product-detail/en/APT1608QBC%2FD/754-1434-1-ND/2163792) (754-1434-1-ND) | 2 | Yes (LEDs)
[0805 470Ω Resistor](http://www.digikey.com/product-detail/en/RMCF0805JG470R/RMCF0805JG470RCT-ND/4425246) (RMCF0805JG470RCT-ND) | 3 | Yes (LEDs)
[SOT-23 MCP1700 Voltage Regulator](http://www.digikey.com/product-detail/en/MCP1700T-3302E%2FTT/MCP1700T3302ETTCT-ND/652677) (MCP1700T3302ETTCT-ND) | 1 | Yes (voltage regulator)
[0805 1uF Capacitor]() () | 1 | Yes (Voltage regulator)

Reference the schematic if you are uncertain where a specific component is required.

##Links

[This repo's wiki](https://github.com/nickswalker/HM-10-breakout-board/wiki) is a good resource for information on using the HM-10 as a BLE development platform.

Assembly: http://www.instructables.com/id/How-to-Create-an-Arduino-Compatible-Bluetooth-40-M/

On-going HM-10 research: http://letsmakerobots.com/node/38009

###Other HM-10/CC254x Based Projects
####Coin BLE Dev Kit
Arduino Pro Mini with a bare CC2540 to provide serial over BLE. Their [wiki](https://github.com/CoinApps/arduino-ble-dev-kit/wiki/_pages) is full of useful information.
####OS CC2450
Project to create open software platform on-top of CC's proprietary stuff. The dev blog includes some useful photos and even some [schematics](http://blog.p-dev.net/?p=93).
####JY MCU
If all you want is a UART serial connection from a HM-10, [buy this baseboard](http://www.fasttech.com/products/1129102).

###Technical Information

HM-10 manufacturer's site: http://www.jnhuamao.cn/

HM-10 datasheet:  http://www.jnhuamao.cn/bluetooth40_en.rar

CC2540 website: http://www.ti.com/product/cc2540&DCMP=LowPowerRFICs+Other&HQS=Other+OT+cc2540

CC2540 datasheet: http://www.ti.com/lit/ds/symlink/cc2540.pdf
