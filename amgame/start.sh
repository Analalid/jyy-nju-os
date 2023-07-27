#!/bin/bash
clear
make
qemu-system-x86_64 -drive format=raw,file=build/amgame-x86_64-qemu