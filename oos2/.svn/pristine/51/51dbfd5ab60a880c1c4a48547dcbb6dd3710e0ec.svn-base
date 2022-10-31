include ..\Makefile.inc

TARGET = ..\..\targets\objs

all		:	$(TARGET)\boot.bin $(TARGET)\sector2.bin $(TARGET)\support.o
		
$(TARGET)\boot.bin	:	boot.s
	$(ASM) -f bin boot.s -o $(TARGET)\boot.bin
