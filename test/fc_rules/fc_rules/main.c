#include "fc.h"

#include "vld.h"

static struct fc_rules rules;
static struct fc_calculation cal;

int main()
{
	fc_rules_register(&rules);
	fc_rules_create_calculation(&cal);

	fc_rules_add_rule(&rules, "IF Temp-Warm AND Weather-Clear THEN Mood-Happy");
	fc_rules_add_rule(&rules, "IF Temp-Cold AND Weather-Rain THEN Mood-Happy");
	fc_rules_add_rule(&rules, "IF Temp-Warm AND Weather-Rain THEN Mood-Depressed");
	fc_rules_print_rule(&rules, nullptr);

	fc_rules_export_calculation(&cal, &rules, 0);
	fc_rules_print_calculation(&cal, "0");
	fc_rules_export_calculation(&cal, &rules, 1);
	fc_rules_print_calculation(&cal, "1");
	fc_rules_export_calculation(&cal, &rules, 2);
	fc_rules_print_calculation(&cal, "2");

	fc_rules_unregister(&rules);
	fc_rules_delete_calculation(&cal);

	return 0;
}
