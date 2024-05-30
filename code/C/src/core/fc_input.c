#include "fc_input.h"

#include "fc_def.h"

bool fc_input_register(struct fc_input* in)
{
    if (in == nullptr) return false;

    in->obj.name = "input";
    in->obj.type = FC_OBJ_INPUT;

    fuzzy_matrix_init(&(in->data));
    in->membership_fns = list_create();
    if (in->membership_fns == nullptr) return false;

    return true;
}

bool fc_input_unregister(struct fc_input* in)
{
    if (in == nullptr) return false;

    in->obj.name = "";
    in->obj.type = FC_OBJ_NULL;

    fuzzy_matrix_delete(&(in->data));
    if (!list_delete(in->membership_fns, nullptr)) return false;

    return true;
}

bool fc_input_add_membership_fn(struct fc_input* in, fc_membership_fn fn)
{
    if (in == nullptr || fn == nullptr) return false;

    if (!IS_FC_OBJ_TYPE_INPUT(in->obj.type)) return false;

    return list_push(in->membership_fns, &fn, sizeof(fc_membership_fn));
}

bool fc_input_clear_membership_fn(struct fc_input* in)
{
    if (in == nullptr) return false;

    if (!IS_FC_OBJ_TYPE_INPUT(in->obj.type)) return false;

    return list_clear(in->membership_fns, nullptr);
}

bool fc_input_fuzzing(struct fc_input* in, accurate_number value)
{
    if (in == nullptr) return false;
    
    // Number of columns in the membership vector
    list_size ms_vector_col = list_length(in->membership_fns);
    if (ms_vector_col <= 0) return false;

    list_node n = list_get_first_node(in->membership_fns);
    if (n == nullptr) return false;

    // Create appropriate membership vectors
    if (__IS_FUZZY_MATRIX_CREATED(&(in->data)))
    {
        fuzzy_matrix_delete(&(in->data));
    }
    if (!fuzzy_matrix_create(&(in->data), 1, ms_vector_col)) return false;

    // Start fuzzing
    for (list_size i = 0; i < ms_vector_col; i++)
    {
        if (n == nullptr) return false;
        fc_membership_fn ms_fn = *(fc_membership_fn*)(n->data);
        if (ms_fn == nullptr) return false;
        in->data.mat[0][i] = ms_fn(value);
        if (in->data.mat[0][i] == FUZZY_DATA_ILLEGAL_VALUE) return false;
        n = list_find_next_node(in->membership_fns, n);
    }

    return true;
}

bool fc_input_print_membership_vector(struct fc_input* in)
{
    if (in == nullptr) return false;
    if (!__IS_FUZZY_MATRIX_CREATED(&(in->data))) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(&(in->data))) return false;

    fuzzy_matrix_print(&(in->data), in->obj.name);

    return true;
}
