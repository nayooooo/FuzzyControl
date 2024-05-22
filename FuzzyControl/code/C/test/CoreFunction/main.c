#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fc.h"

list_head l;

static bool int_pred_eq(list_node node, void* data)
{
    return *((int*)(node->data)) == *((int*)data);
}

static bool int_construct(void* data)
{
    *((int*)data) = 100;
    return true;
}

static bool int_deconstruct(void* data)
{
    printf("HELLO FROM DATA: %d\r\n", *((int*)data));
    return true;
}

static bool int_trav(list_node node, void* data)
{
    printf("%d  ", *((int*)(node->data)));
    /*for (int i = 0; i < *((int*)(node->data)); i++)
    {
        printf("*");
    }
    printf("    %d\r\n", *((int*)(node->data)));*/
    return true;
}

int main()
{
    srand((unsigned int)time(NULL));

    l = list_create();
    printf("list place: 0x%X\r\n", &(*l));
    
    // push
    printf("\r\n\n=========================================================\r\n");
    printf("push\r\n");
    printf("=========================================================\r\n");
    for (int i = 1; i <= 10; i++)
    {
        printf("\r\n\n");
        printf("[%d] before push list length: %d\r\n", i, (int)list_length(l));
        bool ret = list_push(l, &i, sizeof(i));
        printf("[%d] push %s\r\n", i, ret ? ("success") : ("failed"));
        printf("[%d] after push list length: %d\r\n", i, (int)list_length(l));
        list_node ln = list_get_last_node(l);
        if (ln != nullptr)
        {
            int* a = ln->data;
            printf("[%d] last node data: %d", i, *a);
        }
        else
        {
            printf("[%d] last node data: null", i);
        }
    }

    // pop
    printf("\r\n\n=========================================================\r\n");
    printf("pop\r\n");
    printf("=========================================================\r\n");
    for (int i = 1; i <= 10; i++)
    {
        printf("\r\n\n");
        printf("[%d] before pop list length: %d\r\n", i, (int)list_length(l));
        bool ret = list_pop(l);
        printf("[%d] pop %s\r\n", i, ret ? ("success") : ("failed"));
        printf("[%d] after pop list length: %d\r\n", i, (int)list_length(l));
        list_node ln = list_get_last_node(l);
        if (ln != nullptr)
        {
            int* a = ln->data;
            printf("[%d] last node data: %d", i, *a);
        }
        else
        {
            printf("[%d] last node data: null", i);
        }
    }

    // push
    printf("\r\n\n=========================================================\r\n");
    printf("push2\r\n");
    printf("=========================================================\r\n");
    for (int i = 1; i <= 20; i++)
    {
        printf("\r\n\n");
        printf("[%d] before push list length: %d\r\n", i, (int)list_length(l));
        bool ret = list_push(l, &i, sizeof(i));
        printf("[%d] push %s\r\n", i, ret ? ("success") : ("failed"));
        printf("[%d] after push list length: %d\r\n", i, (int)list_length(l));
        list_node ln = list_get_last_node(l);
        if (ln != nullptr)
        {
            int* a = ln->data;
            printf("[%d] last node data: %d", i, *a);
        }
        else
        {
            printf("[%d] last node data: null", i);
        }
    }

    //// clear
    //printf("\r\n\n=========================================================\r\n");
    //printf("clear\r\n");
    //printf("=========================================================\r\n");
    //printf("before clear list length: %d\r\n", (int)list_length(l));
    //list_clear(l, nullptr);
    //printf("after clear list length: %d\r\n", (int)list_length(l));

    // find_if
    printf("\r\n\n=========================================================\r\n");
    printf("find_if\r\n");
    printf("=========================================================\r\n");
    {
        list_head ll = list_create();
        for (int i = 1; i <= 100; i++)
        {
            printf("to find data = %d\r\n", i);
            list_node n = list_find_if(l, &i, int_pred_eq);
            printf("find_if %s\r\n", n ? ("success") : ("failed"));
            if (n)
            {
                list_push(ll, &i, sizeof(i));
            }
        }
        list_node n = list_find_if(ll, nullptr, list_pred_true);  // to get the 1st node
        printf("\r\n\n");
        if (n == nullptr) printf("not found any node, which data is between 1 and 100\r\n");
        printf("the list has number: \r\n");
        while (n)
        {
            int* num = n->data;
            printf("%d  ", *num);
            n = n->next;
        }
        printf("\r\n\n");
        list_delete(ll, nullptr);
    }

    // remove_if
    printf("\r\n\n=========================================================\r\n");
    printf("remove_if\r\n");
    printf("=========================================================\r\n");
    for (int i = 1; i <= 30; i++)
    {
        int num = rand() % i;
        printf("\r\n\n");
        printf("[%d] before remove_if list length: %d\r\n", i, (int)list_length(l));
        printf("will remove: %d\r\n", num);
        bool ret = list_remove_if(l, &num, int_pred_eq, int_deconstruct);
        printf("[%d] remove_if %s\r\n", i, ret ? ("success") : ("failed"));
    }

    // swap_if
    printf("\r\n\n=========================================================\r\n");
    printf("swap_if\r\n");
    printf("=========================================================\r\n");
    list_clear(l, nullptr);
    {
        int n1, n2;
        for (int i = 1; i <= 1; i++)
        {
            list_push(l, &i, sizeof(i));
        }
        printf("list length: %d\r\n", (int)list_length(l));
        printf("before swap node data: \r\n");
        list_node n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 1; n2 = 1;
        bool ret = list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");
    }

    list_clear(l, nullptr);
    {
        int n1, n2;
        for (int i = 1; i <= 2; i++)
        {
            list_push(l, &i, sizeof(i));
        }
        printf("list length: %d\r\n", (int)list_length(l));
        printf("before swap node data: \r\n");
        list_node n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 1; n2 = 2;
        bool ret = list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");
    }

    list_clear(l, nullptr);
    {
        int n1, n2;
        for (int i = 1; i <= 3; i++)
        {
            list_push(l, &i, sizeof(i));
        }
        printf("list length: %d\r\n", (int)list_length(l));
        printf("before swap node data: \r\n");
        list_node n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 2; n2 = 1;
        bool ret = list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");
    }

    list_clear(l, nullptr);
    {
        int n1, n2;
        for (int i = 1; i <= 3; i++)
        {
            list_push(l, &i, sizeof(i));
        }
        printf("list length: %d\r\n", (int)list_length(l));
        printf("before swap node data: \r\n");
        list_node n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 2; n2 = 3;
        bool ret = list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");
    }

    list_clear(l, nullptr);
    {
        int n1, n2;
        for (int i = 1; i <= 10; i++)
        {
            list_push(l, &i, sizeof(i));
        }
        printf("list length: %d\r\n", (int)list_length(l));
        printf("before swap node data: \r\n");
        list_node n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 1; n2 = 9;
        bool ret = list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");

        printf("before swap node data: \r\n");
        n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 1; n2 = 10;
        ret = list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = list_find_if(l, nullptr, list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");
    }

    // trav
    printf("\r\n\n=========================================================\r\n");
    printf("trav\r\n");
    printf("=========================================================\r\n");
    list_clear(l, nullptr);
    {
        for (int i = 0; i <= 20; i++)
        {
            list_push(l, &i, sizeof(i));
        }
        list_trav(l, nullptr, int_trav);
        printf("\r\n\n");
    }

    // get node data
    printf("\r\n\n=========================================================\r\n");
    printf("get node data\r\n");
    printf("=========================================================\r\n");
    list_clear(l, nullptr);
    {
        for (int i = 0; i < 20; i++)
        {
            list_push(l, &i, sizeof(i));
        }
        printf("list node data: \r\n");
        list_trav(l, nullptr, int_trav);
        printf("\r\n\n");
        int* num;
        printf("list_get_node_data\r\n");
        num = list_get_node_data(l,             1);
        printf("to get the node, which index is 1: %d\r\n",  num ? *num : -1);
        num = list_get_node_data(l,             6);
        printf("to get the node, which index is 6: %d\r\n",  num ? *num : -1);
        num = list_get_node_data(l,             14);
        printf("to get the node, which index is 14: %d\r\n", num ? *num : -1);
        num = list_get_node_data(l,             18);
        printf("to get the node, which index is 18: %d\r\n", num ? *num : -1);
        num = list_get_node_data(l,             22);
        printf("to get the node, which index is 22: %d\r\n", num ? *num : -1);

        printf("list_get_node_data_if\r\n");
        int a;
        a = 1;
        num = list_get_node_data_if(l, &a, int_pred_eq);
        printf("to get the node, which index is 1: %d\r\n", num ? *num : -1);
        a = 6;
        num = list_get_node_data_if(l, &a, int_pred_eq);
        printf("to get the node, which index is 6: %d\r\n", num ? *num : -1);
        a = 14;
        num = list_get_node_data_if(l, &a, int_pred_eq);
        printf("to get the node, which index is 14: %d\r\n", num ? *num : -1);
        a = 18;
        num = list_get_node_data_if(l, &a, int_pred_eq);
        printf("to get the node, which index is 18: %d\r\n", num ? *num : -1);
        a = 22;
        num = list_get_node_data_if(l, &a, int_pred_eq);
        printf("to get the node, which index is 22: %d\r\n", num ? *num : -1);
    }

    list_delete(l, nullptr);

    printf("input enter...");
    getchar();

    return 0;
}
