#include "fc_input.h"

static bool __fc_input_fuzzy_set_construct_cb(void* data)
{
	if (data == nullptr) return false;

	struct fuzzy_set* set = data;

	if (set->label == nullptr) return false;
	const char* label = (const char*)__FC_INPUT_MALLOC((__FC_INPUT_STRLEN(set->label) + 1) * sizeof(char));
	if (label == nullptr) return false;
	__FC_INPUT_STRCPY_S((char*)label, (__FC_INPUT_STRLEN(set->label) + 1) * sizeof(char), set->label);
	set->label = label;

	return true;
}

static bool __fc_input_fuzzy_set_deconstruct_cb(void* data)
{
	if (data == nullptr) return false;

	struct fuzzy_set* set = data;

	if (set->label == nullptr) return false;
	__FC_INPUT_FREE((void*)(set->label));
	set->label = nullptr;

	return true;
}

bool fc_input_register(struct fc_input* const in, const char* name)
{
	if (in == nullptr || name == nullptr) return false;

	in->name = (const char*)__FC_INPUT_MALLOC((__FC_INPUT_STRLEN(name) + 1) * sizeof(char));
	if (in->name == nullptr) return false;
	__FC_INPUT_STRCPY_S((char*)(in->name), (__FC_INPUT_STRLEN(name) + 1) * sizeof(char), name);
	fuzzy_matrix_init(&(in->data));
	in->fuzzy_set = list_create();
	if (in->fuzzy_set == nullptr)
	{
		__FC_INPUT_FREE((void*)(in->name));
		in->name = nullptr;
		return false;
	}

	return true;
}

bool fc_input_unregister(struct fc_input* const in)
{
	if (in == nullptr) return false;

	if (in->name != nullptr)
	{
		__FC_INPUT_FREE((void*)(in->name));
		in->name = nullptr;
	}

	fuzzy_matrix_delete(&(in->data));
	list_delete(in->fuzzy_set, __fc_input_fuzzy_set_deconstruct_cb);
	in->fuzzy_set = nullptr;

	return true;
}

bool fc_input_add_fuzzy_set(const struct fc_input* const in, const struct fuzzy_set* const set)
{
	if (in == nullptr || set == nullptr) return false;
	if (in->fuzzy_set == nullptr || set->label == nullptr || set->ms == nullptr) return false;

	if (!list_push_if(in->fuzzy_set, (void*)set, sizeof(struct fuzzy_set), list_pred_true, __fc_input_fuzzy_set_construct_cb))
		return false;

	return true;
}

bool fc_input_clear_fuzzy_set(const struct fc_input* const in)
{
	if (in == nullptr) return false;
	if (in->fuzzy_set == nullptr) return false;

	if (!list_clear(in->fuzzy_set, __fc_input_fuzzy_set_deconstruct_cb))
		return false;

	return true;
}

bool fc_input_fuzzing(struct fc_input* const in, accurate_number* data, fc_size num)
{
	if (in == nullptr || data == nullptr) return false;

	if (num <= 0) return false;
	list_size col = list_length(in->fuzzy_set);
	if (col <= 0) return false;

	fuzzy_matrix_delete(&(in->data));
	if (!fuzzy_matrix_create(&(in->data), num, col)) return false;

	for (fc_size r = 0; r < num; r++)
	{
		list_node n = list_get_first_node(in->fuzzy_set);
		if (n == nullptr) return false;
		for (fc_size c = 0; c < col; c++)
		{
			in->data.mat[r][c] = ((struct fuzzy_set*)(n->data))->ms(data[r]);
			n = list_find_next_node(in->fuzzy_set, n);
		}
	}

	return true;
}

fuzzy_number fc_input_get_fuzzy_number_by_label(const struct fc_input* const in, const char* label, fc_index row)
{
	if (in == nullptr) return FUZZY_DATA_ILLEGAL_VALUE;
	if (label == nullptr) return FUZZY_DATA_ILLEGAL_VALUE;
	if (row < 0 || (row + 1) > in->data.row) return FUZZY_DATA_ILLEGAL_VALUE;
	if (in->data.mat == nullptr || in->data.row <= 0 || in->data.col <= 0) return FUZZY_DATA_ILLEGAL_VALUE;

	fc_index len = (fc_index)list_length(in->fuzzy_set);
	for (fc_index i = 0; i < len; i++)
	{
		struct fuzzy_set *fs = list_get_node_data(in->fuzzy_set, i);
		if (fs == nullptr) return FUZZY_DATA_ILLEGAL_VALUE;
		if (!__FC_INPUT_STRCMP(fs->label, label)) return in->data.mat[row][i];
	}

	return FUZZY_DATA_ILLEGAL_VALUE;
}

bool fc_input_fuzzing_by_label(
	const struct fc_input* const in,
	accurate_number* data, fc_size num,
	fuzzy_number* fuzzy_data,
	const char* label
)
{
	if (in == nullptr || data == nullptr || fuzzy_data == nullptr) return false;
	if (num <= 0) return false;
	if (label == nullptr) return false;

	// find fuzzy set
	if (in->fuzzy_set == nullptr) return false;
	if (list_length(in->fuzzy_set) <= 0) return false;
	const struct fuzzy_set* fs = nullptr;
	list_node n = list_get_first_node(in->fuzzy_set);
	while (n != nullptr)
	{
		fs = n->data;
		if (fs == nullptr) return false;
		if (__FC_INPUT_STRCMP(label, fs->label) == 0)
			break;

		n = list_find_next_node(in->fuzzy_set, n);
	}
	if (n == nullptr || fs == nullptr) return false;

	if (fs->ms == nullptr) return false;
	for (fc_size i = 0; i < num; i++)
	{
		fuzzy_data[i] = fs->ms(data[i]);
	}

	return true;
}

bool fc_input_print_data(const struct fc_input* const in, const char* label)
{
	if (in == nullptr) return false;
	if (in->data.mat == nullptr) return false;

	if (label != nullptr && *label != '\0')
	{
		__FC_INPUT_PRINTF("%s: \r\n", label);
	}
	__FC_INPUT_PRINTF("%s\t", in->name);
	list_node n = list_get_first_node(in->fuzzy_set);
	for (fc_size i = list_length(in->fuzzy_set); i > 0; i--)
	{
		const char* label = ((struct fuzzy_set*)n->data)->label;
		__FC_INPUT_PRINTF(" %s\t", label);
		n = list_find_next_node(in->fuzzy_set, n);
	}

	fuzzy_matrix_print(&(in->data), "fuzzy data");

	return true;
}

bool fc_input_print_fuzzy_set(const struct fc_input* const in, const char* label)
{
	if (in == nullptr) return false;
	if (in->name == nullptr || in->fuzzy_set == nullptr) return false;

	__FC_INPUT_PRINTF("%s: \r\n", label ? label : "(unset label)");
	list_node n = list_get_first_node(in->fuzzy_set);
	if (n == nullptr)
	{
		__FC_INPUT_PRINTF("There is no fuzzy set!\r\n");
		return true;
	}
	while (n != nullptr)
	{
		__FC_INPUT_PRINTF("%s-%s\r\n", in->name, ((struct fuzzy_set*)(n->data))->label);

		n = list_find_next_node(in->fuzzy_set, n);
	}

	return true;
}
