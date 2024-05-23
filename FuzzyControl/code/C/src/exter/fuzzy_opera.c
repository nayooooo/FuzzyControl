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

/*==================================================================================
    Internal Fuzzy Operation API
==================================================================================*/

#include <math.h>

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
 * \return size_t digit
 */
static size_t get_digit(size_t num)
{
    size_t digit = 0;
    
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

bool fuzzy_matrix_create(struct fuzzy_matrix* mat, size_t row, size_t col)
{
    if (mat == nullptr) return false;
    if (row <= 0 || col <= 0) return false;

    // Uninitialized, matrix creation not allowed
    if (mat->mat != nullptr) return false;

    // Dynamic application matrix memory
    mat->mat = (fuzzy_number**)__fuzzy_matrix_malloc(row * sizeof(fuzzy_number*));
    if (mat->mat == nullptr) return false;
    for (size_t r = 0; r < row; r++)
    {
        mat->mat[r] = (fuzzy_number*)__fuzzy_matrix_malloc(col * sizeof(fuzzy_number));
        if (mat->mat[r] == nullptr)
        {
            for (size_t i = 0; i < r; i++)
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

bool fuzzy_matrix_delete(struct fuzzy_matrix* mat)
{
    if (mat == nullptr) return false;
    if (mat->mat == nullptr) return true;

    for (size_t r = 0; r < mat->row; r++)
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
    if (dst->mat == nullptr || src->mat == nullptr) return false;

    if (dst->row == src->row && dst->col == src->col)
    {
        goto skip_delete;
    }

    if (!fuzzy_matrix_delete(dst)) return false;
    if (!fuzzy_matrix_create(dst, src->row, src->col)) return false;

skip_delete:

    for (size_t r = 0; r < dst->row; r++)
    {
        for (size_t c = 0; c < dst->col; c++)
        {
            dst->mat[r][c] = src->mat[r][c];
        }
    }

    return true;
}

bool fuzzy_matrix_trav(struct fuzzy_matrix* mat, void* data, fuzzy_opera_event_cb event_cb)
{
    if (mat == nullptr) return false;
    if (mat->mat == nullptr) return false;
    if (event_cb == nullptr) return false;

    for (size_t r = 0; r < mat->row; r++)
    {
        for (size_t c = 0; c < mat->col; c++)
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
    if (mat->mat == nullptr) return;

    printf("\r\n");
    if (label != nullptr)
    {
        printf("%s: \r\n", label);
    }
    printf("\t\t");
    for (size_t c = 0; c < mat->col; c++)
    {
        size_t digit = get_digit(c) + 2;
        printf("[%u]", c);
        for (size_t n = 0; n < 2 - digit / 4; n++)
        {
            printf("\t");
        }
    }
    printf("\r\n");
    for (size_t r = 0; r < mat->row; r++)
    {
        size_t digit = get_digit(r) + 2;
        printf("[%u]", r);
        for (size_t n = 0; n < 2 - digit / 4; n++)
        {
            printf("\t");
        }
        for (size_t c = 0; c < mat->col; c++)
        {
            printf("%0.4f\t\t", mat->mat[r][c]);
        }
        printf("\r\n");
    }
}

/*==================================================================================
    Fuzzy operation API
==================================================================================*/

bool fuzzy_opera_trans(struct fuzzy_matrix* mv, struct fuzzy_matrix* mvt)
{
    if (mv == nullptr || mvt == nullptr) return false;
    if (mv->mat == nullptr) return false;

    // Determine if it is just right and appropriate
    if (mv->row == mvt->col && mv->col == mvt->row)
    {
        if (mvt->mat != nullptr)
        {
            goto skip_delete;
        }
    }

    // Create a suitable matrix
    if (!fuzzy_matrix_delete(mvt)) return false;
    if (!fuzzy_matrix_create(mvt, mv->col, mv->row)) return false;

skip_delete:

    // Start transpose
    for (size_t r = 0; r < mv->row; r++)
    {
        for (size_t c = 0; c < mv->col; c++)
        {
            mvt->mat[c][r] = mv->mat[r][c];
        }
    }

    return true;
}

bool fuzzy_opera_dir_pro(struct fuzzy_matrix* mat1, struct fuzzy_matrix* mat2, struct fuzzy_matrix* result)
{
    if (mat1 == nullptr || mat2 == nullptr || result == nullptr) return false;
    if (mat1->mat == nullptr || mat2->mat == nullptr) return false;

    // mat1 needs to be transposed. If mat1 is not a row vector, 
    // then the transpose of each row is treated as a column
    // and arranged in sequence as a column
    size_t result_row = mat1->row * mat1->col;
    size_t result_col = mat2->col;
    if (result->row == result_row && result->col == result_col)
    {
        if (result->mat != nullptr)
        {
            goto skip_delete;
        }
    }

    // Create a suitable matrix
    if (!fuzzy_matrix_delete(result)) return false;
    if (!fuzzy_matrix_create(result, result_row, result_col)) return false;

skip_delete:

    // Start direct product
    for (size_t r = 0; r < result_row; r++)
    {
        for (size_t c = 0; c < result_col; c++)
        {
            size_t left_r = r / mat1->col;
            size_t left_c = r % mat1->col;
            fuzzy_number res = __fuzzy_opera_int(mat1->mat[left_r][left_c], mat2->mat[0][c]);
            for (size_t n = 1; n < mat2->row; n++)
            {
                res = __fuzzy_opera_uni(res, __fuzzy_opera_int(mat1->mat[left_r][left_c], mat2->mat[n][c]));
            }
            result->mat[r][c] = res;
        }
    }

    return true;
}
