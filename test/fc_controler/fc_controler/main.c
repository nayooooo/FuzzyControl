#include <stdio.h>
#include "fc.h"

#include <vld.h>

static struct fc_controler fcc;

int main()
{
	printf("\r\n================================================================================\r\n\r\n");

	fc_controler_register(&fcc);

	fc_controler_unregister(&fcc);

	printf("\r\n================================================================================\r\n\r\n");

	return 0;
}
