#include <stdio.h>
#include <stdbool.h>
#include "fc.h"

int main()
{
	printf("\r\n");
	printf("================================================================\r\n");
	printf("    trans\r\n");
	printf("================================================================\r\n");
	float **a = nullptr, **aT = nullptr;
	a = (float**)malloc(3 * sizeof(float*));
	if (a == nullptr) goto out;
	for (size_t i = 0; i < 3; i++)
	{
		a[i] = (float*)malloc(4 * sizeof(float));
	}
	aT = (float**)malloc(4 * sizeof(float*));
	if (aT == nullptr) goto out;
	for (size_t i = 0; i < 4; i++)
	{
		aT[i] = (float*)malloc(3 * sizeof(float));
	}
	for (size_t c = 0; c < 4; c++)
	{
		for (size_t r = 0; r < 3; r++)
		{
			a[r][c] = c * 3 + r;
		}
	}
	printf("ori: \r\n");
	for (size_t c = 0; c < 4; c++)
	{
		for (size_t r = 0; r < 3; r++)
		{
			printf("%0.2f\t", a[r][c]);
		}
		printf("\r\n");
	}
	printf("trans: \r\n");
	fuzzy_opera_trans((fuzzy_number**)a, 3, 4, (fuzzy_number**)aT);
	for (size_t c = 0; c < 3; c++)
	{
		for (size_t r = 0; r < 4; r++)
		{
			printf("%0.2f\t", aT[r][c]);
		}
		printf("\r\n");
	}

out:
	if (a) { free(a); a = nullptr; }
	if (aT) { free(aT); aT = nullptr; }

	return 0;
}
