#include "fuzzy_opera.h"

#include <stdio.h>

#ifndef nullptr
#   define nullptr ((void*)0)
#endif // !nullptr

#include <stdlib.h>
#include <string.h>

#define __fuzzy_matrix_malloc malloc
#define __fuzzy_matrix_free free
#define __fuzzy_matrix_realloc realloc

#define __fuzzy_matrix_memset memset
#define __fuzzy_matrix_memcpy memcpy

/*==================================================================================
    Internal Fuzzy Operation API
==================================================================================*/

#include <math.h>

#ifndef min
#   define min(x, y) ((x<y) ? (x) : (y))
#endif

#define FLT_EPSILON      1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0

/**
 * @brief Determine whether two numbers are equal
 *
 * \param n1
 * \param n2
 * \return true equal
 * \return false no
 */
#define __is_fuzzy_number_eq(n1, n2)        (fabs(n1 - n2) < FLT_EPSILON)

 /**
  * @brief The union operation of a single fuzzy number
  *
  * @param uni1
  * @param uni2
  * @return fuzzy_number result
  */
#define __fuzzy_opera_uni(uni1, uni2)       (fmaxf(uni1, uni2))

  /**
   * @brief The intersection operation of a single fuzzy number
   *
   * @param int1
   * @param int2
   * @return fuzzy_number result
   */
#define __fuzzy_opera_int(int1, int2)       (fminf(int1, int2))

   /**
    * @brief The complement operation of a single fuzzy number
    *
    * @param com
    * @return fuzzy_number result
    */
#define __fuzzy_opera_com(com)              (1 - com)

/**
 * @brief Obtain the number of digits
 * 
 * \param num
 * \return fuzzy_size digit
 */
static fuzzy_size get_digit(fuzzy_size num)
{
    fuzzy_size digit = 0;
    
    do
    {
        num /= 10;
        digit++;
    } while (num);

    return digit;
}

/*==================================================================================
    Fuzzy Matrix API
==================================================================================*/

bool fuzzy_matrix_init(struct fuzzy_matrix* mat)
{
    if (mat == nullptr) return false;

    mat->mat = nullptr;
    mat->row = 0;
    mat->col = 0;

    return true;
}

bool fuzzy_matrix_create(struct fuzzy_matrix* mat, fuzzy_size row, fuzzy_size col)
{
    if (mat == nullptr) return false;
    if (row <= 0 || col <= 0) return false;

    // Uninitialized, matrix creation not allowed
    if (__is_fuzzy_matrix_created(mat)) return false;

    // Dynamic application matrix memory
    mat->mat = (fuzzy_number**)__fuzzy_matrix_malloc(row * sizeof(fuzzy_number*));
    if (mat->mat == nullptr) return false;
    for (fuzzy_size r = 0; r < row; r++)
    {
        mat->mat[r] = (fuzzy_number*)__fuzzy_matrix_malloc(col * sizeof(fuzzy_number));
        if (mat->mat[r] == nullptr)
        {
            for (fuzzy_size i = 0; i < r; i++)
            {
                __fuzzy_matrix_free(mat->mat[i]);
                mat->mat[i] = nullptr;
            }
            __fuzzy_matrix_free(mat->mat);
            mat->mat = nullptr;
            return false;
        }
        __fuzzy_matrix_memset(mat->mat[r], 0, col * sizeof(fuzzy_number));
    }
    mat->row = row;
    mat->col = col;

    return true;
}

