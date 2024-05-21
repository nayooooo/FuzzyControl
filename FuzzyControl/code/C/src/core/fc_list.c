#include "fc_list.h"
#include "fc_std.h"

bool fc_list_pred_true(fc_list_node node, void *data)
{
    UNUSED(node);
    UNUSED(data);
    return true;
}

bool fc_list_pred_false(fc_list_node node, void *data)
{
    UNUSED(node);
    UNUSED(data);
    return false;
}

bool fc_list_node_data_construct_null(void *data)
{
    UNUSED(data);
    return true;
}

bool fc_list_node_data_deconstruct_null(void *data)
{
    UNUSED(data);
    return true;
}

fc_list_head fc_list_create(void)
{
    fc_list_head l = (fc_list_head)fc_malloc(sizeof(fc_list_node));
    if (l == nullptr) return nullptr;

    *l = nullptr;

    return l;
}

bool fc_list_clear(fc_list_head l, fc_list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;
    if (deconstruct == nullptr) return false;

    return false;
}

bool fc_list_delete(fc_list_head l, fc_list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;
    if (deconstruct == nullptr) return false;

    if (!fc_list_clear(l, deconstruct)) return false;

    fc_free(l);

    return true;
}

size_t fc_list_length(fc_list_head l)
{
    if (l == nullptr) return 0;
    
    fc_list_node n = *l;
    size_t length = 0;
    while (n)
    {
        length++;
        n = n->next;
    }

    return length;
}

fc_list_node fc_list_find_last_node(fc_list_head l)
{
    if (l == nullptr) return nullptr;

    if (*l == nullptr) return nullptr;

    fc_list_node n = *l;
    fc_list_node fn = n;
    while (n)
    {
        fn = n;
        n = n->next;
    }
    return fn;
}

fc_list_node fc_list_find_if(fc_list_head l, void *data, fc_list_pred pred)
{
    return nullptr;
}

bool fc_list_push_if(fc_list_head l, void *data, size_t data_size, fc_list_pred pred, fc_list_node_data_construct_cb construct)
{
    if (l == nullptr) return false;
    if (data == nullptr) return false;
    if (data_size <= 0) return false;
    if (pred == nullptr) return false;
    if (construct == nullptr) return false;

    // Find the last node and determine whether to create a new node
    fc_list_node ln = fc_list_find_last_node(l);
    if (!pred(ln, data)) return false;

    // Create a new node
    fc_list_node n = (fc_list_node)fc_malloc(sizeof(struct fc_list));
    if (n == nullptr) return false;
    n->data = (void*)fc_malloc(data_size);
    if (n->data == nullptr)
    {
        fc_free(n);
        return false;
    }
    fc_memcpy(n->data, data, data_size);
    construct(n->data);
    n->next = nullptr;

    // Link the created node to the end of the linked list
    if (ln == nullptr)
    {
        *l = n;
    }
    else
    {
        ln->next = n;
    }

    return true;
}

bool fc_list_pop_if(fc_list_head l, void *data, fc_list_pred pred, fc_list_node_data_deconstruct_cb deconstruct)
{
    return false;
}

bool fc_list_swap_if(fc_list_head l, void *data1, void *data2, fc_list_pred pred1, fc_list_pred pred2)
{
    return false;
}
