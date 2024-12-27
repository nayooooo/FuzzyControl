#include "fc_controller.h"

static bool __fc_controller_input_deconstruct_cb(struct fc_input *in)
{
	if (in == nullptr) return false;

	return fc_input_unregister(in);
}

static bool __fc_controller_output_deconstruct_cb(struct fc_output *out)
{
	if (out == nullptr) return false;

	return fc_output_unregister(out);
}

static bool _fc_controller_input_name_is(list_node node, const char* name)
{
	if (node == nullptr) return false;
	if (node->data == nullptr) return false;

	return !__FC_CONTROLLER_STRCMP(((struct fc_input*)(node->data))->name, name);
}

static bool _fc_controller_output_name_is(list_node node, const char* name)
{
	if (node == nullptr) return false;
	if (node->data == nullptr) return false;

	return !__FC_CONTROLLER_STRCMP(((struct fc_output*)(node->data))->name, name);
}

bool fc_controller_register(struct fc_controller* const fcc)
{
	if (fcc == nullptr) return false;

	fcc->in = list_create();
	if (fcc->in == nullptr) goto _error_out;
	fcc->out = list_create();
	if (fcc->out == nullptr) goto _error_out;
	fcc->rules = (struct fc_rules*)__FC_CONTROLLER_MALLOC(sizeof(struct fc_rules));
	if (fcc->rules == nullptr) goto _error_out;
	if (!fc_rules_register(fcc->rules)) goto _error_out;;
	
	return true;

_error_out:

	list_delete(fcc->in, nullptr);
	list_delete(fcc->out, nullptr);
	fcc->in = nullptr;
	fcc->out = nullptr;
	if (fcc->rules != nullptr)
	{
		__FC_CONTROLLER_FREE((void*)(fcc->rules));
		fcc->rules = nullptr;
	}

	return false;
}

bool fc_controller_unregister(struct fc_controller* const fcc)
{
	if (fcc == nullptr) return false;

	list_delete(fcc->in, (list_node_data_deconstruct_cb)__fc_controller_input_deconstruct_cb);
	list_delete(fcc->out, (list_node_data_deconstruct_cb)__fc_controller_output_deconstruct_cb);
	fcc->in = nullptr;
	fcc->out = nullptr;
	if (fcc->rules != nullptr)
	{
		fc_rules_unregister(fcc->rules);
		__FC_CONTROLLER_FREE((void*)(fcc->rules));
		fcc->rules = nullptr;
	}

	return true;
}

bool fc_controller_increase_a_input(const struct fc_controller* const fcc, const char* name)
{
	if (fcc == nullptr) return false;
	if (fcc->in == nullptr) return false;
	if (name == nullptr) return false;

	struct fc_input in;
	if (!list_push(fcc->in, (void*)&in, sizeof(struct fc_input)))
		return false;
	
	list_node n = list_get_last_node(fcc->in);
	if (n == nullptr) return false;
	struct fc_input *innd = n->data;
	if (innd == nullptr) goto _error_out;

	if (!fc_input_register(innd, name)) goto _error_out;
	
	return true;

_error_out:

	list_pop(fcc->in);

	return false;
}

bool fc_controller_decrease_a_input(const struct fc_controller* const fcc, const char* name)
{
	if (fcc == nullptr) return false;
	if (fcc->in == nullptr) return false;

	if (name == nullptr)
		return list_pop_if(fcc->in, nullptr, list_pred_true, (list_node_data_deconstruct_cb)__fc_controller_input_deconstruct_cb);
	else
		return list_remove_if(fcc->in, (void*)name, (list_pred)_fc_controller_input_name_is, (list_node_data_deconstruct_cb)__fc_controller_input_deconstruct_cb);
}

bool fc_controller_delete_all_input(const struct fc_controller* const fcc)
{
	if (fcc == nullptr) return false;
	if (fcc->in == nullptr) return false;

	return list_clear(fcc->in, (list_node_data_deconstruct_cb)__fc_controller_input_deconstruct_cb);
}

