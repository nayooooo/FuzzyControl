#include "fc_output.h"

static bool __fc_output_fuzzy_set_construct_cb(void* data)
{
	if (data == nullptr) return false;

	struct fuzzy_set* set = data;

	if (set->label == nullptr) return false;
	const char* label = (const char*)__FC_OUTPUT_MALLOC((__FC_OUTPUT_STRLEN(set->label) + 1) * sizeof(char));
	if (label == nullptr) return false;
	__FC_OUTPUT_STRCPY_S((char*)label, (__FC_OUTPUT_STRLEN(set->label) + 1) * sizeof(char), set->label);
	set->label = label;

	return true;
}

static bool __fc_output_fuzzy_set_deconstruct_cb(void* data)
{
	if (data == nullptr) return false;

	struct fuzzy_set* set = data;

	if (set->label == nullptr) return false;
	__FC_OUTPUT_FREE((void*)(set->label));
	set->label = nullptr;

	return true;
}

static bool __fc_output_inference_result_construct_cb(void* data)
{
	if (data == nullptr) return false;

	struct inference_result* ir = data;
	if (ir->name_tag == nullptr) return false;
	const char* name_tag = (const char*)__FC_OUTPUT_MALLOC((__FC_OUTPUT_STRLEN(ir->name_tag) + 1) * sizeof(char));
	if (name_tag == nullptr) return false;
	__FC_OUTPUT_STRCPY_S((char*)name_tag, (__FC_OUTPUT_STRLEN(ir->name_tag) + 1) * sizeof(char), ir->name_tag);
	ir->name_tag = name_tag;

	return true;
}

static bool __fc_output_inference_result_deconstruct_cb(void* data)
{
	if (data == nullptr) return false;

	struct inference_result* ir = data;

	if (ir->name_tag == nullptr) return false;
	__FC_OUTPUT_FREE((void*)(ir->name_tag));
	ir->name_tag = nullptr;

	return true;
}

static bool __fc_output_inference_result_group_deconstruct_cb(void* data)
{
	if (data == nullptr) return false;

	return list_delete(*(list_head*)data, __fc_output_inference_result_deconstruct_cb);
}

bool fc_output_register(struct fc_output* const out, const char* name)
{
	if (out == nullptr || name == nullptr) return false;

	out->name = (const char*)__FC_OUTPUT_MALLOC((__FC_OUTPUT_STRLEN(name) + 1) * sizeof(char));
	if (out->name == nullptr) return false;
	__FC_OUTPUT_STRCPY_S((char*)(out->name), (__FC_OUTPUT_STRLEN(name) + 1) * sizeof(char), name);
	out->data = list_create();
	if (out->data == nullptr) goto _error_out;
	out->fuzzy_set = list_create();
	if (out->fuzzy_set == nullptr) goto _error_out;

	return true;

_error_out:
	if (out->name != nullptr)
	{
		__FC_OUTPUT_FREE((void*)(out->name));
		out->name = nullptr;
	}
	if (out->data != nullptr)
	{
		list_delete(out->data, nullptr);
		out->data = nullptr;
	}
	if (out->fuzzy_set != nullptr)
	{
		list_delete(out->fuzzy_set, nullptr);
		out->fuzzy_set = nullptr;
	}

	return false;
}

bool fc_output_unregister(struct fc_output* const out)
{
	if (out == nullptr) return false;

	if (out->name != nullptr)
	{
		__FC_OUTPUT_FREE((void*)(out->name));
		out->name = nullptr;
	}

	list_delete(out->data, __fc_output_inference_result_group_deconstruct_cb);
	list_delete(out->fuzzy_set, __fc_output_fuzzy_set_deconstruct_cb);
	out->data = nullptr;
	out->fuzzy_set = nullptr;

	return true;
}

bool fc_output_add_fuzzy_set(const struct fc_output* const out, const struct fuzzy_set* const set)
{
	if (out == nullptr || set == nullptr) return false;
	if (out->fuzzy_set == nullptr || set->label == nullptr || set->ms == nullptr) return false;

	if (!list_push_if(out->fuzzy_set, (void*)set, sizeof(struct fuzzy_set), list_pred_true, __fc_output_fuzzy_set_construct_cb))
		return false;

	return true;
}

bool fc_output_clear_fuzzy_set(const struct fc_output* const out)
{
	if (out == nullptr) return false;
	if (out->fuzzy_set == nullptr) return false;

	if (!list_clear(out->fuzzy_set, __fc_output_fuzzy_set_deconstruct_cb))
		return false;

	return true;
}

