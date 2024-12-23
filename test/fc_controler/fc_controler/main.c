#include <stdio.h>
#include "fc.h"

#include <windows.h>

#include <vld.h>

static fuzzy_number __sludge_SD(accurate_number value)
{
	return fc_core_Triangle(value, -1, 0, 50, 1);
}

static fuzzy_number __sludge_MD(accurate_number value)
{
	return fc_core_Triangle(value, 0, 50, 100, 1);
}

static fuzzy_number __sludge_LD(accurate_number value)
{
	return fc_core_Triangle(value, 50, 100, 101, 1);
}

static fuzzy_number __grease_SG(accurate_number value)
{
	return fc_core_Triangle(value, -1, 0, 50, 1);
}

static fuzzy_number __grease_MG(accurate_number value)
{
	return fc_core_Triangle(value, 0, 50, 100, 1);
}

static fuzzy_number __grease_LG(accurate_number value)
{
	return fc_core_Triangle(value, 50, 100, 101, 1);
}

static fuzzy_number __time_VS(accurate_number value)
{
	return fc_core_Triangle(value, -1, 0, 10, 1);
}

static fuzzy_number __time_S(accurate_number value)
{
	return fc_core_Triangle(value, 0, 10, 25, 1);
}

static fuzzy_number __time_M(accurate_number value)
{
	return fc_core_Triangle(value, 10, 25, 40, 1);
}

static fuzzy_number __time_L(accurate_number value)
{
	return fc_core_Triangle(value, 25, 40, 60, 1);
}

static fuzzy_number __time_VL(accurate_number value)
{
	return fc_core_Triangle(value, 40, 60, 61, 1);
}

struct _cal_thread_param
{
	int row;
	int size;
	decimal_number* result;
};

long long progress = 0;
HANDLE hMutex;
DWORD startTime;
#define ACCURACY		1E-4f
#define ACCURACY_STR	"1E-4f"

static DWORD WINAPI cal_thread(LPVOID param)
{
	if (param == NULL) return 1;

	struct _cal_thread_param* result = param;

	struct fc_controler fcc;
	fc_controler_register(&fcc);

	fc_controler_increase_a_input(&fcc, "sludge");  // 污泥
	fc_controler_increase_a_input(&fcc, "grease");  // 油脂

	fc_controler_add_fuzzy_set_for_input(&fcc, "sludge", "SD", __sludge_SD);
	fc_controler_add_fuzzy_set_for_input(&fcc, "sludge", "MD", __sludge_MD);
	fc_controler_add_fuzzy_set_for_input(&fcc, "sludge", "LD", __sludge_LD);

	fc_controler_add_fuzzy_set_for_input(&fcc, "grease", "SG", __grease_SG);
	fc_controler_add_fuzzy_set_for_input(&fcc, "grease", "MG", __grease_MG);
	fc_controler_add_fuzzy_set_for_input(&fcc, "grease", "LG", __grease_LG);

	fc_controler_increase_a_output(&fcc, "time", 0, 60, ACCURACY);  // 洗衣时间

	fc_controler_add_fuzzy_set_for_output(&fcc, "time", "VS", __time_VS);
	fc_controler_add_fuzzy_set_for_output(&fcc, "time", "S", __time_S);
	fc_controler_add_fuzzy_set_for_output(&fcc, "time", "M", __time_M);
	fc_controler_add_fuzzy_set_for_output(&fcc, "time", "L", __time_L);
	fc_controler_add_fuzzy_set_for_output(&fcc, "time", "VL", __time_VL);

	fc_controler_add_rule(&fcc, "IF sludge-SD AND grease-SG THEN time-VS");
	fc_controler_add_rule(&fcc, "IF sludge-SD AND grease-MG THEN time-M");
	fc_controler_add_rule(&fcc, "IF sludge-SD AND grease-LG THEN time-L");
	fc_controler_add_rule(&fcc, "IF sludge-MD AND grease-SG THEN time-S");
	fc_controler_add_rule(&fcc, "IF sludge-MD AND grease-MG THEN time-M");
	fc_controler_add_rule(&fcc, "IF sludge-MD AND grease-LG THEN time-L");
	fc_controler_add_rule(&fcc, "IF sludge-LD AND grease-SG THEN time-M");
	fc_controler_add_rule(&fcc, "IF sludge-LD AND grease-MG THEN time-L");
	fc_controler_add_rule(&fcc, "IF sludge-LD AND grease-LG THEN time-VL");

	for (int i = 0; i < result->size; i++)
	{
		struct tag_data indata[2] = { { "sludge", result->row }, { "grease", i } };
		struct tag_data outdata[1] = { "time", 0 };
		DWORD milliseconds = GetTickCount64();
		fc_controler_reasoning(&fcc, indata, 2, outdata, 1);
		DWORD currentseconds = GetTickCount64();
		milliseconds = currentseconds - milliseconds;
		result->result[i] = outdata[0].data;

		WaitForSingleObject(hMutex, INFINITE);
		progress += 1;
		printf("\033[2K\r进度: %7.5f%% 数据点用时 %5.3lfs，预计还需 %llu h %llu min %llu s",
			(float)progress / (101 * 101) * 100,
			(float)milliseconds / 1000,
			((unsigned long long)(currentseconds - startTime) * ((unsigned long long)101 * 101) / progress / 1000) / 60 / 60,
			((unsigned long long)(currentseconds - startTime) * ((unsigned long long)101 * 101) / progress / 1000) % ((unsigned long long)60 * 60) / 60,
			((unsigned long long)(currentseconds - startTime) * ((unsigned long long)101 * 101) / progress / 1000) % ((unsigned long long)60 * 60) % 60);
		fflush(stdout);
		ReleaseMutex(hMutex);
	}

	fc_controler_unregister(&fcc);

	return 0;
}

int main(int argc, char* argv[])
{
	printf("\r\n================================================================================\r\n\r\n");

	char save_path[100] = "./../Matlab/data/" ACCURACY_STR "/building/time.bin";

	printf("will write to \"%s\"\n", save_path);
	FILE* file = fopen(save_path, "wb");
	if (file == nullptr)
	{
		perror("Error opening file");
		return 2;
	}

	hMutex = CreateMutex(NULL, FALSE, NULL);
	if (hMutex == NULL) {
		fprintf(stderr, "CreateMutex error: %d\n", GetLastError());
		return 1;
	}

	HANDLE threads[101];
	int row = 101;
	int col = 101;
	decimal_number result[101][101] = { 0 };
	struct _cal_thread_param param[101];
	startTime = GetTickCount64();
	for (int i = 0; i < row; i++)
	{
		param[i].row = i;
		param[i].size = col;
		param[i].result = &result[i][0];
		threads[i] = CreateThread(
			NULL,
			0,
			cal_thread,
			&param[i],
			0,
			NULL
		);
		if (threads[i] == NULL)
		{
			fprintf(stderr, "Failed to create thread %d, error %lu\n", i, GetLastError());
			return 1;
		}
	}
	printf("进度: %7.5f%%", (float)progress / (101 * 101) * 100);

	for (int i = 0; i <= 100; i++)
	{
		WaitForSingleObject(threads[i], INFINITE);  // 不要用于精度要求高的问题求解
		CloseHandle(threads[i]);
	}
	CloseHandle(hMutex);
	printf("\n");

	fwrite(&row, sizeof(row), 1, file);
	fwrite(&col, sizeof(col), 1, file);
	fwrite(&result[0][0], sizeof(result[0][0]), row * col, file);
	fclose(file);
	printf("ok!\n");

	printf("\r\n================================================================================\r\n\r\n");

	return 0;
}
