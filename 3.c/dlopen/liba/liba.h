#include <stdio.h>

void liba_1(void);
void __attribute__ ((visibility("hidden"))) liba_2(void);
void liba_3(void);

void my_dlopen(void);
