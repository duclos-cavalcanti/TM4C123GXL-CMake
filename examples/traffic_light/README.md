# Traffic Light
## Lab Example
This example was used to implement a simple traffic light. Pressing switch 1 (PE0)
the western traffic light will turn green while the eastern traffic light moves to yellow
and then red.

The inverse situation occurs by pressing switch 2(PE1).

Pressing both switches will cause permanent oscillations between green/yellow/red on both
traffic lights.
### I/O Configuration
#### Traffic Lights - GPIO Port B
	1. PortB5-B3 Traffic Light East
    2. PortB2-B0 Traffic Light West

#### Sensors - GPIO Port E
    1. PortE0 West Sensor
    2. PortE1 East Sensor 
    3. PortE2 Walk Sensor

#### Walk Lights - GPIO Port F
    1. PortF1 Dont Walk Light (RED) --Not implemented
    2. PortF3 Walk Light (GREEN)    --Not implemented

## LEDs
Pull-Down Resistors enable positive logic LEDs.
Ex: (Positive Logic)
```
Output Pin ---o
              |
              R (470 Ohms)
              |
             LED
              |
             Gnd

Voh - R*Id - Vled = 0

Ideal Voltage is 1.8V with Id being 2mA.
Vled is also called Vd.
Voh = 3.3V.

Vd = 3.3V - R*Id; 
Therefore R = 500 Ohms and 470 Ohms would be enough.
```

Ex: (Negative Logic)
```
             3.3V
              |
              R (10K Ohms)
              |
             LED
              |
Output Pin ---o 
```

## Switches
Board built-in switches have a negative logic. Meaning if the switch is pressed the
corresponding bit allocated to this switch will be 0. Inversely it will be 1 given a
non-pressed switch.

External switches, if built upon a breadboard, may have different logic orientations
depending on how they are placed in the circuit.

Pull-Down Resistors enable positive logic switches.
Pull-Up Resistors enable negative logic switches.

Ex: (Positive Logic)
```
            3.3V
             |
             _/ (SWITCH)
             |
Input Pin ---o 
             |
             R (10K Ohms)
             |
            Gnd

Logic 0 for the uC has to be a voltage between 0 and 0.8V.
Iil = I_input_low = 2uA.

Vr = R*Iil (When Switch is OFF)
if R == 10k Ohms then Vr = 0.02V which will be considered a logic 0.
```

Ex: (Negative Logic)
```
            3.3V
             |
             R (10K Ohms)
             | 
Input Pin ---o 
             |
             _/ (SWITCH)
             |
            Gnd
```

