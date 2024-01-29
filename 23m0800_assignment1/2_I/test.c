#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "query_ioctl.h"

int main(void) {

    // Allocate a byte-size memory on the heap
    int *value = (int*)malloc(sizeof(int));

    if (value == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;  // Exit with an error code
    }

    // Assign the value "6" to the memory
    *value = 6;

    query_arg_t arg;

    arg.v_addr = (unsigned long)value;
    arg.phys_addr = 0;
    arg.value = *value;

    printf("\n\nVirtual Address: 0x%lx\n", arg.v_addr);
    printf("Value prior call: %d\n", *value);
  
    // opening fd
    int fd = open("/dev/query", O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device");
        return EXIT_FAILURE;
    }

    // Get the physical address
    if(ioctl(fd, VIR_TO_PHYS_TRANS, &arg) < 0) {
        perror("VIR_TO_PHYS_TRANS failed");
        close(fd);
        return EXIT_FAILURE;
    }
    printf("Virtual Address: 0x%lx -> Physical Address: 0x%lx\n", arg.v_addr, arg.phys_addr);

    // change the value to 5
    arg.value = 5;
    if(ioctl(fd, PUT_VALUE_IN_ADDR, &arg) < 0) {
        perror("PUT_VALUE_IN_ADDR failed");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("Value after call: %d\n", *value);
    printf("Virtual Address: 0x%lx -> Physical Address: 0x%lx\n\n\n", arg.v_addr, arg.phys_addr);
    return 1;
}