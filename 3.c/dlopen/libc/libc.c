#include <stdio.h>
#include "libc.h"

int libc_global = 5;

void libc_3(void)
{
    int libc_local = 6;

    printf("libc_3: g=%d l=%d\n", libc_global, libc_local);
}
