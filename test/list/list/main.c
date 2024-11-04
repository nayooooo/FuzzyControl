#include <stdio.h>

#include "list.h"

#include "vld.h"

#ifndef UNUSED
#   define UNUSED(data) ((void)data)
#endif

#ifndef nullptr
#   define nullptr NULL
#endif

static bool int_trav(list_node n, void* data)
{
    UNUSED(data);

    int* num = n->data;
    printf("%d  ", *num);

    return true;
}

static bool list_node_trav(list_node n, void* data)
{
    UNUSED(data);

    list_head* ptr = n->data;
    printf("%p  ", *ptr);

    return true;
}

static bool int_prev_eq(list_node n, void* data)
{
    return *(int*)(n->data) == *(int*)data;
}

static bool int_prev_even(list_node n, void* data)
{
    UNUSED(data);

    return ((*(int*)(n->data) > 0) && (*(int*)(n->data) % 2 == 0));
}

int main()
{
    int a = 3;
    int b = 11;
    list_head l;

    printf("\r\n\n");
    l = list_create();
    if (l == nullptr)
    {
        printf("list create failed!\r\n");
        goto list_over;
    }
    printf("[create] list address: 0x%X\r\n", l);
    printf("[create] list node number: %u\r\n", list_length(l));

    printf("\r\n\n");
    for (int i = 0; i < 20; i++)
    {
        list_push(l, &i, sizeof(i));
    }
    printf("[push] list node number: %u\r\n", list_length(l));
    printf("[push] node data: \r\n");
    list_trav(l, nullptr, int_trav);
    printf("\r\n");

    printf("\r\n\n");
    for (int i = 0; i < 20; i++)
    {
        list_remove_if(l, nullptr, int_prev_even, nullptr);
    }
    printf("[remove_if] list node number: %u\r\n", list_length(l));
    printf("[remove_if] node data: \r\n");
    list_trav(l, nullptr, int_trav);
    printf("\r\n");

    printf("\r\n\n");
    for (int i = 0; i < 3; i++)
    {
        list_pop(l);
    }
    printf("[pop] list node number: %u\r\n", list_length(l));
    printf("[pop] node data: \r\n");
    list_trav(l, nullptr, int_trav);
    printf("\r\n");

    printf("\r\n\n");
    list_swap_if(l, &a, &b, int_prev_eq, int_prev_eq);
    printf("[swap_if] list node number: %u\r\n", list_length(l));
    printf("[swap_if] node data: \r\n");
    list_trav(l, nullptr, int_trav);
    printf("\r\n");

list_over:
    printf("\r\n\n");
    list_delete(l, nullptr);
    l = nullptr;
    printf("[delete] list address: 0x%X\r\n", l);

    l = list_create();
    list_head ll = list_create();

    printf("\r\n\n");
    list_push_list(l, &ll);
    list_push_list(l, &ll);
    list_push_list(l, &ll);
    printf("ll place: %p\r\n", ll);
    list_trav(l, nullptr, list_node_trav);
    printf("\r\n");

list_list_over:
    list_delete(l, nullptr);
    list_delete(ll, nullptr);

    return 0;
}
