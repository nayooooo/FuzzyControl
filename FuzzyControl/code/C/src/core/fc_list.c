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

fc_list_head fc_list_create(void)
{
    fc_list_head l = (fc_list_head)fc_mem_malloc(sizeof(fc_list_node));

    return l;
}

bool fc_list_clear(fc_list_head l)
{
    return false;
}

bool fc_list_delete(fc_list_head l)
{
    return false;
}

fc_list_node fc_list_find_if(fc_list_head l, void *data, fc_list_pred pred)
{
    return nullptr;
}

bool fc_list_push_if(fc_list_head l, void *data, fc_list_pred pred)
{
    return false;
}

bool fc_list_pop_if(fc_list_head l, void *data, fc_list_pred pred)
{
    return false;
}

bool fc_list_swap_if(fc_list_head l, void *data1, void *data2, fc_list_pred pred)
{
    return false;
}
