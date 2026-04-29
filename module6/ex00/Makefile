CC        := avr-gcc

PORT := /dev/ttyUSB0

all: hex flash

hex:
	$(CC) -Os -D F_CPU=16000000UL -mmcu=atmega328p -o main.bin main.c
	avr-objcopy -O ihex main.bin main.hex

flash:
	avrdude -c arduino -p m328p -P $(PORT) -b 115200 -U flash:w:main.hex 

screen:
	screen /dev/ttyUSB0 115200

clean:
		rm -rf main.bin main.hex
