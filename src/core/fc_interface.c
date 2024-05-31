#include "fc_interface.h"

#include "fc_def.h"

bool fc_interface_register(struct fc_interface* interface, fc_obj_type type)
{
    if (interface == nullptr) return false;
    if (!__IS_FC_OBJ_TYPE(type)) return false;

    if (__IS_FC_OBJ_TYPE_INPUT(type)) interface->obj.name = "input";
    else if (__IS_FC_OBJ_TYPE_OUTPUT(type)) interface->obj.name = "output";
    else return false;
    interface->obj.type = type;

    fuzzy_matrix_init(&(interface->data));
    interface->membership_fns = list_create();
    if (interface->membership_fns == nullptr)
    {
        fuzzy_matrix_delete(&(interface->data));
        return false;
    }

    return true;
}

bool fc_interface_unregister(struct fc_interface* interface)
{
    if (interface == nullptr) return false;

    interface->obj.name = "";
    interface->obj.type = FC_OBJ_NULL;

    fuzzy_matrix_delete(&(interface->data));
    if (!list_delete(interface->membership_fns, nullptr)) return false;

    return true;
}

bool fc_interface_add_membership_fn(struct fc_interface* interface, fc_membership_fn fn, fc_obj_type type)
{
    if (interface == nullptr || fn == nullptr) return false;

    if (!__IS_FC_OBJ_TYPE_IS(interface->obj.type, type)) return false;

    return list_push(interface->membership_fns, &fn, sizeof(fc_membership_fn));
}

bool fc_interface_clear_membership_fn(struct fc_interface* interface, fc_obj_type type)
{
    if (interface == nullptr) return false;

    if (!__IS_FC_OBJ_TYPE_IS(interface->obj.type, type)) return false;

    return list_clear(interface->membership_fns, nullptr);
}

bool fc_interface_fuzzing(struct fc_interface* interface, accurate_number* value, fuzzy_size value_size)
{
    if (interface == nullptr || value == nullptr) return false;
    if (value_size <= 0) return false;

    if (!__IS_FC_OBJ_TYPE_IS(interface->obj.type, FC_OBJ_INPUT)) return false;
    
    // Number of columns in the membership vector
    list_size ms_vector_col = list_length(interface->membership_fns);
    if (ms_vector_col <= 0) return false;

    // Create appropriate membership vectors
    if (__IS_FUZZY_MATRIX_CREATED(&(interface->data)))
    {
        fuzzy_matrix_delete(&(interface->data));
    }
    if (!fuzzy_matrix_create(&(interface->data), value_size, ms_vector_col)) return false;

    // Start fuzzing
    for (fuzzy_size r = 0; r < value_size; r++)
    {
        list_node n = list_get_first_node(interface->membership_fns);
        for (list_size i = 0; i < ms_vector_col; i++)
        {
            if (n == nullptr) return false;
            fc_membership_fn ms_fn = *(fc_membership_fn*)(n->data);
            if (ms_fn == nullptr) return false;
            interface->data.mat[r][i] = ms_fn(value[r]);
            if (interface->data.mat[r][i] == FUZZY_DATA_ILLEGAL_VALUE) return false;
            n = list_find_next_node(interface->membership_fns, n);
        }
    }

    return true;
}

// TODO
bool fc_interface_unfuzzing(struct fc_interface* interface, accurate_number* value, fuzzy_size value_size)
{
    if (interface == nullptr || value == nullptr) return false;
    if (value_size <= 0) return false;

    if (!__IS_FC_OBJ_TYPE_IS(interface->obj.type, FC_OBJ_OUTPUT)) return false;

    return false;
}

bool fc_interface_print_membership_vector(struct fc_interface* interface)
{
    if (interface == nullptr) return false;
    if (!__IS_FUZZY_MATRIX_CREATED(&(interface->data))) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(&(interface->data))) return false;
    if (!__IS_FC_OBJ_TYPE(interface->obj.type)) return false;

    fuzzy_matrix_print(&(interface->data), interface->obj.name);

    return true;
}
