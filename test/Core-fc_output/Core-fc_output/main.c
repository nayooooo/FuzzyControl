/*****************************************************************//**
 * \file   main.c
 * \brief  
 * \encode GB 2312
 * 
 * \author YEWAN
 * \date   July 2024
 *********************************************************************/

#include <vld.h>

#include <stdio.h>

#include "fc.h"

static struct fc_rules r;
static struct fc_calculation cal;

static struct fc_input in1, in2;
static struct fc_output out;

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

static void __fc_input_add_fuzzy_set(const struct fc_input* const in)
{
	if (in == nullptr) return;

	struct fuzzy_set set = { "", nullptr };
	set.label = "failed";
	set.ms = score_failed;
	fc_input_add_fuzzy_set(in, &set);
	set.label = "pass";
	set.ms = score_pass;
	fc_input_add_fuzzy_set(in, &set);
	set.label = "good";
	set.ms = score_good;
	fc_input_add_fuzzy_set(in, &set);
	set.label = "excellent";
	set.ms = score_excellent;
	fc_input_add_fuzzy_set(in, &set);
}

int main()
{
	fc_rule_item rule = "";

	// 得到一个计算式
	fc_rules_register(&r);
	rule = "IF A-pass AND B-pass THEN U-1";
	fc_rules_add_rule(&r, rule);
	fc_rules_create_calculation(&cal);
	fc_rules_export_calculation(&cal, &r, 0);
	fc_rules_print_calculation(&cal, "calculation");

	// 注册输入部件并添加必要的隶属函数
	fc_input_register(&in1, "A");
	fc_input_register(&in2, "B");
	__fc_input_add_fuzzy_set(&in1);
	__fc_input_add_fuzzy_set(&in2);
	
	fc_input_unregister(&in1);
	fc_input_unregister(&in2);
	fc_rules_delete_calculation(&cal);
	fc_rules_unregister(&r);

	return 0;
}
