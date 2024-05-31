#include "fc_input.h"

#include "fc_def.h"

bool fc_input_register(struct fc_input* const in, const char* name)
{
    if (in == nullptr || name == nullptr) return false;

    if (!fc_interface_register(&(in->interface), name, FC_OBJ_INPUT)) return false;

    in->register_dev = fc_input_register;
    in->unregister_dev = fc_input_unregister;
    in->add_membership_fn = fc_input_add_membership_fn;
    in->clear_membership_fn = fc_input_clear_membership_fn;
    in->fuzzing = fc_input_fuzzing;
    in->print = fc_input_print_membership_vector_with_label;

    return true;
}

bool fc_input_unregister(struct fc_input* const in)
{
    if (in == nullptr) return false;

    if (!fc_interface_unregister(&(in->interface))) return false;

    in->register_dev = nullptr;
    in->unregister_dev = nullptr;
    in->add_membership_fn = nullptr;
    in->clear_membership_fn = nullptr;
    in->fuzzing = nullptr;
    in->print = nullptr;

    return true;
}

bool fc_input_add_membership_fn(const struct fc_input* const in, const fc_membership_fn fn, const char* label)
{
    if (in == nullptr || fn == nullptr || label == nullptr) return false;

    if (!__IS_FC_OBJ_TYPE_INPUT(in->interface.obj.type)) return false;

    struct membership_fn_label fnl = {
        .fn = fn,
        .label = label
    };

    return list_push(in->interface.l, &fnl, sizeof(struct membership_fn_label));
}

bool fc_input_clear_membership_fn(const struct fc_input* const in)
{
    if (in == nullptr) return false;

    if (!__IS_FC_OBJ_TYPE_INPUT(in->interface.obj.type)) return false;

    return list_clear(in->interface.l, nullptr);
}

bool fc_input_fuzzing(const struct fc_input* const in, const accurate_number* const value, const fuzzy_size value_size)
{
    if (in == nullptr || value == nullptr) return false;
    if (value_size <= 0) return false;

    if (!__IS_FC_OBJ_TYPE_INPUT(in->interface.obj.type)) return false;

    // Number of columns in the membership vector
    list_size ms_vector_col = list_length(in->interface.l);
    if (ms_vector_col <= 0) return false;

    // Create appropriate membership vectors
    if (__IS_FUZZY_MATRIX_CREATED(&(in->interface.data)))
    {
        fuzzy_matrix_delete(&(in->interface.data));
    }
    if (!fuzzy_matrix_create(&(in->interface.data), value_size, ms_vector_col)) return false;

    // Start fuzzing
    for (fuzzy_size r = 0; r < value_size; r++)
    {
        list_node n = list_get_first_node(in->interface.l);
        for (list_size i = 0; i < ms_vector_col; i++)
        {
            if (n == nullptr) return false;
            fc_membership_fn ms_fn = ((struct membership_fn_label*)(n->data))->fn;
            if (ms_fn == nullptr) return false;
            in->interface.data.mat[r][i] = ms_fn(value[r]);
            if (in->interface.data.mat[r][i] == FUZZY_DATA_ILLEGAL_VALUE) return false;
            n = list_find_next_node(in->interface.l, n);
        }
    }

    return true;
}

bool fc_input_print_membership_vector(const struct fc_input* const in)
{
    if (in == nullptr) return false;

    return fc_interface_print_data(&(in->interface));
}

bool fc_input_print_membership_vector_with_label(const struct fc_input* const in)
{
    if (in == nullptr) return false;

    list_node n = list_get_first_node(in->interface.l);
    if (n == nullptr) return false;
    list_index i = 0;
    __FUZZY_MATRIX_PRINTF("\r\n\n");
    while (n != nullptr)
    {
        const char* label = ((struct membership_fn_label*)(n->data))->label;
        if (label == nullptr) return false;
        __FUZZY_MATRIX_PRINTF("[%u] %s\r\n", i++, label);
        n = list_find_next_node(in->interface.l, n);
    }

    return fc_interface_print_data(&(in->interface));
}
