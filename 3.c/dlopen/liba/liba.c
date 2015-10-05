#include <stdio.h>
#include <stdlib.h>
#include "liba.h"
#include "../libb/libb.h"
#include "dlfcn.h"

int liba_global = 1;

#define ERROR(f, arg...)                 \
        do                               \
        {                                \
            printf("ERROR " f );         \
            exit(1);                     \
        }                                \
        while(0)

void liba_1(void)
{
    int liba_local = 2;

    printf("liba_1: g=%d l=%d\n", liba_global, liba_local);


    my_dlopen();
}


void __attribute__ ((visibility("hidden"))) liba_2(void)
{
   printf("liba_2\n");
}


void liba_3(void)
{
   printf("liba_3\n");
}

void my_dlopen(void)
{
    void *handle = NULL;
    void (*foo_ptr)(void) = NULL;

    handle = dlopen("libb/libb.so", RTLD_LAZY);
    if (!handle)
        ERROR("dlopen filed\n");

    foo_ptr = (void (*)(void)) dlsym(handle, "libb_2");
    if (!foo_ptr)
        ERROR("dlsym failed\n");

    (*foo_ptr)();    
}