bool fc_controller_print_input_list(const struct fc_controller* const fcc, const char* label)
{
	if (fcc == nullptr) return false;
	if (fcc->in == nullptr) return false;

	__FC_CONTROLLER_PRINTF("%s: \r\n", label ? label : "(unset label)");

	list_node n = list_get_first_node(fcc->in);
	while (n != nullptr)
	{
		__FC_CONTROLLER_PRINTF("%s\r\n", ((struct fc_input*)(n->data))->name);

		n = list_find_next_node(fcc->in, n);
	}

	return true;
}

bool fc_controller_increase_a_output(const struct fc_controller* const fcc, const char* name, accurate_number min, accurate_number max, accurate_number step)
{
	if (fcc == nullptr) return false;
	if (fcc->out == nullptr) return false;
	if (name == nullptr) return false;

	struct fc_output out;
	if (!list_push(fcc->out, (void*)&out, sizeof(struct fc_output)))
		return false;

	list_node n = list_get_last_node(fcc->out);
	if (n == nullptr) return false;
	struct fc_output* outnd = n->data;
	if (outnd == nullptr) goto _error_out;

	if (!fc_output_register(outnd, name, min, max, step)) goto _error_out;

	return true;

_error_out:

	list_pop(fcc->in);

	return false;
}

bool fc_controller_decrease_a_output(const struct fc_controller* const fcc, const char* name)
{
	if (fcc == nullptr) return false;
	if (fcc->out == nullptr) return false;

	if (name == nullptr)
		return list_pop_if(fcc->out, nullptr, list_pred_true, (list_node_data_deconstruct_cb)__fc_controller_output_deconstruct_cb);
	else
		return list_remove_if(fcc->out, (void*)name, (list_pred)_fc_controller_output_name_is, (list_node_data_deconstruct_cb)__fc_controller_output_deconstruct_cb);
}

bool fc_controller_delete_all_output(const struct fc_controller* const fcc)
{
	if (fcc == nullptr) return false;
	if (fcc->out == nullptr) return false;

	return list_clear(fcc->out, (list_node_data_deconstruct_cb)__fc_controller_output_deconstruct_cb);
}

bool fc_controller_print_output_list(const struct fc_controller* const fcc, const char* label)
{
	if (fcc == nullptr) return false;
	if (fcc->out == nullptr) return false;

	__FC_CONTROLLER_PRINTF("%s: \r\n", label ? label : "(unset label)");

	list_node n = list_get_first_node(fcc->out);
	while (n != nullptr)
	{
		__FC_CONTROLLER_PRINTF("%s\r\n", ((struct fc_output*)(n->data))->name);

		n = list_find_next_node(fcc->out, n);
	}

	return true;
}

bool fc_controller_add_fuzzy_set_for_input(const struct fc_controller* const fcc, const char* name, const char* tag, fc_membership_fn ms)
{
	if (fcc == nullptr) return false;
	if (fcc->in == nullptr) return false;
	if (name == nullptr) return false;
	if (tag == nullptr || ms == nullptr) return false;

	struct fuzzy_set set = {
		.label = tag,
		.ms = ms
	};

	list_node n = list_find_if(fcc->in, (void*)name, (list_pred)_fc_controller_input_name_is);
	if (n == nullptr) return false;
	if (n->data == nullptr) return false;

	return fc_input_add_fuzzy_set((struct fc_input*)(n->data), &set);
}

bool fc_controller_clear_fuzzy_set_for_input(const struct fc_controller* const fcc, const char* name)
{
	if (fcc == nullptr) return false;
	if (fcc->in == nullptr) return false;
	if (name == nullptr) return false;

	list_node n = list_find_if(fcc->in, (void*)name, (list_pred)_fc_controller_input_name_is);
	if (n == nullptr) return false;
	if (n->data == nullptr) return false;

	return fc_input_clear_fuzzy_set((struct fc_input*)(n->data));
}

