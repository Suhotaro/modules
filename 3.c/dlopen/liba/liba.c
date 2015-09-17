#include <stdio.h>
#include "liba.h"
#include "../libb/libb.h"

int liba_global = 1;

void liba_procedure(void)
{
    int liba_local = 2;

    printf("liba_procedure: g=%d l=%d\n", liba_global, liba_local);

    libb_procedure();
}
