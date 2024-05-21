#include "fc_list.h"
#include "fc_mem.h"

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
    fc_list_head l = (fc_list_head)fc_mem_malloc(sizeof(fc_list_node));
    if (l == nullptr) return nullptr;

    *l = nullptr;

    return l;
}

bool fc_list_clear(fc_list_head l, fc_list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;

    return false;
}

bool fc_list_delete(fc_list_head l, fc_list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;

    if (!fc_list_clear(l, deconstruct)) return false;

    fc_mem_free(l);

    return true;
}

fc_list_node fc_list_find_if(fc_list_head l, void *data, fc_list_pred pred)
{
    return nullptr;
}

bool fc_list_push_if(fc_list_head l, void *data, size_t data_size, fc_list_pred pred, fc_list_node_data_construct_cb construct)
{
    return false;
}

bool fc_list_pop_if(fc_list_head l, void *data, fc_list_pred pred, fc_list_node_data_deconstruct_cb deconstruct)
{
    return false;
}

bool fc_list_swap_if(fc_list_head l, void *data1, void *data2, fc_list_pred pred1, fc_list_pred pred2)
{
    return false;
}
