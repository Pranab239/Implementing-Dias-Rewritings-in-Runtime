#!/bin/bash

make
sudo insmod query_ioctl.ko
gcc test.c
sudo ./a.out
sudo rmmod query_ioctl
make clean