bool fuzzy_matrix_reshape(struct fuzzy_matrix* mat, fuzzy_size row, fuzzy_size col)
{
    if (mat == nullptr) return false;
    if (row <= 0 || col <= 0) return false;
    if (__is_fuzzy_matrix_damaged(mat)) return false;

    if (mat->row == row && mat->col == col) return true;

    fuzzy_size ori_row = mat->row;  // Number of rows in the original matrix
    fuzzy_size ori_col = mat->col;  // Number of columns in the original matrix

    // The number of rows in the new matrix is not equal to the number of rows
    // in the original matrix, and a new application needs to be made
    {
        void* temp = nullptr;
        struct fuzzy_matrix mark;

        // Reduce the number of rows in the new matrix
        if (row < ori_row)
        {
            // Record the row vectors to be destroyed
            fuzzy_matrix_init(&mark);
            if (!fuzzy_matrix_create(&mark, ori_row - row, 1)) return false;
            __fuzzy_matrix_memcpy(mark.mat, &(mat->mat[row]), ori_row - row);
        }

        // Applying for row vector pointers for a new matrix
        temp = __fuzzy_matrix_realloc(mat->mat, row * sizeof(fuzzy_number*));
        if (temp == nullptr)
        {
            return false;
        }
        mat->mat = temp;
        mat->row = row;

        if (row > ori_row)
        {
            __fuzzy_matrix_memset(&(mat->mat[ori_row]), 0, (row - ori_row) * sizeof(fuzzy_number*));
        }

        // Reduce the number of rows in the new matrix
        if (row < ori_row)
        {
            // Destroy unwanted row vectors
            for (fuzzy_size r = 0; r < ori_row - row; r++)
            {
                if (mark.mat[r] != nullptr)
                {
                    __fuzzy_matrix_free(mark.mat[r]);
                    mark.mat[r] = nullptr;
                }
            }

            // Destroy mark
            fuzzy_matrix_delete(&mark);
        }
    }

    // The number of columns in the new matrix is not equal to the number of rows
    // in the original matrix, and a new application is needed
    {
        void *temp = nullptr;

        for (fuzzy_size r = 0; r < mat->row; r++)
        {
            temp = __fuzzy_matrix_realloc(mat->mat[r], col * sizeof(fuzzy_number));
            if (temp == nullptr)
            {
                // Failed to realloc for matrix, marked as damaged matrix
                __set_fuzzy_matrix_as_damaged(mat);
                return false;
            }
            mat->mat[r] = temp;
        }

        mat->col = col;

        if (col > ori_col)
        {
            for (fuzzy_size r = 0; r < mat->row; r++)
            {
                __fuzzy_matrix_memset(&(mat->mat[r][ori_col]), 0, (col - ori_col) * sizeof(fuzzy_number));
            }
        }

        if (row > ori_row)
        {
            for (fuzzy_size r = ori_row; r < row; r++)
            {
                __fuzzy_matrix_memset(mat->mat[r], 0, col * sizeof(fuzzy_number));
            }
        }
    }

    return true;
}

bool fuzzy_matrix_reshape_s(struct fuzzy_matrix* mat, fuzzy_size row, fuzzy_size col)
{
    if (mat == nullptr) return false;
    if (row <= 0 || col <= 0) return false;
    if (__is_fuzzy_matrix_damaged(mat)) return false;

    if (mat->row == row && mat->col == col) return true;

    // Let the brave go and slaughter the dragon,
    // and then the king will snatch the credit of the brave
    struct fuzzy_matrix brave;
    fuzzy_matrix_init(&brave);
    if (!fuzzy_matrix_create(&brave, row, col)) return false;
    fuzzy_matrix_clear(&brave);
    fuzzy_matrix_copy_just_elem(&brave, mat);

    if (!fuzzy_matrix_delete(mat))
    {
        fuzzy_matrix_delete(&brave);
        return false;
    }

    fuzzy_matrix_pay_tribute(mat, &brave);

    return true;
}

bool fuzzy_matrix_clear(struct fuzzy_matrix* mat)
{
    if (mat == nullptr) return false;
    if (__is_fuzzy_matrix_damaged(mat)) return false;

    for (fuzzy_size r = 0; r < mat->row; r++)
    {
        __fuzzy_matrix_memset(mat->mat[r], 0, mat->col * sizeof(fuzzy_number));
    }

    return true;
}

