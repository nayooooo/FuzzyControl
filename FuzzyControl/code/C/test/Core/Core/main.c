#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fc.h"

fc_list_head l;

static bool int_pred_eq(fc_list_node node, void* data)
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

static bool int_trav(fc_list_node node, void* data)
{
    //printf("%d  ", *((int*)(node->data)));
    for (int i = 0; i < *((int*)(node->data)); i++)
    {
        printf("*");
    }
    printf("    %d\r\n", *((int*)(node->data)));
    return true;
}

int main()
{
    srand((unsigned int)time(NULL));

    l = fc_list_create();
    printf("list place: 0x%X\r\n", (unsigned int)l);
    
    // push
    printf("\r\n\n=========================================================\r\n");
    printf("push\r\n");
    printf("=========================================================\r\n");
    for (int i = 1; i <= 10; i++)
    {
        printf("\r\n\n");
        printf("[%d] before push list length: %d\r\n", i, (int)fc_list_length(l));
        bool ret = fc_list_push(l, &i, sizeof(i));
        printf("[%d] push %s\r\n", i, ret ? ("success") : ("failed"));
        printf("[%d] after push list length: %d\r\n", i, (int)fc_list_length(l));
        fc_list_node ln = fc_list_find_last_node(l);
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
        printf("[%d] before pop list length: %d\r\n", i, (int)fc_list_length(l));
        bool ret = fc_list_pop(l);
        printf("[%d] pop %s\r\n", i, ret ? ("success") : ("failed"));
        printf("[%d] after pop list length: %d\r\n", i, (int)fc_list_length(l));
        fc_list_node ln = fc_list_find_last_node(l);
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
        printf("[%d] before push list length: %d\r\n", i, (int)fc_list_length(l));
        bool ret = fc_list_push(l, &i, sizeof(i));
        printf("[%d] push %s\r\n", i, ret ? ("success") : ("failed"));
        printf("[%d] after push list length: %d\r\n", i, (int)fc_list_length(l));
        fc_list_node ln = fc_list_find_last_node(l);
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
    //printf("before clear list length: %d\r\n", (int)fc_list_length(l));
    //fc_list_clear(l, nullptr);
    //printf("after clear list length: %d\r\n", (int)fc_list_length(l));

    // find_if
    printf("\r\n\n=========================================================\r\n");
    printf("find_if\r\n");
    printf("=========================================================\r\n");
    {
        fc_list_head ll = fc_list_create();
        for (int i = 1; i <= 100; i++)
        {
            printf("to find data = %d\r\n", i);
            fc_list_node n = fc_list_find_if(l, &i, int_pred_eq);
            printf("find_if %s\r\n", n ? ("success") : ("failed"));
            if (n)
            {
                fc_list_push(ll, &i, sizeof(i));
            }
        }
        fc_list_node n = fc_list_find_if(ll, nullptr, fc_list_pred_true);  // to get the 1st node
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
        fc_list_delete(ll, nullptr);
    }

    // remove_if
    printf("\r\n\n=========================================================\r\n");
    printf("remove_if\r\n");
    printf("=========================================================\r\n");
    for (int i = 1; i <= 30; i++)
    {
        int num = rand() % i;
        printf("\r\n\n");
        printf("[%d] before remove_if list length: %d\r\n", i, (int)fc_list_length(l));
        printf("will remove: %d\r\n", num);
        bool ret = fc_list_remove_if(l, &num, int_pred_eq, int_deconstruct);
        printf("[%d] remove_if %s\r\n", i, ret ? ("success") : ("failed"));
    }

    // swap_if
    printf("\r\n\n=========================================================\r\n");
    printf("swap_if\r\n");
    printf("=========================================================\r\n");
    fc_list_clear(l, nullptr);
    {
        int n1, n2;
        for (int i = 1; i <= 1; i++)
        {
            fc_list_push(l, &i, sizeof(i));
        }
        printf("list length: %d\r\n", (int)fc_list_length(l));
        printf("before swap node data: \r\n");
        fc_list_node n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 1; n2 = 1;
        bool ret = fc_list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");
    }

    fc_list_clear(l, nullptr);
    {
        int n1, n2;
        for (int i = 1; i <= 2; i++)
        {
            fc_list_push(l, &i, sizeof(i));
        }
        printf("list length: %d\r\n", (int)fc_list_length(l));
        printf("before swap node data: \r\n");
        fc_list_node n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 1; n2 = 2;
        bool ret = fc_list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");
    }

    fc_list_clear(l, nullptr);
    {
        int n1, n2;
        for (int i = 1; i <= 3; i++)
        {
            fc_list_push(l, &i, sizeof(i));
        }
        printf("list length: %d\r\n", (int)fc_list_length(l));
        printf("before swap node data: \r\n");
        fc_list_node n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 2; n2 = 1;
        bool ret = fc_list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");
    }

    fc_list_clear(l, nullptr);
    {
        int n1, n2;
        for (int i = 1; i <= 3; i++)
        {
            fc_list_push(l, &i, sizeof(i));
        }
        printf("list length: %d\r\n", (int)fc_list_length(l));
        printf("before swap node data: \r\n");
        fc_list_node n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 2; n2 = 3;
        bool ret = fc_list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");
    }

    fc_list_clear(l, nullptr);
    {
        int n1, n2;
        for (int i = 1; i <= 10; i++)
        {
            fc_list_push(l, &i, sizeof(i));
        }
        printf("list length: %d\r\n", (int)fc_list_length(l));
        printf("before swap node data: \r\n");
        fc_list_node n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 1; n2 = 9;
        bool ret = fc_list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n\n");

        printf("before swap node data: \r\n");
        n = fc_list_find_if(l, nullptr, fc_list_pred_true);
        while (n)
        {
            printf("%d  ", *((int*)(n->data)));
            n = n->next;
        }
        printf("\r\n");
        n1 = 1; n2 = 10;
        ret = fc_list_swap_if(l, &n1, &n2, int_pred_eq, int_pred_eq);
        printf("swap_if %s\r\n", ret ? ("success") : ("failed"));
        printf("after swap node data: \r\n");
        n = fc_list_find_if(l, nullptr, fc_list_pred_true);
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
    fc_list_clear(l, nullptr);
    {
        for (int i = 0; i <= 20; i++)
        {
            fc_list_push(l, &i, sizeof(i));
        }
        fc_list_trav(l, nullptr, int_trav);
        printf("\r\n\n");
    }

    fc_list_delete(l, nullptr);

    return 0;
}
