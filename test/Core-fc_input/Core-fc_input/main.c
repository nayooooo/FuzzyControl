#include <stdio.h>
#include "fc.h"

#include <vld.h>

struct fc_input in;

// 不及格
static fuzzy_number score_failed(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value >= 60) return 0;

	return (60 - value) / 60;
}

// 及格
static fuzzy_number score_pass(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 60) return 0;
	if (value > 70) return 1;

	return (value - 60) / 10;
}

// 良好
static fuzzy_number score_good(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 70 || value > 95) return 0;

	if (value < 80) return (value - 70) / 10;
	else return (95 - value) / 10;
}

// 优秀
static fuzzy_number score_excellent(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 90) return 0;

	return (value - 90) / 10;
}

static bool print_label(list_node node, void* data)
{
	UNUSED(data);
	if (node == nullptr) return false;

	printf("%s\r\n", (node->data));
}

int main()
{
	fc_input_register(&in, "A");

	in.add_membership_fn(&in, score_failed, "failed");
	in.add_membership_fn(&in, score_pass, "pass");
	in.add_membership_fn(&in, score_good, "good");
	in.add_membership_fn(&in, score_excellent, "excellent");

	/*for (accurate_number score = 0; score <= 100; score++)
	{
		printf("\r\n\nscore=%.2f\r\n", score);
		in.fuzzing(&in, &score, 1);
		in.print(&in);
	}*/

	accurate_number score[3] = { 59.9, 79.1, 98.6 };
	in.fuzzing(&in, score, 3);
	printf("%s: \r\n", fc_input_get_name(&in));
	in.print(&in);

	printf("\r\n");
	list_head label = list_create();
	fc_input_get_label(&in, label);
	list_trav(label, nullptr, print_label);

	struct fuzzy_matrix mat;
	fuzzy_matrix_init(&mat);
	fc_input_get_fuzzy_data(&in, &mat);
	fuzzy_matrix_print(&mat, "data");

	fuzzy_matrix_delete(&mat);
	list_delete(label, nullptr);
	in.unregister_dev(&in);

	printf("Done!\r\n");

	return 0;
}
