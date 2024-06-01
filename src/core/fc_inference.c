#include "fc_inference.h"
#include "fc_def.h"

/**
 * @brief Fuzzy input data description
 * 
 * @memberof name fuzzy input data name
 * @memberof data fuzzy input data
 * @memberof label fuzzy input data label(list)
 */
struct fuzzy_input_data_name_label
{
	const char* name;
	struct fuzzy_matrix data;
	list_head label;
};

static bool fidnl_deconstruct_cb(void* data)
{
	if (data == nullptr) return false;

	struct fuzzy_input_data_name_label* fidnl = data;
	fuzzy_matrix_delete(&(fidnl->data));
	list_delete(fidnl->label, nullptr);

	return true;
}

bool fc_inference_register(struct fc_inference* const infer, const char* name)
{
	if (infer == nullptr || name == nullptr) return false;

	if (!fc_interface_register(&(infer->interface), name, FC_OBJ_INFERENCE)) return false;

	infer->register_dev = fc_inference_register;
	infer->unregister_dev = fc_inference_unregister;
	infer->add_fuzzy_input_data = fc_inference_add_fuzzy_input_data;
	infer->clear_fuzzy_input_data = fc_inference_clear_fuzzy_input_data;
	infer->inference = fc_inference_inference;
	infer->print = fc_inference_print;

	return true;
}

bool fc_inference_unregister(struct fc_inference* const infer)
{
	if (infer == nullptr) return false;

	if (!fc_inference_clear_fuzzy_input_data(infer)) return false;
	
	if (!fc_interface_unregister(&(infer->interface))) return false;

	infer->register_dev = nullptr;
	infer->unregister_dev = nullptr;
	infer->add_fuzzy_input_data = nullptr;
	infer->clear_fuzzy_input_data = nullptr;
	infer->inference = nullptr;
	infer->print = nullptr;

	return true;
}

bool fc_inference_add_fuzzy_input_data(
	const struct fc_inference* const infer, const struct fuzzy_matrix* const data,
	const char* name, list_head label
)
{
	if (infer == nullptr || data == nullptr || name == nullptr || label == nullptr) return false;
	if (!__IS_FUZZY_MATRIX_CREATED(data)) return false;
	if (__IS_FUZZY_MATRIX_DAMAGED(data)) return false;
	if (data->row != 1) return false;
	if (list_length(label) <= 0) return false;

	struct fuzzy_input_data_name_label fidnl = {
		.name = name,
		.label = nullptr
	};
	fuzzy_matrix_init(&(fidnl.data));
	if (!fuzzy_matrix_copy(&(fidnl.data), data)) return false;
	fidnl.label = list_create();
	if (fidnl.label == nullptr) goto error_out;
	list_node n = list_get_first_node(label);
	while (n != nullptr)
	{
		if (!list_push(fidnl.label, n->data, __LIST_STRLEN(n->data) + 1)) goto error_out;
		n = list_find_next_node(label, n);
	}

	list_push(infer->interface.l, &fidnl, sizeof(struct fuzzy_input_data_name_label));

	if (0)
	{
	error_out:
		fuzzy_matrix_delete(&(fidnl.data));
		list_delete(fidnl.label, nullptr);
		return false;
	}

	return true;
}

bool fc_inference_clear_fuzzy_input_data(const struct fc_inference* const infer)
{
	if (infer == nullptr) return false;

	if (list_length(infer->interface.l) == 0) return true;
	else if (list_length(infer->interface.l) < 0) return false;

	if (!list_clear(infer->interface.l, fidnl_deconstruct_cb)) return false;

	return true;
}

bool fc_inference_inference(const struct fc_inference* const infer)
{
	if (infer == nullptr) return false;

	if (list_length(infer->interface.l) <= 0) return false;

	list_node n = list_get_first_node(infer->interface.l);
	if (n == nullptr) return false;
	struct fuzzy_input_data_name_label* fidnl = n->data;
	if (!fuzzy_matrix_copy(&(infer->interface.data), &(fidnl->data))) return false;
	n = list_find_next_node(infer->interface.l, n);
	while (n != nullptr)
	{
		fidnl = n->data;
		if (!fuzzy_opera_dir_pro(&(infer->interface.data), nullptr, &(fidnl->data))) return false;
		n = list_find_next_node(infer->interface.l, n);
	}

	return true;
}

bool fc_inference_print(const struct fc_inference* const infer)
{
	if (infer == nullptr) return false;

	return fc_interface_print_data(&(infer->interface));
}