bool fc_controller_print_input_fuzzy_set(const struct fc_controller* const fcc, const char* name, const char* label)
{
	if (fcc == nullptr) return false;
	if (fcc->in == nullptr) return false;
	if (name == nullptr) return false;

	list_node n = list_find_if(fcc->in, (void*)name, (list_pred)_fc_controller_input_name_is);
	if (n == nullptr) return false;
	if (n->data == nullptr) return false;

	return fc_input_print_fuzzy_set((struct fc_input*)(n->data), label);
}

bool fc_controller_add_fuzzy_set_for_output(const struct fc_controller* const fcc, const char* name, const char* tag, fc_membership_fn ms)
{
	if (fcc == nullptr) return false;
	if (fcc->out == nullptr) return false;
	if (name == nullptr) return false;
	if (tag == nullptr || ms == nullptr) return false;

	struct fuzzy_set set = {
		.label = tag,
		.ms = ms
	};

	list_node n = list_find_if(fcc->out, (void*)name, (list_pred)_fc_controller_output_name_is);
	if (n == nullptr) return false;
	if (n->data == nullptr) return false;

	return fc_output_add_fuzzy_set((struct fc_output*)(n->data), &set);
}

bool fc_controller_clear_fuzzy_set_for_output(const struct fc_controller* const fcc, const char* name)
{
	if (fcc == nullptr) return false;
	if (fcc->out == nullptr) return false;
	if (name == nullptr) return false;

	list_node n = list_find_if(fcc->out, (void*)name, (list_pred)_fc_controller_output_name_is);
	if (n == nullptr) return false;
	if (n->data == nullptr) return false;

	return fc_output_clear_fuzzy_set((struct fc_output*)(n->data));
}

bool fc_controller_print_output_fuzzy_set(const struct fc_controller* const fcc, const char* name, const char* label)
{
	if (fcc == nullptr) return false;
	if (fcc->out == nullptr) return false;
	if (name == nullptr) return false;

	list_node n = list_find_if(fcc->out, (void*)name, (list_pred)_fc_controller_output_name_is);
	if (n == nullptr) return false;
	if (n->data == nullptr) return false;

	return fc_output_print_fuzzy_set((struct fc_output*)(n->data), label);
}

bool fc_controller_add_rule(const struct fc_controller* const fcc, fc_rule_item rule)
{
	if (fcc == nullptr) return false;
	if (fcc->rules == nullptr) return false;

	return fc_rules_add_rule(fcc->rules, rule);
}

bool fc_controller_clear_rule(const struct fc_controller* const fcc)
{
	if (fcc == nullptr) return false;
	if (fcc->rules == nullptr) return false;

	return fc_rules_clear_rule(fcc->rules);
}

bool fc_controller_print_rule(const struct fc_controller* const fcc, const char* label)
{
	if (fcc == nullptr) return false;
	if (fcc->rules == nullptr) return false;

	return fc_rules_print_rule(fcc->rules, label);
}

