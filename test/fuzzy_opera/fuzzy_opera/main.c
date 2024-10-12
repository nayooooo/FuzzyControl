#include <stdio.h>

#include "fuzzy_opera.h"

#include "vld.h"

#ifndef nullptr
#   define nullptr NULL
#endif // !nullptr


int main()
{
    struct fuzzy_matrix mat;
    struct fuzzy_matrix mat2;
    struct fuzzy_matrix mat3;
    fuzzy_matrix_init(&mat);
    fuzzy_matrix_init(&mat2);
    fuzzy_matrix_init(&mat3);

    printf("\r\n\n");
    fuzzy_matrix_create(&mat, 3, 4);
    fuzzy_matrix_set_elem(&mat, 0.1145);
    fuzzy_matrix_print(&mat, "[mat] ori");

    printf("\r\n\n");
    fuzzy_matrix_reshape(&mat, 2, 2);
    fuzzy_matrix_print(&mat, "[mat] reshape to (2,2)");
    fuzzy_matrix_reshape_s(&mat, 5, 5);
    fuzzy_matrix_print(&mat, "[mat] reshape_s to (5,5)");


    printf("\r\n\n");
    fuzzy_matrix_copy(&mat2, &mat);
    fuzzy_matrix_print(&mat2, "[mat2] copy mat");

    printf("\r\n\n");
    fuzzy_matrix_delete(&mat);
    fuzzy_matrix_delete(&mat2);
    fuzzy_matrix_create(&mat, 2, 3);
    for (fuzzy_size r = 0; r < mat.row; r++)
    {
        for (fuzzy_size c = 0; c < mat.col; c++)
        {
            mat.mat[r][c] = r * mat.col + c + 1;
        }
    }
    fuzzy_matrix_print(&mat, "[mat] ori");
    fuzzy_matrix_create(&mat2, 2, 2);
    fuzzy_matrix_set_elem(&mat2, 1);
    fuzzy_matrix_print(&mat2, "[mat2] ori");
    fuzzy_matrix_horzcat(&mat3, &mat, &mat2);
    fuzzy_matrix_print(&mat3, "[mat3] horzcat mat and mat2");

    printf("\r\n\n");
    fuzzy_matrix_print(&mat, "[mat] ori");
    fuzzy_opera_transpose(&mat, nullptr);
    fuzzy_matrix_print(&mat, "[mat] trans");

    printf("\r\n\n");
    fuzzy_matrix_delete(&mat);
    fuzzy_matrix_delete(&mat2);
    fuzzy_matrix_create(&mat, 2, 2);
    for (fuzzy_size r = 0; r < mat.row; r++)
    {
        for (fuzzy_size c = 0; c < mat.col; c++)
        {
            mat.mat[r][c] = r * mat.col + c + 1;
        }
    }
    fuzzy_matrix_create(&mat2, 2, 4);
    for (fuzzy_size r = 0; r < mat2.row; r++)
    {
        for (fuzzy_size c = 0; c < mat2.col; c++)
        {
            mat2.mat[r][c] = r * mat2.col + c + 1;
        }
    }
    fuzzy_matrix_print(&mat, "[mat] ori");
    fuzzy_matrix_print(&mat2, "[mat2] ori");
    bool ret = fuzzy_opera_dir_pro(&mat3, &mat, &mat2);
    if (ret == false)
    {
        printf("dir_pro failed!\r\n");
    }
    else
    {
        fuzzy_matrix_print(&mat3, "[mat3] dir_pro");
    }
    fuzzy_matrix_delete(&mat2);
    fuzzy_matrix_create(&mat2, 1, 4);
    for (fuzzy_size r = 0; r < mat2.row; r++)
    {
        for (fuzzy_size c = 0; c < mat2.col; c++)
        {
            mat2.mat[r][c] = r * mat2.col + c + 1;
        }
    }
    fuzzy_matrix_print(&mat, "[mat] ori");
    fuzzy_matrix_print(&mat2, "[mat2] ori");
    fuzzy_opera_dir_pro(&mat3, &mat, &mat2);
    fuzzy_matrix_print(&mat3, "[mat3] dir_pro");

    fuzzy_matrix_delete(&mat);
    fuzzy_matrix_delete(&mat2);
    fuzzy_matrix_delete(&mat3);
    fuzzy_matrix_create(&mat, 1, 3);
    fuzzy_matrix_create(&mat2, 1, 2);
    mat.mat[0][0] = 0.2; mat.mat[0][1] = 0.8; mat.mat[0][2] = 0.5;
    mat2.mat[0][0] = 0.7892; mat2.mat[0][1] = 0.3213;
    fuzzy_matrix_print(&mat, "mat");
    fuzzy_matrix_print(&mat2, "mat2");
    fuzzy_opera_dir_pro(&mat3, &mat, &mat2);
    fuzzy_matrix_print(&mat3, "dir_pro");
    fuzzy_opera(&mat2, &mat, &mat3);
    fuzzy_matrix_print(&mat2, "fuzzy_opera");

fuzzy_matrix_over:
    printf("\r\n\n");
    fuzzy_matrix_delete(&mat);
    fuzzy_matrix_delete(&mat2);
    fuzzy_matrix_delete(&mat3);

	return 0;
}
