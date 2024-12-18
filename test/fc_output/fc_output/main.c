#include "fc.h"
#include <stdio.h>

#include <vld.h>

static struct fuzzy_set fs = { "", nullptr };
static struct inference_result ir = { "", 0 };

static struct fc_output out;

accurate_number result = 0;

static fuzzy_number __time_VS(accurate_number value)
{
	return fc_core_Triangle(value, -1, 10, 0, 1);
}

static fuzzy_number __time_S(accurate_number value)
{
	return fc_core_Triangle(value, 0, 25, 10, 1);
}

static fuzzy_number __time_M(accurate_number value)
{
	return fc_core_Triangle(value, 10, 40, 25, 1);
}

static fuzzy_number __time_L(accurate_number value)
{
	return fc_core_Triangle(value, 25, 60, 40, 1);
}

static fuzzy_number __time_VL(accurate_number value)
{
	return fc_core_Triangle(value, 40, 61, 60, 1);
}

int main()
{
	printf("\r\n================================================================================\r\n\r\n");

	fc_output_register(&out, "time", 0, 60, 1E-2f);

	fs.label = "VS"; fs.ms = __time_VS;
	fc_output_add_fuzzy_set(&out, &fs);
	fs.label = "S"; fs.ms = __time_S;
	fc_output_add_fuzzy_set(&out, &fs);
	fs.label = "M"; fs.ms = __time_M;
	fc_output_add_fuzzy_set(&out, &fs);
	fs.label = "L"; fs.ms = __time_L;
	fc_output_add_fuzzy_set(&out, &fs);
	fs.label = "VL"; fs.ms = __time_VL;
	fc_output_add_fuzzy_set(&out, &fs);

	fc_output_increase_a_inference_result_group(&out);
	ir.name_tag = "M"; ir.activate = 0.6f;
	fc_output_add_inference_result(&out, 0, &ir);
	ir.name_tag = "L"; ir.activate = 0.4f;
	fc_output_add_inference_result(&out, 0, &ir);
	ir.name_tag = "L"; ir.activate = 0.2f;
	fc_output_add_inference_result(&out, 0, &ir);
	ir.name_tag = "VL"; ir.activate = 0.2f;
	fc_output_add_inference_result(&out, 0, &ir);

	fc_output_unfuzzing(&out, 0, 1, &result, FC_OUTPUT_AREA_CENTER_METHOD);
	printf("result=%4.2f\r\n", result);

	printf("\r\n");
	fc_output_print_fuzzy_set(&out, "out fuzzy set");
	printf("\r\n");

	printf("\r\n");
	fc_output_print_inference_result(&out, 0, "out inference result");
	printf("\r\n");

	fc_output_unregister(&out);

	printf("\r\n================================================================================\r\n\r\n");

	return 0;
}
