# Modified-Unix-system

## Introduction
 Modified process switching logic and added an ext2-like file system for Unix V6 system.

## Our work
- In [project 1](./oos1), modified Unix v6 operating system and the process of virtual and 
  real address mapping page, and speeded up the process switching by saving the page table 
  of multiple processes 
- In [project 2](./oos2), Modified Unix disk format to support variable length file names 
  according to the Linux ext2 disk format, achieving the compatibility of the original disk 
  format
- For more information, refer to [Project1 report](./resources/Project%20report1.pdf) and
  [Project2 report](./resources/Project%20report2.pdf).

## Dependencies
- Installment-free versions of dependencies are available in the project.
- [Bochs-2.6](http://bochs.sourceforge.net/): An open source emulator for X86 hardware. 
  It emulates x86 hardware on any platform where bochs is compiled and run, including I/O 
  devices, memory, and BIOS. The installation-free version is provided in the tool package
  [here](./Bochs-2.6).
- [MinGW](http://www.mingw.org/): Minimalist GNU on Windows. MinGW is a set of file and 
  port libraries that allow console-mode programs to use Microsoft's standard C runtime 
  library (MSVCRT.DLL). The installation free version is provided [here](./MinGW).
- NASM: nasm: An 80x86 assembler designed for portability and modularity. It supports a
  wide range of object file formats. The installation free version is provided [here](./NASM).

## Install
1. Configuration of the environment
- In order to run UNIX V6, you need to set some environment variables. To start running, 
  locate the ```\oos\tools\oosvars_mingw.bat``` file corresponding to your running project.
  Modify the Settings of the files bochs, MinGW, NSAM to change to the correct path on 
  your computer.
```shell
@set oos_path=C:\unix v6++\oos            # You need to modify
@set mingw_path=C:\unix v6++\MinGW\bin    # You need to modify
@set nasm_path=C:\unix v6++\NASM          # You need to modify
@set bochs_path=C:\unix v6++\Bochs-2.6    # You need to modify
@set BXSHARE=%bochs_path%
@set partcopy_path=%oos_path%\tools\partcopy

@set path=%partcopy_path%;%bochs_path%;%nasm_path%;%mingw_path%;%oos_path%;%path%

@cls
@echo Setting develop and build environment for UnixV6++.
```

2. Run unix as a sub operate system
- The basic CMake command is supported, and you can run ```clean``` to clear the 
  compilation cache, run ```all``` to recompile, and run ```run``` to start running 
  Unix system.
<img src=".\resources\1.PNG" width="400"/>
<img src=".\resources\2.PNG" width="400"/>
<img src=".\resources\3.PNG" width="400"/>

## Running examples
### Project1
1. Initializes successfully 
- <img src=".\resources\1_1.PNG" width="400"/>
2. Run ```stack``` command
- <img src=".\resources\1_2.PNG" width="400"/>
- <img src=".\resources\1_3.PNG" width="400"/>

### Project2
1. Add folders with long names
- <img src=".\resources\2_1.PNG" width="400"/>
2. Add files with long names
- <img src=".\resources\2_2.PNG" width="400"/>
3. Adding Files with Short Names (folders)
- <img src=".\resources\2_3.PNG" width="400"/>
4. Go to a folder with a long name and add subfiles
- <img src=".\resources\2_4.PNG" width="400"/>
5. Recursively delete folders with long names
- <img src=".\resources\2_5.PNG" width="400"/>
6. Delete files with long names
- <img src=".\resources\2_6.PNG" width="400"/>
7. In the folder with the short name (fill the folder with the original long name)
- As can be seen from ls, the file name directory entry of the new short name is filled in the place of the original directory entry (4*32 in total, 3*32 is used).
- <img src=".\resources\2_7.PNG" width="400"/>
8. After re-entry, save the disk
- You can see that the original created directory entries and files are there.
- <img src=".\resources\2_8.PNG" width="400"/>

## Reference
[1] Tian Yu. Design and implementation of a 64-bit operating system. Beijing: 
    Posts and Telecommunications Press, 2018.05. 

[2] Wang Honghui. Embedded system Linux kernel development practical guide ARM platform. 
    Beijing: Publishing House of Electronics Industry, 2009.03. 

[3] Harver M. Deitel, Paul J. Deitel, David R. Choffnes. Operating system version 3. 
    Beijing: Tsinghua University Press, 2007.08.