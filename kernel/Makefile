NAME           := kernel
SRCS           := framework/main.c $(shell find -L ./src/ -name "*.c")
INC_PATH       := include/ framework/


export AM_HOME := $(PWD)/../abstract-machine
ifeq ($(ARCH),)
export ARCH    := x86_64-qemu
endif

include $(AM_HOME)/Makefile
include ../Makefile.lab


image: git
run1:
	qemu-system-i386 -smp 1
test: git
	@gcc \
			$(shell find test/ -name "*.c") \
			$(shell find test/ -name "*.h") \
			$(shell find include/ -name "*.h") \
			-Iframework -Itest -DTEST -lpthread \
			-o build/test
	@build/test
