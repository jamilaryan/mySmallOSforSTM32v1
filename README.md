# mySmallOSforSTM32v1
Basic cooperative operating system for STM32 Discovery Board
Here are the commands to set it up onto your stm32 discovery board after clone:
- type make
- make program
- sudo minicom -D /dev/ttyACM0

make sure you have ARM gcc installed as well as OpenOCD which is required for the onboard JTAG port.

