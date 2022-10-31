一、首先下载并安装安装nasm-2.07、Bochs-2.4.2和Mingw-5.1.6

二、下载oos，并解压到D盘，UNIX V6++根目录为D:\oos，如果不放在D:\oos，运行调试器时会出错

二、在第一次使用编译环境前需要修改oos\tools\oosvar_mingw.bat中如下环境变量
    oos_path=<os工作目录>(一定要设为D:\oos，否则调试器运行时会出错)
    mingw_path=<mingw工作目录>
    nasm_path=<nasm工作目录>
    bochs_path=<bochs工作目录>
    BXSHARE=%bochs_path%
    partcopy_path=%oos_path%\tools\partcopy

    look like:
    @set oos_path=D:\oos
    @set mingw_path=C:\Program Files\MinGW\bin
    @set nasm_path=C:\Program Files\NASM
    @set bochs_path=C:\Program Files\Bochs-2.4.2
    @set BXSHARE=%bochs_path%
    @set partcopy_path=%oos_path%\tools\partcopy

三、运行oos\tools\OOS Command Prompt快捷方式
    clean命令清除上次编译结果
    all命令执行编译和部署
    run命令在bochs虚拟机上启动操作系统      