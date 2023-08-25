#! /bin/bash

clear

./build.sh

qemu-system-x86_64 -cdrom build/output.iso