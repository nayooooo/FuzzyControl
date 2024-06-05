#include "fc.h"

#include <vld.h>

static struct fc_rules r;

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
	fc_rule_item rule = "IF A-1 THEN U-2";
	ret = is_fc_rules_legal_rule(rule, r.rule_keyword_table, r.rule_keyword_num);
	printf("rule(%s) is %s\r\n", rule, ret ? "legal" : "illegal");
	rule = "IF A-1 AND B-2 AND C-3 OR C-8 THEN U-8 | U-9";
	ret = is_fc_rules_legal_rule(rule, r.rule_keyword_table, r.rule_keyword_num);
	printf("rule(%s) is %s\r\n", rule, ret ? "legal" : "illegal");
	printf("\r\n");

	fc_rules_unregister(&r);

	return 0;
}
