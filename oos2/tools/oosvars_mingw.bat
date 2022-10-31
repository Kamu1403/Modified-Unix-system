@set oos_path=C:\unix v6++\oos
@set mingw_path=C:\unix v6++\MinGW\bin
@set nasm_path=C:\unix v6++\NASM
@set bochs_path=C:\unix v6++\Bochs-2.6
@set BXSHARE=%bochs_path%
@set partcopy_path=%oos_path%\tools\partcopy

@set path=%partcopy_path%;%bochs_path%;%nasm_path%;%mingw_path%;%oos_path%;%path%

@cls
@echo Setting develop and build environment for UnixV6++.