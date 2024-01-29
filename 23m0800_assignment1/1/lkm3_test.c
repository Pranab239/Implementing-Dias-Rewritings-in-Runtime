/*
* Usage: Run the virtual_addr.c It will print a virtual address & pid on terminal.
* Put this virtual address & pid while loading the module into the kernel.
*
* Commands:
* gcc -o lkm3_test lkm3_test.c
* ./lkm3_test
* Open another terminal in the same folder
* make
* sudo insmod lkm3.ko pid=XXXX vaddr=0xXXXXXXXXXXXX
* sudo rmmod lkm3
* sudo dmesg | tail -4
*/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

enum { I0 = 0x12345678 };

static volatile uint32_t i = I0;

int main(void) {
    int a[1000] = {0};
    printf("vaddr %p\n", (void *)&i);
    printf("pid %ju\n", (uintmax_t)getpid());
    while (i == I0) {
        sleep(1);
    }
    printf("i %jx\n", (uintmax_t)i);
    return EXIT_SUCCESS;
}