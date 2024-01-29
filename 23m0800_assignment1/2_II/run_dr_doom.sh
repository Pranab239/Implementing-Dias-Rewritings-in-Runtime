#!/bin/bash

## Building the applications
make

## Build your ioctl driver and load it here
cd helper
make 
sudo insmod help_dr_doom.ko
cd ..

###############################################

# Launching the control station
./control_station &
c_pid=$!
# echo "Control station PID: $c_pid"

# Launching the soldier
sudo ./soldier $c_pid &
# echo "Soldier PID: $!"

sleep 2
kill -9 $c_pid

## Remove the driver here
cd helper
sudo rmmod help_dr_doom
cd ..
