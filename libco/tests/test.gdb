# script.gdb
#init GDB use it in file:/tests
set environment LD_LIBRARY_PATH=..
file libco-test-32
break main
p co_main
run