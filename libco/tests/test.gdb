# script.gdb, 打开gdb后使用这个脚本好好看看执行流程，不然的话迟早你会后悔的
#init GDB use it in file:/tests
layout split
set environment LD_LIBRARY_PATH=..
file libco-test-32
break main
p co_main
run