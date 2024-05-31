#include "fc_output.h"

#include "fc_def.h"

bool fc_output_register(struct fc_output* out)
{
	if (out == nullptr) return false;

	out->obj.name = "output";
	out->obj.type = FC_OBJ_OUTPUT;

	fuzzy_matrix_init(&(out->data));
	out->membership_fns = list_create();
	if (out->membership_fns == nullptr) return false;

	return true;
}
