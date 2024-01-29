Usage lkm3:
Run the lkm3_test.c It will print a virtual address & pid on terminal.
Put this virtual address & pid while loading the module into the kernel.

Commands:
gcc -o lkm3_test lkm3_test.c
./lkm3_test

Open another terminal in the same folder:
make
sudo insmod lkm3.ko pid=XXXX vaddr=0xXXXXXXXXXXXX
sudo rmmod lkm3
sudo dmesg | tail -4



Usage lkm4:
First compile and run the lkm4_test.c.
After first memory allocation load the module in the kernel with shown pid
And again do the same for the second memory allocation

Commans:
gcc -o lkm4_test lkm4_test.c
./lkm4_test

Open another window:
sudo insmod lkm4.ko pid=XXXX (from the lkm4_test)
sudo rmmod lkm4.ko
After the second initialization processing do the above two steps again
sudo dmesg | tail -8 