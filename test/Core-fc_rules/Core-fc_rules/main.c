#include <vld.h>

#include "fc.h"

static struct fc_rules r;
struct fc_calculation cal;

int main()
{
	bool ret = false;

	printf("\r\n");
	fc_rules_register(&r);
	printf("table p: 0x%X\r\n", r.rule_keyword_table);
	printf("table number: %d\r\n", r.rule_keyword_num);
	printf("rules: 0x%X\r\n", r.rules);
	printf("\r\n");

	printf("\r\n");
	printf("%s\r\n", (is_fc_fules_keyword("IF", r.rule_keyword_table, r.rule_keyword_num) >= 0) ? "true" : "false");
	printf("%s\r\n", (is_fc_fules_keyword("THEN", r.rule_keyword_table, r.rule_keyword_num) >= 0) ? "true" : "false");
	printf("%s\r\n", (is_fc_fules_keyword("AND", r.rule_keyword_table, r.rule_keyword_num) >= 0) ? "true" : "false");
	printf("%s\r\n", (is_fc_fules_keyword("OR", r.rule_keyword_table, r.rule_keyword_num) >= 0) ? "true" : "false");
	printf("%s\r\n", (is_fc_fules_keyword("?", r.rule_keyword_table, r.rule_keyword_num) >= 0) ? "true" : "false");
	printf("%s\r\n", (is_fc_fules_keyword(":", r.rule_keyword_table, r.rule_keyword_num) >= 0) ? "true" : "false");
	printf("\r\n");

	printf("\r\n");
	fc_rule_item rule;
	rule = "IF A-1 THEN U-2";
	ret = is_fc_rules_legal_rule(rule, r.rule_keyword_table, r.rule_keyword_num);
	printf("rule(%s) is %s\r\n", rule, ret ? "legal" : "illegal");
	rule = "IF A1 THEN U-2";
	ret = is_fc_rules_legal_rule(rule, r.rule_keyword_table, r.rule_keyword_num);
	printf("rule(%s) is %s\r\n", rule, ret ? "legal" : "illegal");
	rule = "IF A-1 AND B-2 OR C-3 THEN U-2";
	ret = is_fc_rules_legal_rule(rule, r.rule_keyword_table, r.rule_keyword_num);
	printf("rule(%s) is %s\r\n", rule, ret ? "legal" : "illegal");
	rule = "IF A-1 AND B-2 OR C-3 THEN U-2 |";
	ret = is_fc_rules_legal_rule(rule, r.rule_keyword_table, r.rule_keyword_num);
	printf("rule(%s) is %s\r\n", rule, ret ? "legal" : "illegal");
	rule = "IF A-1 AND B-2 OR C-3 THEN U-2 | V-1";
	ret = is_fc_rules_legal_rule(rule, r.rule_keyword_table, r.rule_keyword_num);
	printf("rule(%s) is %s\r\n", rule, ret ? "legal" : "illegal");
	rule = "IF A-1 AND B-2 OR C-3 THEN U-2 | U-4";
	ret = is_fc_rules_legal_rule(rule, r.rule_keyword_table, r.rule_keyword_num);
	printf("rule(%s) is %s\r\n", rule, ret ? "legal" : "illegal");
	rule = "IF A-1 AND B-2 OR C-3 THEN U-2";
	ret = is_fc_rules_legal_rule(rule, r.rule_keyword_table, r.rule_keyword_num);
	printf("rule(%s) is %s\r\n", rule, ret ? "legal" : "illegal");
	rule = "aaaaIF A-1 AND B-2 OR C-3 THEN U-2";
	ret = is_fc_rules_legal_rule(rule, r.rule_keyword_table, r.rule_keyword_num);
	printf("rule(%s) is %s\r\n", rule, ret ? "legal" : "illegal");
	printf("\r\n");

	printf("\r\n");
	rule = "IF A-1 THEN U-2";
	ret = fc_rules_add_rule(&r, rule);
	printf("[Add Rule] \"%s\" %s\r\n", rule, ret ? "success" : "failed");
	rule = "IF A1 THEN U-2";
	ret = fc_rules_add_rule(&r, rule);
	printf("[Add Rule] \"%s\" %s\r\n", rule, ret ? "success" : "failed");
	rule = "IF A-1 AND B-2 OR C-3 THEN U-2";
	ret = fc_rules_add_rule(&r, rule);
	printf("[Add Rule] \"%s\" %s\r\n", rule, ret ? "success" : "failed");
	rule = "IF A-1 AND B-2 OR C-3 THEN U-2 | V-7";
	ret = fc_rules_add_rule(&r, rule);
	printf("[Add Rule] \"%s\" %s\r\n", rule, ret ? "success" : "failed");
	printf("\r\n");

	printf("\r\n");
	fc_rules_print_rule(&r, "rules");
	printf("\r\n");

	fc_rules_create_calculation(&cal);
	fc_rules_export_calculation(&r, &cal, 2);
	fc_rules_print_calculation(&cal, "calculation");

	fc_rules_delete_calculation(&cal);
	fc_rules_unregister(&r);

	malloc(1);

	return 0;
}
