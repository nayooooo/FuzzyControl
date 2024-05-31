#include "fc_interface.h"

#include "fc_def.h"

bool fc_interface_register(struct fc_interface* const interface, const char* name, const fc_obj_type type)
{
    if (interface == nullptr || name == nullptr) return false;
    if (!__IS_FC_OBJ_TYPE(type)) return false;

    interface->obj.name = name;
    interface->obj.type = type;

    fuzzy_matrix_init(&(interface->data));
    interface->l = list_create();
    if (interface->l == nullptr)
    {
        fuzzy_matrix_delete(&(interface->data));
        return false;
    }

    return true;
}

bool fc_interface_unregister(struct fc_interface* const interface)
{
    if (interface == nullptr) return false;

    interface->obj.name = "";
    interface->obj.type = FC_OBJ_NULL;

    fuzzy_matrix_delete(&(interface->data));
    if (!list_delete(interface->l, nullptr)) return false;

    return true;
}

bool fc_interface_print_data(const struct fc_interface* const interface)
{
    if (interface == nullptr) return false;
    if (!__IS_FC_OBJ_TYPE(interface->obj.type)) return false;
    if (!__IS_FUZZY_MATRIX_CREATED(&(interface->data))) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(&(interface->data))) return false;

    fuzzy_matrix_print(&(interface->data), interface->obj.name);

    return true;
}
