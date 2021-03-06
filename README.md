# Developing on TI's TM4C123 
Bare CMake project to develop on TI's TM4c123 board. Repo's main objective was to understand the
toolchain process involved to begin developing on it.

## Dependencies
Will be using [lm4tools](https://github.com/utzig/lm4tools) to flash the device.

```
make cmake arm-none-eabi-gcc arm-none-eabi-gdb arm-none-eabi-newlib gcc-c++ libusb unzip openocd lm4flash
```

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
