#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
#include <linux/ioctl.h>
#include <linux/module.h>
 
// typedef struct
// {
//     int value;
// } query_arg_t;
 
#define CHANGE_PARENT _IOR('q', 1, int*)
#endif