bool fc_controller_reasoning(const struct fc_controller* const fcc, const struct tag_data* in, fc_size in_num, struct tag_data* out, fc_size out_num)
{
	if (fcc == nullptr) return false;
	if (fcc->in == nullptr || fcc->out == nullptr || fcc->rules == nullptr) return false;
	if (list_length(fcc->in) <= 0 || list_length(fcc->out) <= 0 || fc_rules_get_rule_num(fcc->rules) <= 0) return false;
	if (in == nullptr || out == nullptr) return false;
	if (in_num <= 0 || out_num <= 0) return false;

	bool is_success = true;

	// 1. input value fuzzy
	for (fc_size i = 0; i < in_num; i++)
	{
		list_node n = list_find_if(fcc->in, (void*)(in[i].tag), (list_pred)_fc_controller_input_name_is);
		if (n == nullptr) goto _error_out;
		if (n->data == nullptr) goto _error_out;

		if (!fc_input_fuzzing((struct fc_input*)(n->data), (accurate_number*)&(in[i].data), 1)) goto _error_out;
	}

	// 2. reasoning
	struct inference_result ir = { "", 0 };
	struct fc_calculation cal;
	if (!fc_rules_create_calculation(&cal)) goto _error_out;
	const fc_size rule_num = fc_rules_get_rule_num(fcc->rules);
	for (fc_size i = 0; i < rule_num; i++)
	{
		// 2.1 rules export calculation
		if (!fc_rules_export_calculation(&cal, fcc->rules, i)) goto _error_out;

		// 2.2 calculate activate value
		ir.name_tag = ""; ir.activate = 0;
		list_node n = list_get_first_node(cal.condition);
		while (n != nullptr)
		{
			struct __fc_calculation_unit* cond = n->data;
			if (cond == nullptr) goto _error_out;

			// get fuzzy value
			char* context = nullptr;
			const char* label = __FC_CONTROLLER_STRTOK_S((char*)(cond->cr), "-", &context);  // input name
			list_node inn = list_find_if(fcc->in, (void*)label, (list_pred)_fc_controller_input_name_is);
			if (inn == nullptr || inn->data == nullptr) goto _error_out;
			label = __FC_CONTROLLER_STRTOK_S(nullptr, "", &context);  // input tag
			fuzzy_number ms = fc_input_get_fuzzy_number_by_label((struct fc_input*)(inn->data), label, 0);

			// fuzzy opera
			switch (cond->opera)
			{
			case OPERA_AND:
				ir.activate = __FC_CORE_OPERA_INT(ir.activate, ms);
				break;
			case OPERA_OR:  // the 1st node of cal.condition, is OPERA_OR
				ir.activate = __FC_CORE_OPERA_UNI(ir.activate, ms);
				break;
			case OPERA_VB:
				break;
			}

			n = list_find_next_node(cal.condition, n);
		}

		// 2.3 import activate value
		n = list_get_first_node(cal.result);
		while (n != nullptr)
		{
			struct __fc_calculation_unit* cond = n->data;
			if (cond == nullptr) goto _error_out;

			char* context = nullptr;
			const char* label = __FC_CONTROLLER_STRTOK_S((char*)(cond->cr), "-", &context);  // output name
			list_node outn = list_find_if(fcc->out, (void*)label, (list_pred)_fc_controller_output_name_is);
			if (outn == nullptr || outn->data == nullptr) goto _error_out;
			label = __FC_CONTROLLER_STRTOK_S(nullptr, "", &context);  // output tag
			if (!fc_output_increase_a_inference_result_group((struct fc_output*)(outn->data))) goto _error_out;
			ir.name_tag = label;
			if (!fc_output_add_inference_result((struct fc_output*)(outn->data), 0, &ir)) goto _error_out;

			n = list_find_next_node(cal.result, n);
		}
	}
	
	// 3. unfuzzy data and output
	for (fc_size i = 0; i < out_num; i++)
	{
		list_node n = list_find_if(fcc->out, (void*)(out[i].tag), (list_pred)_fc_controller_output_name_is);
		if (n == nullptr || n->data == nullptr) goto _error_out;
		fc_output_unfuzzing((struct fc_output*)(n->data), 0, 1, (accurate_number*)&(out[i].data), FC_OUTPUT_AREA_CENTER_METHOD);
	}

	if (0)
	{
	_error_out:
		is_success = false;
	}

	// recycle
	for (fc_size i = 0; i < in_num; i++)
	{
		list_node n = list_find_if(fcc->in, (void*)(in[i].tag), (list_pred)_fc_controller_input_name_is);
		if (n == nullptr) break;
		if (n->data == nullptr) break;

		fuzzy_matrix_delete(&(((struct fc_input*)(n->data))->data));
	}

	fc_rules_delete_calculation(&cal);

	list_node n = list_get_first_node(fcc->out);
	while (n != nullptr)
	{
		fc_output_delete_all_inference_result_group((struct fc_output*)(n->data));

		n = list_find_next_node(fcc->out, n);
	}

	return is_success;
}
