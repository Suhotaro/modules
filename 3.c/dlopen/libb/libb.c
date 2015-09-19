#include <stdio.h>
#include "libb.h"
#include "dlfcn.h"

int libb_global = 3;

void libb_2(void)
{
    int libb_local = 4;

    printf("libb_2: g=%d l=%d\n", libb_global, libb_local);

    my_dlopen();
}

void __attribute__ ((visibility("hidden"))) my_dlopen(void)
{
    void *handle = NULL;
    void (*foo_ptr)(void) = NULL;

    handle = dlopen("libc/libc.so", RTLD_LAZY);
    if (!handle)
        ERROR("dlopen filed\n");

    foo_ptr = (void (*)(void)) dlsym(handle, "libc_3");
    if (!foo_ptr)
        ERROR("dlsym failed\n");

    (*foo_ptr)();    
}
