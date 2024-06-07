#include <vld.h>

#include <stdio.h>

#include "fc.h"

static struct fc_input in;

static fuzzy_number score_failed(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value >= 60) return 0;
	else return 1;
}

static fuzzy_number score_pass(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 60) return 0;
	else return 1;
}

static fuzzy_number score_good(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 70) return 0;

	if (value < 80) return (value - 70) / 10;
	else return 1;
}

static fuzzy_number score_excellent(accurate_number value)
{
	if (value < 0 || value > 100) return FUZZY_DATA_ILLEGAL_VALUE;

	if (value < 90) return 0;

	return (value - 90) / 10;
}

int main()
{
	struct fuzzy_set set = { "", nullptr };

	fc_input_register(&in, "score");

	set.label = "failed";
	set.ms = score_failed;
	fc_input_add_fuzzy_set(&in, &set);
	set.label = "pass";
	set.ms = score_pass;
	fc_input_add_fuzzy_set(&in, &set);
	set.label = "good";
	set.ms = score_good;
	fc_input_add_fuzzy_set(&in, &set);
	set.label = "excellent";
	set.ms = score_excellent;
	fc_input_add_fuzzy_set(&in, &set);

	__FC_RULES_PRINTF("\r\n");
	fc_input_print_fuzzy_set(&in, in.name);
	__FC_RULES_PRINTF("\r\n");

	__FC_RULES_PRINTF("\r\n");
	accurate_number data[8] = { 50, 60, 70, 80, 90, 95, 98, 100 };
	fc_input_fuzzing(&in, data, 8);
	fc_input_print_data(&in, in.name);
	__FC_RULES_PRINTF("\r\n");

	fc_input_unregister(&in);

	return 0;
}
