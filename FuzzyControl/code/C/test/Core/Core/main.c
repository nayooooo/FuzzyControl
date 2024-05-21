#include <stdio.h>
#include "fc.h"

fc_list_head l;

int main()
{
    l = fc_list_create();
    printf("list place: 0x%X\r\n", (unsigned)l);

    int num = 100;
    for (int i = 1; i <= 10; i++)
    {
        printf("\r\n\n");
        printf("[%d] before push list length: %d\r\n", i, (int)fc_list_length(l));
        bool ret = fc_list_push_if(l, &i, sizeof(i), fc_list_pred_true, fc_list_node_data_construct_null);
        printf("[%d] push %s\r\n", i, ret ? ("success") : ("failed"));
        printf("[%d] after push list length: %d\r\n", i, (int)fc_list_length(l));
        int* a = fc_list_find_last_node(l)->data;
        printf("[%d] last node data: %d", i, *a);
    }

    return 0;
}