bool fuzzy_matrix_pay_tribute(struct fuzzy_matrix* emperor, struct fuzzy_matrix* vassal)
{
    if (emperor == nullptr || vassal == nullptr) return false;
    if (__is_fuzzy_matrix_damaged(vassal)) return false;
    if (__is_fuzzy_matrix_created(emperor)) return false;
    if (!__is_fuzzy_matrix_created(vassal)) return false;

    __fuzzy_matrix_memcpy(emperor, vassal, sizeof(struct fuzzy_matrix));
    fuzzy_matrix_init(vassal);

    return true;
}

bool fuzzy_matrix_rob(struct fuzzy_matrix* king, struct fuzzy_matrix* brave)
{
    if (king == nullptr || brave == nullptr) return false;
    if (__is_fuzzy_matrix_damaged(brave)) goto king_failed;

    if (0)
    {
    king_failed:
        fuzzy_matrix_delete(king);
        return false;
    }

    fuzzy_matrix_copy(king, brave);
    fuzzy_matrix_delete(brave);

    return true;
}

bool fuzzy_matrix_delete(struct fuzzy_matrix* mat)
{
    if (mat == nullptr) return false;
    if (!__is_fuzzy_matrix_created(mat)) return true;

    for (fuzzy_size r = 0; r < mat->row; r++)
    {
        if (mat->mat[r] != nullptr)
        {
            __fuzzy_matrix_free(mat->mat[r]);
            mat->mat[r] = nullptr;
        }
    }

    __fuzzy_matrix_free(mat->mat);
    mat->mat = nullptr;

    mat->row = 0;
    mat->col = 0;

    return true;
}

bool fuzzy_matrix_copy(struct fuzzy_matrix* dst, const struct fuzzy_matrix* src)
{
    if (dst == nullptr || src == nullptr) return false;
    if (__is_fuzzy_matrix_damaged(src)) return false;
    if (!__is_fuzzy_matrix_created(src)) return false;

    if (dst->row == src->row && dst->col == src->col)
    {
        if (__is_fuzzy_matrix_created(dst))
        {
            goto skip_delete;
        }
    }

    if (!fuzzy_matrix_delete(dst)) return false;
    if (!fuzzy_matrix_create(dst, src->row, src->col)) return false;

skip_delete:

    for (fuzzy_size r = 0; r < dst->row; r++)
    {
        for (fuzzy_size c = 0; c < dst->col; c++)
        {
            dst->mat[r][c] = src->mat[r][c];
        }
    }

    return true;
}

bool fuzzy_matrix_copy_just_elem(struct fuzzy_matrix* dst, const struct fuzzy_matrix* src)
{
    if (dst == nullptr || src == nullptr) return nullptr;
    if (__is_fuzzy_matrix_damaged(dst)) return false;
    if (__is_fuzzy_matrix_damaged(src)) return false;
    if ((!__is_fuzzy_matrix_created(dst)) || (!__is_fuzzy_matrix_created(src))) return false;

    fuzzy_size min_row = min(dst->row, src->row);
    fuzzy_size min_col = min(dst->col, src->col);

    for (fuzzy_size r = 0; r < min_row; r++)
    {
        for (fuzzy_size c = 0; c < min_col; c++)
        {
            dst->mat[r][c] = src->mat[r][c];
        }
    }

    return true;
}

bool fuzzy_matrix_trav(struct fuzzy_matrix* mat, void* data, fuzzy_opera_event_cb event_cb)
{
    if (mat == nullptr) return false;
    if (__is_fuzzy_matrix_damaged(mat)) return false;
    if (!__is_fuzzy_matrix_created(mat)) return false;
    if (event_cb == nullptr) return false;

    for (fuzzy_size r = 0; r < mat->row; r++)
    {
        for (fuzzy_size c = 0; c < mat->col; c++)
        {
            if (mat->mat[r] == nullptr) break;
            if (!event_cb(mat->mat[r][c], data)) return false;
        }
    }

    return true;
}

