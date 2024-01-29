#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
#include <linux/ioctl.h>
#include <linux/module.h>
 
typedef struct
{
    unsigned long v_addr;
    unsigned long phys_addr;
    int value;
} query_arg_t;
 
#define VIR_TO_PHYS_TRANS _IOR('q', 1, query_arg_t *)
#define PUT_VALUE_IN_ADDR _IOR('q', 2, query_arg_t *)
#endif