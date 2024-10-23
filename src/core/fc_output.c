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

static bool __fc_output_inference_result_group_construct_cb(void* data)
{
	UNUSED(data);

	return true;
}

static bool __fc_output_inference_result_group_deconstruct_cb(void* data)
{
	if (data == nullptr) return false;

	list_head group = data;
	if (!list_clear(group, __fc_output_inference_result_deconstruct_cb))
		return false;

	return true;
}

bool fc_output_register(struct fc_output* const out, const char* name)
{
	if (out == nullptr || name == nullptr) return false;

	out->name = (const char*)__FC_OUTPUT_MALLOC((__FC_OUTPUT_STRLEN(name) + 1) * sizeof(char));
	if (out->name == nullptr) return false;
	__FC_OUTPUT_STRCPY_S((char*)(out->name), (__FC_OUTPUT_STRLEN(name) + 1) * sizeof(char), name);
	out->data = list_create();
	if (out->data == nullptr) goto error_out;
	out->fuzzy_set = list_create();
	if (out->fuzzy_set == nullptr) goto error_out;

	if (0)
	{
	error_out:
		if (out->name != nullptr)
		{
			__FC_OUTPUT_FREE((void*)(out->name));
			out->name = nullptr;
		}
		if (out->data != nullptr)
		{
			list_delete(out->data, nullptr);
		}
		if (out->fuzzy_set != nullptr)
		{
			list_delete(out->fuzzy_set, nullptr);
		}

		return false;
	}

	return true;
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

	return false;
}

bool fc_output_add_fuzzy_set(const struct fc_output* const out, const struct fuzzy_set* const set)
{
	if (out == nullptr || set == nullptr) return false;
	if (out->fuzzy_set == nullptr || set->ms == nullptr) return false;

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

	list_head group = list_create();
	if (group == nullptr) return false;
	if (!list_push_if(out->data, group, sizeof(list_head), list_pred_true, __fc_output_inference_result_group_construct_cb))
	{
		list_delete(group, nullptr);
		return false;
	}

	return true;
}

bool fc_output_decrease_a_inference_result_group(const struct fc_output* const out)
{
	if (out == nullptr) return false;
	if (out->data == nullptr) return false;

	if (!list_pop_if(out->data, nullptr, list_pred_true, __fc_output_inference_result_group_deconstruct_cb))
		return false;

	return true;
}
