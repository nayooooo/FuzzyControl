#include <stdio.h>
#include "fc.h"

#include <vld.h>

static struct fc_input in1, in2, in3;
static struct fc_inference infer;

int main()
{
	fc_input_register(&in1, "A");
	fc_input_register(&in2, "B");
	fc_input_register(&in3, "C");
	fc_inference_register(&infer, "R");

	in1.unregister_dev(&in1);
	in2.unregister_dev(&in2);
	in3.unregister_dev(&in3);
	infer.unregister_dev(&infer);

	return 0;
}
