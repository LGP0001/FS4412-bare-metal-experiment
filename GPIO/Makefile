TARGET = led_arm
CROSS_COMPILE = arm-none-linux-gnueabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

all:
	$(CC) -c $(TARGET).s -o $(TARGET).o
	$(LD) $(TARGET).o -Ttext 0x40008000 -o $(TARGET).elf
	$(OBJCOPY) -O binary -S $(TARGET).elf $(TARGET).bin

clean:
	rm $(TARGET).o $(TARGET).elf $(TARGET).bin