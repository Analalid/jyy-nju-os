#!/bin/bash

make
qemu-system-x86_64 -drive format=raw,file=build/amgame-x86_64-qemu