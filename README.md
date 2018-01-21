## Port of RIOT-OS to raspberry pi 3

## Compile: 
In the file  *cpu/arm7_common/Makefile.include* you should change the line:
`export TARGET_ARCH ?= /home/dafna/gcc-arm-none-eabi-6-2017-q2-update/bin/arm-none-eabi`
To your cross-compiler path

Then `cd examples/hello-world/` and `make`
The output file *bin/pi3/hello-world.hex* is the *kernel7.img* in the /boot of rpi.
The example shows uart usage from two threads, with baudrate 115200.

The code is mostly based on the [freertos port for raspberry pi](https://github.com/Forty-Tw0/RaspberryPi-FreeRTOS), and also [bare metal examples](https://github.com/dwelch67/raspberrypi).


