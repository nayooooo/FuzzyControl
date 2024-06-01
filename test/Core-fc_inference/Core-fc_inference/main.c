#include <stdio.h>
#include "fc.h"

#include <vld.h>

static struct fc_input in1, in2, in3;
static struct fc_inference infer;

static fuzzy_number score_failed(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value >= 60) return 0;

	return (60 - value) / 60;
}

static fuzzy_number score_pass(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 60) return 0;
	if (value > 70) return 1;

	return (value - 60) / 10;
}

static fuzzy_number score_good(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 70 || value > 95) return 0;

	if (value < 80) return (value - 70) / 10;
	else return (95 - value) / 10;
}

static fuzzy_number score_excellent(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 90) return 0;

	return (value - 90) / 10;
}

int main()
{
	accurate_number num;

	fc_input_register(&in1, "A");
	fc_input_register(&in2, "B");
	fc_input_register(&in3, "C");
	fc_inference_register(&infer, "R");

	in1.add_membership_fn(&in1, score_failed, "failed");
	in1.add_membership_fn(&in1, score_pass, "pass");
	in1.add_membership_fn(&in1, score_good, "good");
	in1.add_membership_fn(&in1, score_excellent, "excellent");
	num = 79;
	in1.fuzzing(&in1, &num, 1);
	in1.print(&in1);

	in1.add_membership_fn(&in2, score_failed, "failed");
	in1.add_membership_fn(&in2, score_pass, "pass");
	in1.add_membership_fn(&in2, score_good, "good");
	in1.add_membership_fn(&in2, score_excellent, "excellent");
	num = 66;
	in1.fuzzing(&in2, &num, 1);
	in1.print(&in2);

	in1.add_membership_fn(&in3, score_failed, "failed");
	in1.add_membership_fn(&in3, score_pass, "pass");
	in1.add_membership_fn(&in3, score_good, "good");
	in1.add_membership_fn(&in3, score_excellent, "excellent");
	num = 96;
	in1.fuzzing(&in3, &num, 1);
	in1.print(&in3);

	struct fuzzy_matrix mat;
	list_head label = list_create();
	fuzzy_matrix_init(&mat);

	fc_input_get_fuzzy_data(&in1, &mat);
	fc_input_get_label(&in1, label);
	infer.add_fuzzy_input_data(&infer, &mat, fc_input_get_name(&in1), label);
	fc_input_get_fuzzy_data(&in2, &mat);
	fc_input_get_label(&in2, label);
	infer.add_fuzzy_input_data(&infer, &mat, fc_input_get_name(&in2), label);
	fc_input_get_fuzzy_data(&in3, &mat);
	fc_input_get_label(&in3, label);
	//infer.add_fuzzy_input_data(&infer, &mat, fc_input_get_name(&in3), label);

	infer.inference(&infer);
	infer.print(&infer);

	fuzzy_matrix_delete(&mat);
	list_delete(label, nullptr);

	in1.unregister_dev(&in1);
	in2.unregister_dev(&in2);
	in3.unregister_dev(&in3);
	infer.unregister_dev(&infer);

	//malloc(1);

	return 0;
}
