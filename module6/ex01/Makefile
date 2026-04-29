CC      := avr-gcc
OBJCOPY := avr-objcopy
AVRDUDE := avrdude

MCU     := atmega328p
F_CPU   := 16000000UL
PORT    := /dev/ttyUSB0

CFLAGS  := -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Iinclude

SRC		:= src/main.c src/uart.c src/i2c.c src/utils.c
OBJ     := $(SRC:.c=.o)

TARGET  := main

all: hex flash

%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

hex: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET).bin
	$(OBJCOPY) -O ihex -R .eeprom $(TARGET).bin $(TARGET).hex

flash: hex
	$(AVRDUDE) -c arduino -p $(MCU) -P $(PORT) -b 115200 -U flash:w:$(TARGET).hex

screen:
	screen $(PORT) 115200

clean:
	rm -f $(OBJ) $(TARGET).bin $(TARGET).hex