bool fc_output_increase_a_inference_result_group(const struct fc_output* const out)
{
	if (out == nullptr) return false;
	if (out->data == nullptr) return false;

	return list_push_list(out->data, nullptr);
}

bool fc_output_decrease_a_inference_result_group(const struct fc_output* const out)
{
	if (out == nullptr) return false;
	if (out->data == nullptr) return false;

	return list_pop_if(out->data, nullptr, list_pred_true, __fc_output_inference_result_group_deconstruct_cb);
}

bool fc_output_delete_all_inference_result_group(const struct fc_output* const out)
{
	if (out == nullptr) return false;
	if (out->data == nullptr) return false;

	return list_clear(out->data, __fc_output_inference_result_group_deconstruct_cb);
}

fc_size fc_output_get_group_num(const struct fc_output* const out)
{
	if (out == nullptr) return 0;
	if (out->data == nullptr) return 0;

	return (fc_size)list_length(out->data);
}

bool fc_output_add_inference_result(const struct fc_output* const out, fc_index ind, const struct inference_result* const ir)
{
	if (out == nullptr) return false;
	if (out->data == nullptr) return false;
	if (ind < 0) return false;
	if (ir == nullptr) return false;
	if (ir->name_tag == nullptr) return false;

	// find group
	list_head *al = list_get_node_data(out->data, ind);
	if (al == nullptr || *al == nullptr) return false;

	// push ir
	return list_push_if(*al, (void*)ir, sizeof(struct inference_result), list_pred_true, __fc_output_inference_result_construct_cb);
}

bool fc_output_clear_inference_result(const struct fc_output* const out, fc_index ind)
{
	if (out == nullptr) return false;
	if (out->data == nullptr) return false;
	if (ind < 0) return false;

	list_head *al = list_get_node_data(out->data, ind);
	if (al == nullptr || *al == nullptr) return false;

	return list_clear(*al, __fc_output_inference_result_deconstruct_cb);
}

fc_size fc_output_get_inference_result_num(const struct fc_output* const out, fc_index ind)
{
	if (out == nullptr) return 0;
	if (out->data == nullptr) return 0;
	if (ind < 0) return 0;

	list_head *al = list_get_node_data(out->data, ind);
	if (al == nullptr || *al == nullptr) return 0;

	return (fc_size)list_length(*al);
}

bool fc_output_print_inference_result(struct fc_output* const out, fc_index ind, const char* label)
{
	if (out == nullptr) return false;
	if (out->data == nullptr) return false;
	if (ind < 0) return false;

	__FC_OUTPUT_PRINTF("%s: \r\n", label ? label : "(unset label)");

	list_head *al = list_get_node_data(out->data, ind);
	if (al == nullptr || *al == nullptr)
	{
		__FC_OUTPUT_PRINTF("ind is error!\r\n");
		return false;
	}
	list_node n = list_get_first_node(*al);
	if (n == nullptr)
	{
		__FC_OUTPUT_PRINTF("There is no inference result!\r\n");
		return true;
	}
	while (n != nullptr)
	{
		__FC_OUTPUT_PRINTF("%s: %6.4f\r\n", ((struct inference_result*)(n->data))->name_tag, ((struct inference_result*)(n->data))->activate);

		n = list_find_next_node(*al, n);
	}

	return true;
}

bool fc_output_unfuzzing(struct fc_output* const out, fc_index ind, accurate_number* data, fc_size num)
{
	if (out == nullptr) return false;
	if (out->data == nullptr || out->fuzzy_set == nullptr) return false;
	if (ind < 0) return false;
	if (data == nullptr || num <= 0) return false;

	// TODO:
	// 1. find target ir group
	// 2. verify every ir have corresponding membership function
	// 3. calculate output curve or output value
}

bool fc_output_print_fuzzy_set(struct fc_output* const out, const char* label)
{
	if (out == nullptr) return false;
	if (out->name == nullptr || out->fuzzy_set == nullptr) return false;

	__FC_OUTPUT_PRINTF("%s: \r\n", label ? label : "(unset label)");
	list_node n = list_get_first_node(out->fuzzy_set);
	if (n == nullptr)
	{
		__FC_OUTPUT_PRINTF("There is no fuzzy set!\r\n");
		return true;
	}
	while (n != nullptr)
	{
		__FC_OUTPUT_PRINTF("%s-%s\r\n", out->name, ((struct fuzzy_set*)(n->data))->label);

		n = list_find_next_node(out->fuzzy_set, n);
	}

	return true;
}
