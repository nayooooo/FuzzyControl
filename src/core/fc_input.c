#include "fc_input.h"

#include "fc_def.h"

bool fc_input_register(struct fc_input* in)
{
    if (in == nullptr) return false;

    if (!fc_interface_register(&(in->obj), FC_OBJ_INPUT)) return false;

    in->register_dev = fc_input_register;
    in->unregister_dev = fc_input_unregister;
    in->add_membership_fn = fc_input_add_membership_fn;
    in->clear_membership_fn = fc_input_clear_membership_fn;
    in->fuzzing = fc_input_fuzzing;
    in->print = fc_input_print_membership_vector;

    return true;
}

bool fc_input_unregister(struct fc_input* in)
{
    if (in == nullptr) return false;

    if (!fc_interface_unregister(&(in->obj))) return false;

    in->register_dev = nullptr;
    in->unregister_dev = nullptr;
    in->add_membership_fn = nullptr;
    in->clear_membership_fn = nullptr;
    in->fuzzing = nullptr;
    in->print = nullptr;

    return true;
}

bool fc_input_add_membership_fn(struct fc_input* in, fc_membership_fn fn)
{
    if (in == nullptr || fn == nullptr) return false;

    return fc_interface_add_membership_fn(&(in->obj), fn, FC_OBJ_INPUT);
}

bool fc_input_clear_membership_fn(struct fc_input* in)
{
    if (in == nullptr) return false;

    return fc_interface_clear_membership_fn(&(in->obj), FC_OBJ_INPUT);
}

bool fc_input_fuzzing(struct fc_input* in, accurate_number* value, fuzzy_size value_size)
{
    if (in == nullptr || value == nullptr) return false;
    if (value_size <= 0) return false;

    return fc_interface_fuzzing(&(in->obj), value, value_size);
}

bool fc_input_print_membership_vector(struct fc_input* in)
{
    if (in == nullptr) return false;

    return fc_interface_print_membership_vector(&(in->obj));
}
