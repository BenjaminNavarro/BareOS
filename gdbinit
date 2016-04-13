target remote localhost:1234
symbol-file build/kernel-x86_64.bin
tbreak long_mode_start
continue
set arch i386:x86-64
tbreak main
continue
