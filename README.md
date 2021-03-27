# Developing on TI's TM4C123 
Took inspiration from [yusef's](https://www.hackster.io/tcss/upload-code-to-ti-tm4c123-using-linux-cmake-and-lm4tools-c33cec) post on hackster.io, which did take inspiration from Vitor Mato's extensive [repo](https://github.com/vmatos/tiva-c-projects).

## Dependencies
Will be using [lm4tools](https://github.com/utzig/lm4tools) to flash the device.
`make cmake arm-none-eabi-gcc arm-none-eabi-gdb arm-none-eabi-newlib gcc-c++ libusb unzip openocd lm4flash`

### Install
Dependencies are based on a Arch Linux system, should work on any Linux distro as long as
the dependencies are installed accordingly.
```
sudo pacman -S make cmake arm-none-eabi-gcc arm-none-eabi-gdb arm-none-eabi-newlib gcc-c++ libusb unzip openocd
mkdir ~/tm4c123; cd ~/tm4c123
git clone https://github.com/utzig/lm4tools.git
cd lm4tools/lm4flash
make
cd ../
cd ../
git clone git@github.com:duclos-cavalcanti/TM4C123GXL-Cmake.git
```
As an example this will be installed at the home directory, but could be anywhere else for
that matter. Also don't forget to add the lm4flash binary to your PATH variable.

### Udev Rules
It is needed to add the user to the users group as this removes the need to use sudo both
for debugging and flashing the board.
```
echo 'ATTRS{idVendor}=="1cbe", ATTRS{idProduct}=="00fd", GROUP="users", MODE="0660"' | \
sudo tee /etc/udev/rules.d/99-stellaris-launchpad.rules
sudo gpasswd -a <user> users
```

## Downloading Tiva Software
Go [here](https://www.ti.com/tool/SW-TM4C) and download the Full Release .exe of the TM4C
Software Kit.

```
cd ~/tm4c123
mkdir tivaware; cd tivaware
mv ~/Downloads/SW-TM4C-2.1.3.178.exe .
unzip SW-TM4C-2.1.3.178.exe
make
```
Versions may vary.

## Usage
```
cd ~/tm4c123/TM4C123GXL-Cmake
mkdir build
cd build
cmake ..
make # Compiles
make flash # flashes
make debug # debugs using gdb
```

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

