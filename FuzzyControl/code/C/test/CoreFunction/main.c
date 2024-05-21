#include <stdio.h>
#include "./../../src/fc.h"

fc_list_head l;

int main()
{
    l = fc_list_create();

    printf("list place: 0x%X", l);

    return 0;
}
