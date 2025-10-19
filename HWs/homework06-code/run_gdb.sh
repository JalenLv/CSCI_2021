#! /bin/bash

qemu-x86_64 -g 2021 $1 &
gdb-multiarch $1 -ex 'target remote localhost:2021'
