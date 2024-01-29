#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/ioctl.h>

// Include header or define the IOCTL call interface and devide name
#include "helper/help_dr_doom.h"

int open_driver(const char* driver_name) {

    int fd_driver = open(driver_name, O_RDWR);
    if (fd_driver == -1) {
        perror("ERROR: could not open driver");
    }

	return fd_driver;
}

void close_driver(const char* driver_name, int fd_driver) {

    int result = close(fd_driver);
    if (result == -1) {
        perror("ERROR: could not close driver");
    }
}


int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s <parent_pid>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    pid_t parent_pid = atoi(argv[1]);
    const char* driver_name = "/dev/query"; 

    printf("Soldier Pid is: %d\n", getpid());

    // open ioctl driver
    int fd_driver = open_driver(driver_name);
    if (fd_driver == -1) {
        exit(EXIT_FAILURE);
    }
    
    // call ioctl with parent pid as argument to change the parent
    if(ioctl(fd_driver, CHANGE_PARENT, &parent_pid) == -1) {
        perror("ERROR: ioctl failed");
        close_driver(driver_name, fd_driver);
        exit(EXIT_FAILURE);
    }

	
    // close ioctl driver
    close_driver(driver_name, fd_driver);

	return EXIT_SUCCESS;
}

