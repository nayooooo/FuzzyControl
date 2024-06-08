#include <vld.h>

#include <stdio.h>

#include "fc.h"

static struct fc_output out;

//static;

int main()
{
	struct fuzzy_set set;

	fc_output_register(&out, "U");

	set.label = "1";
	set.ms = nullptr;
	fc_output_add_fuzzy_set(&out, &set);

	fc_output_unregister(&out);

	return 0;
}
