#include <stdio.h>
#include "libb.h"

int libb_global = 3;

void libb_procedure(void)
{
    int libb_local = 4;

    printf("libb_procedure: g=%d l=%d\n", libb_global, libb_local);
}
