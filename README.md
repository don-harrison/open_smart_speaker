the compiler knows the rules for using a stack on Cortex-M7

The linker uses a linker script (e.g., imxrt1062.ld) that defines memory regions (Flash, RAM, peripherals, etc.) and places code/data into them.

the linker script chooses where RAM regions live and where the stack top symbol is

the startup code loads that stack address into the CPU’s sp register before main() runs.

install VS Code

Setup WSL, in wsl 
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  make \
  gcc-arm-none-eabi \
  binutils-arm-none-eabi \
  gdb-multiarch \
  git

git clone the repo

Open the repo in the vscode

Shift + Ctrl + P and type "reopen folder in WSL"

this lets you open the project in WSL so you can use wsl to build and flash the project to the teensy 4.0

To Create .cc file from a tflite model:
xxd -i hello_world_float.tflite > m
odel_data.cc

Build project
rm -rf build
cmake -G "Unix Makefiles" \
  -DCMAKE_MAKE_PROGRAM=$(which make) \
  -DCMAKE_TOOLCHAIN_FILE=./toolchain-arm-none-eabi.cmake \
  -S . -B build
cmake --build build --verbose

Since the device will be connected to windows by default, the flash command will hang waiting for the device to show up in wsl

download usbipd for windows

Connect the teensy via usb
we need to connect the usb to wsl
attach the busid id windows using cmd
usbipd list
usbipd attach --wsl --busid 1-1

press bootloader button on teensy

Flash to Teensy 4.0 over usb
sudo teensy_loader_cli --mcu=TEENSY40 -w ./build/firmware.hex