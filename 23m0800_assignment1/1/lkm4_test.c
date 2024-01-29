/*
* Usage: First compile and run the test.c.
* After first memory allocation load the module in the kernel with shown pid
* And again do the same for the second memory allocation
* gcc -o lkm4_test lkm4_test.c
* ./lkm4_test
*
* Another window 
* sudo insmod lkm4.ko pid=XXXX (from the lkm4_test)
* sudo rmmod lkm4.ko
* After the second initialization processing do the above two steps again
* sudo dmesg | tail -8 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {

    // Allocate memory for an array of integers
    printf("First memory allocation is processing!\n");
    printf("My process id is : %d\n", getpid());

    int *dynamicArray = (int*)malloc(10000000 * sizeof(int));
    if (dynamicArray == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for(int i = 0 ; i < 10000000 ; i++) {
        *(dynamicArray+i) = 1;
    }

    sleep(20);
    printf("First Memory allocation is done. Freeing the memory.\n");

    // Clear the memory
    free(dynamicArray);


    // Allocate memory for an array of integers
    printf("Second memory allocation is processing!\n");

    dynamicArray = (void*)malloc(10000);
    if (dynamicArray == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    for(int i = 0 ; i < 10000 ; i++) {
        *(dynamicArray+i) = 1;
    }
    
    sleep(20);
    printf("Second memory allocation is done. Freeing the memory!\n");;

    printf("Done.\n");
    
    return 0;
}
