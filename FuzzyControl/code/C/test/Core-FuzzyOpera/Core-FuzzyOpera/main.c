#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "fc.h"

static bool print_number(fuzzy_number fn, void* data)
{
	UNUSED(data);
	printf("%0.2f\t", fn);

	return true;
}

int main()
{
	srand(time(NULL));

	// fuzzy matrix
	printf("\r\n");
	printf("================================================================\r\n");
	printf("    fuzzy matrix\r\n");
	printf("================================================================\r\n");
	{
		struct fuzzy_matrix mat;
		fuzzy_matrix_init(&mat);
		fuzzy_matrix_create(&mat, 3, 4);

		printf("\r\n");
		printf("--------------------------------\r\n");
		printf("    trav\r\n");
		printf("--------------------------------\r\n");
		for (size_t r = 0; r < mat.row; r++)
		{
			for (size_t c = 0; c < mat.col; c++)
			{
				size_t base = r * mat.col + c;
				//mat.mat[r][c] = (float)(rand() % (100 * (base ? base : 1))) / 100;
				mat.mat[r][c] = base;
			}
		}
		fuzzy_matrix_trav(&mat, nullptr, print_number);

		printf("\r\n");
		printf("--------------------------------\r\n");
		printf("    trans\r\n");
		printf("--------------------------------\r\n");
		struct fuzzy_matrix matT;
		fuzzy_matrix_init(&matT);

		fuzzy_opera_trans(&mat, &matT);
		fuzzy_matrix_print(&mat, "mat");
		fuzzy_matrix_print(&matT, "matT");

		fuzzy_matrix_delete(&mat);
	}

	return 0;
}
