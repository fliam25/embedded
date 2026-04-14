CC        := avr-gcc

PORT := ch340

all: hex flash

hex:
	$(CC) -D F_CPU=16000000UL -mmcu=atmega328p -nostdlib -lgcc -Os -o main.bin main.c
	avr-objcopy -O ihex main.bin main.hex

flash:
	avrdude -c arduino -p m328p -P $(PORT) -b 115200 -U flash:w:main.hex 

clean:
		rm -rf main.bin main.hex
