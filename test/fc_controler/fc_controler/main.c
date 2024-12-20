#include <stdio.h>
#include "fc.h"

#include <windows.h>

#include <vld.h>

static struct fc_controler fcc;

static fuzzy_number __sludge_SD(accurate_number value)
{
	return fc_core_Triangle(value, -1, 50, 0, 1);
}

static fuzzy_number __sludge_MD(accurate_number value)
{
	return fc_core_Triangle(value, 0, 100, 50, 1);
}

static fuzzy_number __sludge_LD(accurate_number value)
{
	return fc_core_Triangle(value, 50, 101, 100, 1);
}

static fuzzy_number __grease_SG(accurate_number value)
{
	return fc_core_Triangle(value, -1, 50, 0, 1);
}

static fuzzy_number __grease_MG(accurate_number value)
{
	return fc_core_Triangle(value, 0, 100, 50, 1);
}

static fuzzy_number __grease_LG(accurate_number value)
{
	return fc_core_Triangle(value, 50, 101, 100, 1);
}

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

	fc_controler_register(&fcc);

	printf("\r\n");
	fc_controler_increase_a_input(&fcc, "sludge");  // Œ€ƒ‡
	fc_controler_increase_a_input(&fcc, "grease");  // ”Õ÷¨
	fc_controler_print_input_list(&fcc, "input");
	printf("\r\n");

	printf("\r\n");
	fc_controler_add_fuzzy_set_for_input(&fcc, "sludge", "SD", __sludge_SD);
	fc_controler_add_fuzzy_set_for_input(&fcc, "sludge", "MD", __sludge_MD);
	fc_controler_add_fuzzy_set_for_input(&fcc, "sludge", "LD", __sludge_LD);
	fc_controler_print_input_fuzzy_set(&fcc, "sludge", "input-sludge");
	printf("\r\n");

	printf("\r\n");
	fc_controler_add_fuzzy_set_for_input(&fcc, "grease", "SG", __grease_SG);
	fc_controler_add_fuzzy_set_for_input(&fcc, "grease", "MG", __grease_MG);
	fc_controler_add_fuzzy_set_for_input(&fcc, "grease", "LG", __grease_LG);
	fc_controler_print_input_fuzzy_set(&fcc, "grease", "input-grease");
	printf("\r\n");

	printf("\r\n");
	fc_controler_increase_a_output(&fcc, "time", 0, 60, 1E-3f);  // œ¥“¬ ±º‰
	fc_controler_print_output_list(&fcc, "output");
	printf("\r\n");

	printf("\r\n");
	fc_controler_add_fuzzy_set_for_output(&fcc, "time", "VS", __time_VS);
	fc_controler_add_fuzzy_set_for_output(&fcc, "time", "S", __time_S);
	fc_controler_add_fuzzy_set_for_output(&fcc, "time", "M", __time_M);
	fc_controler_add_fuzzy_set_for_output(&fcc, "time", "L", __time_L);
	fc_controler_add_fuzzy_set_for_output(&fcc, "time", "VL", __time_VL);
	fc_controler_print_output_fuzzy_set(&fcc, "time", "output-time");
	printf("\r\n");

	printf("\r\n");
	fc_controler_add_rule(&fcc, "IF sludge-SD AND grease-SG THEN time-VS");
	fc_controler_add_rule(&fcc, "IF sludge-SD AND grease-MG THEN time-M");
	fc_controler_add_rule(&fcc, "IF sludge-SD AND grease-LG THEN time-L");
	fc_controler_add_rule(&fcc, "IF sludge-MD AND grease-SG THEN time-S");
	fc_controler_add_rule(&fcc, "IF sludge-MD AND grease-MG THEN time-M");
	fc_controler_add_rule(&fcc, "IF sludge-MD AND grease-LG THEN time-L");
	fc_controler_add_rule(&fcc, "IF sludge-LD AND grease-SG THEN time-M");
	fc_controler_add_rule(&fcc, "IF sludge-LD AND grease-MG THEN time-L");
	fc_controler_add_rule(&fcc, "IF sludge-LD AND grease-LG THEN time-VL");
	fc_controler_print_rule(&fcc, "rules");
	printf("\r\n");

	//printf("\r\n");
	//struct tag_data indata[2] = { { "sludge", 60 }, { "grease", 70 }};
	//struct tag_data outdata[1] = {"time", 0};
	//DWORD milliseconds = GetTickCount64();
	//fc_controler_reasoning(&fcc, indata, 2, outdata, 1);
	//milliseconds = GetTickCount64() - milliseconds;
	//printf("reasoning use time: %5.3lf s\r\n", (double)milliseconds / 1000);
	//printf("in: sludge=%4.2f, grease=%4.2f\r\n", indata[0].data, indata[1].data);
	//printf("out: time=%4.2f\r\n", outdata[0].data);
	//printf("\r\n");

	printf("\r\n");
	while (true)
	{
		for (int i = 0; i <= 100; i++)
		{
			for (int j = 0; j <= 100; j++)
			{
				struct tag_data indata[2] = { { "sludge", i }, { "grease", j } };
				struct tag_data outdata[1] = { "time", 0 };
				fc_controler_reasoning(&fcc, indata, 2, outdata, 1);
				printf("in: sludge=%4.2f, grease=%4.2f\r\n", indata[0].data, indata[1].data);
				printf("out: time=%4.2f\r\n", outdata[0].data);
				if ('\n' == getch()) goto _out;
			}
		}
	}
_out:
	printf("\r\n");

	fc_controler_unregister(&fcc);

	printf("\r\n================================================================================\r\n\r\n");

	return 0;
}
