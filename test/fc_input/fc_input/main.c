#include "fc.h"

#include <math.h>

#include "vld.h"

bool flag;

static fuzzy_number in_set_Hot(accurate_number value)
{
	if (value > 35) return 1;
	return fc_core_Gaussian(value, 35, 4, 1);
}

static fuzzy_number in_set_Warm(accurate_number value)
{
	return fc_core_Gaussian(value, 25, 3.5, 1);
}

static fuzzy_number in_set_Cold(accurate_number value)
{
	if (value < 18) return 1;
	return fc_core_Gaussian(value, 18, 4, 1);
}

int main()
{
	struct fc_input in;
	struct fuzzy_set set = { "" };

	fc_input_register(&in, "Temperature");

	flag = true;
	while (1)
	{
		set.label = "Hot"; set.ms = in_set_Hot;
		fc_input_add_fuzzy_set(&in, &set);
		set.label = "Warm"; set.ms = in_set_Warm;
		fc_input_add_fuzzy_set(&in, &set);
		set.label = "Cold"; set.ms = in_set_Cold;
		fc_input_add_fuzzy_set(&in, &set);
		fc_input_print_fuzzy_set(&in, "add fuzzy sets");

		flag = !flag;
		if (flag) break;

		fc_input_clear_fuzzy_set(&in);
		fc_input_print_fuzzy_set(&in, "full clear");
	}

	accurate_number num;
	num = 15;
	fc_input_fuzzing(&in, &num, 1);
	fc_input_print_data(&in, "15¡æ");
	num = 27;
	fc_input_fuzzing(&in, &num, 1);
	fc_input_print_data(&in, "27¡æ");
	num = 30;
	fc_input_fuzzing(&in, &num, 1);
	fc_input_print_data(&in, "30¡æ");

	accurate_number an[8] = { 10, 14, 18, 22, 26, 30, 34, 38 };
	fuzzy_number fn[8] = { 0 };
	fc_input_fuzzing_by_label(&in, an, 8, fn, "Hot");
	fc_printf("Hot: \r\n");
	for (fc_size i = 0; i < 8; i++)
	{
		fc_printf("%.0f\t", an[i]);
	}
	fc_printf("\r\n");
	for (fc_size i = 0; i < 8; i++)
	{
		fc_printf("%.4f\t", fn[i]);
	}
	fc_printf("\r\n");
	fc_input_fuzzing_by_label(&in, an, 8, fn, "Warm");
	fc_printf("Warm: \r\n");
	for (fc_size i = 0; i < 8; i++)
	{
		fc_printf("%.0f\t", an[i]);
	}
	fc_printf("\r\n");
	for (fc_size i = 0; i < 8; i++)
	{
		fc_printf("%.4f\t", fn[i]);
	}
	fc_printf("\r\n");
	fc_input_fuzzing_by_label(&in, an, 8, fn, "Cold");
	fc_printf("Cold: \r\n");
	for (fc_size i = 0; i < 8; i++)
	{
		fc_printf("%.0f\t", an[i]);
	}
	fc_printf("\r\n");
	for (fc_size i = 0; i < 8; i++)
	{
		fc_printf("%.4f\t", fn[i]);
	}
	fc_printf("\r\n");

	fc_input_unregister(&in);

	return 0;
}
