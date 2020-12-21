PORT = /dev/cu.SLAB_USBtoUART
DEVICE  = atmega8
F_CPU   = 16000000	# in Hz

AVRDUDE = avrdude -c stk500v2 -P $(PORT) -p $(DEVICE) -v -b9600 -F -D -u
COMPILE = avr-gcc -Os -Wall -I. -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -D__AVR_LIBC_DEPRECATED_ENABLE__ -DDEBUG_LEVEL=1 -std=c99 -ggdb
OBJECTS = main.o


# symbolic targets:
all:	main.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

clean:
	rm -f $(OBJECTS) main.hex
	
# file targets:
main.bin:	$(OBJECTS)
	$(COMPILE) -o main.bin $(OBJECTS)

main.hex:	main.bin
	rm -f main.hex main.eep.hex
	avr-objcopy -j .text -j .data -O ihex main.bin main.hex
	./checksize main.bin 8912 960

disasm:	main.bin
	avr-objdump -d main.bin

cpp:
	$(COMPILE) -E main.c

flash: main.hex
	$(AVRDUDE) -U main.hex

#fuse:
#	$(AVRDUDE) -U lfuse:w:0x9f:m -U hfuse:w:0xc9:m
