# x-bus-expander
Emulated Model Train digital interface for getting upto 64-inputs and 64-outputs.

Setup the interface
======
* Solder everything.
* Connect up to four boards in a chain.
* Modify dip switches so every board have a unique combination.
* Connect USB-cable to PC and install CH340G driver.

How to connect Inputs
======
Switches can be connected to IN1 or IN2, pull-up is located on-board. 
The input signals must be electrical isolated from the rest of the system to not dammage anything.
If other devices that switches will be connected a optocoupler is suggested to electrical isolate the input.
C1-C8, C10-C17 is to avoid interface from inputs.

How to connect Outputs
======
Led:s / lamps can be connected to OUT1 and OUT2. The output voltage is equal to the VIN voltage.
With 2N7002 the maximum load current is 115mA and suggested voltage shall be 24V DC or less on VIN.
Remember to use correct polarity on VIN, marked with + and - on PCB.

How to use the interface
======
If you are running Freiwald TrainController (www.freiwald.com)
 choose "Lenz Digital Plus / LI100(F)" and correct com-port.
 The outputs will be named with Address 1-64
 The inputs will be named with Address 1-8 and Input 1-8

Hardware modification to Arduino nano:
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

