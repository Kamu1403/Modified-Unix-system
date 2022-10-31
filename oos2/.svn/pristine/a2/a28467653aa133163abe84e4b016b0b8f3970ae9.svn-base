include Makefile.inc

.PHONY : all cpobj build deploy run $(DIRS)

PROGRAMS =	cat.exe	\
			cp.exe	\
			ls.exe	\
			mkdir.exe	\
			rm.exe

all: cpobj build deploy 

$(DIRS) : 
	$(MAKE) --directory=$@
cpobj   :
	copy $(OBJFILES)\sector2.bin $(TARGET)\sector2.bin
	copy $(OBJFILES)\support.o $(TARGET)\support.o
	copy $(OBJFILES)\chardevice.o $(TARGET)\chardevice.o
	copy $(OBJFILES)\crt.o $(TARGET)\crt.o
	copy $(OBJFILES)\tty.o $(TARGET)\tty.o
	copy $(OBJFILES)\keyboard.o $(TARGET)\keyboard.o
	copy $(OBJFILES)\kernel.o $(TARGET)\kernel.o
	copy $(OBJFILES)\machine.o $(TARGET)\machine.o
	copy $(OBJFILES)\main.o $(TARGET)\main.o
	
build	: $(DIRS)	
	$(LD) $(LDFLAGS) $(OBJS) -o $(TARGET)\kernel.exe	
	$(OBJCOPY) -O binary $(TARGET)\kernel.exe $(TARGET)\kernel.bin
	$(NM) $(TARGET)\kernel.exe > $(TARGET)\kernel.sym
	$(OBJDUMP) -d $(TARGET)\kernel.exe  > $(TARGET)\kernel.asm

deploy	:

	copy $(TARGET)\boot.bin $(IMGTARGET)\boot.bin
	copy $(TARGET)\kernel.bin $(IMGTARGET)\kernel.bin
	copy $(TARGET)\kernel.sym  $(IMGTARGET)\kernel.sym
	copy $(TARGET)\kernel.asm  $(IMGTARGET)\kernel.asm

	cd $(IMGTARGET) && partcopy boot.bin 0 200 c.img 0
	cd $(IMGTARGET) && partcopy kernel.bin 0 13000 c.img 200

	copy $(TARGET)\boot.bin $(MakeImageDebug)\boot.bin
	copy $(TARGET)\kernel.bin $(MakeImageDebug)\kernel.bin
	copy $(IMGTARGET)\c.img $(MakeImageDebug)\c.img

	cd $(MakeImageDebug) && build.exe c.img boot.bin kernel.bin programs
	copy $(MakeImageDebug)\$(IMG) $(UNIXV6PPTARGET)\$(IMG)

clean	:	
	del     $(TARGET)\*.o
	del     $(TARGET)\*.exe
	del     $(TARGET)\*.bin
	del     $(TARGET)\*.sym
	del     $(TARGET)\*.asm
	
	del     $(IMGTARGET)\*.bin
	del     $(IMGTARGET)\*.sym
	del     $(IMGTARGET)\*.asm
	
	del     $(UNIXV6PPTARGET)\c.img
