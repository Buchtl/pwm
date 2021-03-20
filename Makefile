
compile: main.c
#	avr-gcc -Wall -g -O0 -mmcu=avr5 -o main.bin main.c
	avr-gcc -Wall -g -O0 -mmcu=atmega328p -o main.bin main.c

hexhex: compile
	avr-objcopy -j .text -j .data -O ihex main.bin main.hex

upload: hexhex
	avrdude -p m328p -P /dev/ttyACM0 -c arduino -U flash:w:main.hex:i -F =P usb

clean:
	rm -f main.bin main.hex
