#include "fc.h"

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

int main()
{
	fc_input_register(&in);

	fc_input_add_membership_fn(&in, score_failed);
	fc_input_add_membership_fn(&in, score_pass);
	fc_input_add_membership_fn(&in, score_good);
	fc_input_add_membership_fn(&in, score_excellent);

	for (accurate_number score = 0; score <= 100; score++)
	{
		printf("\r\n\nscore=%.2f\r\n", score);
		fc_input_fuzzing(&in, score);
		fc_input_print_membership_vector(&in);
	}

	fc_input_unregister(&in);

	printf("Done!\r\n");

	return 0;
}
