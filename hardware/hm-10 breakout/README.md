HM-10 Breakout Board
====================

<img src="/images/top.png" width="200px" title="Top" />
<img src="/images/bottom.png" width="200px" title="Bottom" />

This is a slight modification of [Ladvien's board](https://github.com/Ladvien/HM-10). This version has more accurate pin notations and removes routes to unused pins on the HM-10, thereby shrinking the overall footprint of the board. 
The HM-10's on board TI CC2541 is fully programmable with CC-Debugger or SmartRF04EB. See this repo's [wiki](https://github.com/nickswalker/HM-10-breakout-board/wiki) for more resources.


## Bill of Materials

If you are planning on ordering the PCB through OSHPark, keep in mind that you will receive three copies. You may want to go ahead and order three times as many components. Digi-Key numbers are provided as a courtesy; check eBay or any other preferred supplier for better pricing. As long as the component matches the packaging a specifications, it should be fine.

At a bare minimum, the PCB and the HM-10 module are required. If you want to use UART but only have the capability of providing 5v, you will want to populate the level conversion components. However, this is /grossly/ inefficient and defeats the purpose of using a low power connection technology. Bridge the solder jumper to bypass the regulator circuit. You may consider simply regulating off board for development, using a cheap level converter like [this one](http://www.ebay.com/itm/1PC-New-5V-3V-IIC-UART-Level-Converter-Module-Adapter-4-Way-For-Arduino-/251384699933?pt=LH_DefaultDomain_0&hash=item3a87b21c1d).

Part (Digi-Key #) | Quantity | Optional (Required For)
:---------------- | :-------:| :----------------------
[Breakout PCB](https://oshpark.com/shared_projects/A6hoH6Fi)  | 1 | No
[HM-10 Module](http://www.fasttech.com/products/0/10004051/1292002-ti-cc2540-bluetooth-40-ble-2540-transparent-serial) | 1 | No
[0805 20kΩ Resistor](http://www.digikey.com/product-detail/en/RMCF0805JT20K0/RMCF0805JT20K0CT-ND/1942580) (RMCF0805JT20K0CT-ND) | 1 | Yes (UART level conversion)
[0805 10kΩ Resistor](http://www.digikey.com/product-detail/en/RMCF0805FT10K0/RMCF0805FT10K0DKR-ND/1943341) (RMCF0805FT10K0CT-ND) | 3 | Yes (UART level conversion)
[0603 LED](http://www.digikey.com/product-detail/en/LTST-C191KRKT/160-1447-1-ND/386836) (160-1447-1-ND) | 1 | Yes (LEDs)
[0603 LED (>3v drop)](http://www.digikey.com/product-detail/en/APT1608QBC%2FD/754-1434-1-ND/2163792) (754-1434-1-ND) | 2 | Yes (LEDs)
[0805 470Ω Resistor](http://www.digikey.com/product-detail/en/RMCF0805JG470R/RMCF0805JG470RCT-ND/4425246) (RMCF0805JG470RCT-ND) | 3 | Yes (LEDs)
[SOT-23 MCP1700 Voltage Regulator](http://www.digikey.com/product-detail/en/MCP1700T-3302E%2FTT/MCP1700T3302ETTCT-ND/652677) (MCP1700T3302ETTCT-ND) | 1 | Yes (voltage regulator)
[0805 1uF Capacitor](http://www.digikey.com/product-detail/en/JMK105BJ105KV-F/587-1231-1-ND/931008) (587-1231-1-ND) | 1 | Yes (Voltage regulator)

The silkscreen doesn't include values, so be sure to have it open in front of you while you assemble.