void fuzzy_matrix_print(struct fuzzy_matrix* mat, const char* label)
{
    if (mat == nullptr) return;
    if (__is_fuzzy_matrix_damaged(mat)) return;
    if (!__is_fuzzy_matrix_created(mat)) return;

    printf("\r\n");
    if (label != nullptr)
    {
        printf("%s: \r\n", label);
    }
    printf("\t");
    for (fuzzy_size c = 0; c < mat->col; c++)
    {
        fuzzy_size digit = get_digit(c) + 2;
        printf("   [%llu]", c);
        if ((2 - digit / 4) < 0) continue;
        for (fuzzy_size n = 0; n < 2 - digit / 4; n++)
        {
            printf("\t");
        }
    }
    printf("\r\n");
    for (fuzzy_size r = 0; r < mat->row; r++)
    {
        fuzzy_size digit = get_digit(r) + 2;
        printf("[%llu]", r);
        if ((2 - digit / 4) < 0) goto print_data_start;
        for (fuzzy_size n = 0; n < 1 - digit / 4; n++)
        {
            printf("\t");
        }
    print_data_start:
        for (fuzzy_size c = 0; c < mat->col; c++)
        {
            printf("%9.4f\t", mat->mat[r][c]);
        }
        printf("\r\n");
    }
}

/*==================================================================================
    Fuzzy operation API
==================================================================================*/

bool fuzzy_opera_trans(struct fuzzy_matrix* mat, struct fuzzy_matrix* matT)
{
    if (mat == nullptr || matT == nullptr) return false;
    if (__is_fuzzy_matrix_damaged(mat)) return false;
    if (!__is_fuzzy_matrix_created(mat)) return false;

    // Determine if it is just right and appropriate
    if (mat->row == matT->col && mat->col == matT->row)
    {
        if (__is_fuzzy_matrix_created(matT))
        {
            goto skip_delete;
        }
    }

    // Create a suitable matrix
    if (!fuzzy_matrix_delete(matT)) return false;
    if (!fuzzy_matrix_create(matT, mat->col, mat->row)) return false;

skip_delete:

    // Start transpose
    for (fuzzy_size r = 0; r < mat->row; r++)
    {
        for (fuzzy_size c = 0; c < mat->col; c++)
        {
            matT->mat[c][r] = mat->mat[r][c];
        }
    }

    return true;
}

bool fuzzy_opera_dir_pro(struct fuzzy_matrix* mat1, struct fuzzy_matrix* mat2, struct fuzzy_matrix* result)
{
    if (mat1 == nullptr || mat2 == nullptr || result == nullptr) return false;
    if (__is_fuzzy_matrix_damaged(mat1)) return false;
    if (__is_fuzzy_matrix_damaged(mat2)) return false;
    if ((!__is_fuzzy_matrix_created(mat1)) || (!__is_fuzzy_matrix_created(mat2))) return false;

    // mat1 needs to be transposed. If mat1 is not a row vector, 
    // then the transpose of each row is treated as a column
    // and arranged in sequence as a column
    fuzzy_size result_row = mat1->row * mat1->col;
    fuzzy_size result_col = mat2->col;
    if (result->row == result_row && result->col == result_col)
    {
        if (__is_fuzzy_matrix_created(result))
        {
            goto skip_delete;
        }
    }

    // Create a suitable matrix
    if (!fuzzy_matrix_delete(result)) return false;
    if (!fuzzy_matrix_create(result, result_row, result_col)) return false;

skip_delete:

    // Start direct product
    for (fuzzy_size r = 0; r < result_row; r++)
    {
        for (fuzzy_size c = 0; c < result_col; c++)
        {
            fuzzy_size left_r = r / mat1->col;
            fuzzy_size left_c = r % mat1->col;
            fuzzy_number res = __fuzzy_opera_int(mat1->mat[left_r][left_c], mat2->mat[0][c]);
            for (fuzzy_size n = 1; n < mat2->row; n++)
            {
                res = __fuzzy_opera_uni(res, __fuzzy_opera_int(mat1->mat[left_r][left_c], mat2->mat[n][c]));
            }
            result->mat[r][c] = res;
        }
    }

    return true;
}
