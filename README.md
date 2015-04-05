# x-bus-expander
Emulated Model Train digital interface for getting upto 64-inputs and 64-outputs.

Setup the interface
======
* Solder everything.
* Connect up to four boards in a chain.
* Modify dip switches so every board have a unique combination.
* Connect DC-Power to VIN, remember polarity.
* Connect USB-cable to PC and install CH340G driver.

How to use the interface
======
If you are running Freiwald TrainController (www.freiwald.com)
 choose "Lenz Digital Plus / LI100(F)" and correct com-port.


Hardware modification to Arduino nano V3:
======
UART signal CTS is attached to D9 (easiest done to clones with CH340G USB chip).
A jumper with 120R resistor to disable auto-reset feature is also added.


Hardware
======
The hardware is adapted to fit a single 10x5cm board.
Suggested manufacture is iteadstudio.com or elecrow.com
The PCB must be separated into two parts after manufacture.
Use a hacksaw for example if PCB tickness is 1.6mm. 
If running at 1.2mm the PCB can acually be cut by a ordinary scissors (the scissors will loose sharpness).

