the compiler knows the rules for using a stack on Cortex-M7

The linker uses a linker script (e.g., imxrt1062.ld) that defines memory regions (Flash, RAM, peripherals, etc.) and places code/data into them.

the linker script chooses where RAM regions live and where the stack top symbol is

the startup code loads that stack address into the CPU’s sp register before main() runs.

sudo teensy_loader_cli --mcu=TEENSY40 -w ./build/firmware.hex

press bootloader button on teensy
then attach the busid in windows
usbipd list
usbipd attach --wsl --busid 1-1