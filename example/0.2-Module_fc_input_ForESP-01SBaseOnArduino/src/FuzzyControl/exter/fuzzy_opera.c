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
#ifndef max
#   define max(x, y) ((x>y) ? (x) : (y))
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

bool fuzzy_matrix_init(struct fuzzy_matrix* const mat)
{
    if (mat == nullptr) return false;

    mat->mat = nullptr;
    mat->row = 0;
    mat->col = 0;

    return true;
}

bool fuzzy_matrix_create(struct fuzzy_matrix* const mat, const fuzzy_size row, const fuzzy_size col)
{
    if (mat == nullptr) return false;
    if (row <= 0 || col <= 0) return false;

    // Uninitialized, matrix creation not allowed
    if (__IS_FUZZY_MATRIX_CREATED(mat)) return false;

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

bool fuzzy_matrix_reshape(struct fuzzy_matrix* const mat, const fuzzy_size row, const fuzzy_size col)
{
    if (mat == nullptr) return false;
    if (row <= 0 || col <= 0) return false;
    if (__IS_FUZZY_MATRIX_CREATED(mat) && __IS_FUZZY_MATRIX_DAMAGED(mat)) return false;

    if (__IS_FUZZY_MATRIX_CREATED(mat) && (mat->row == row && mat->col == col)) return true;

    fuzzy_size ori_row = mat->row;  // Number of rows in the original matrix
    fuzzy_size ori_col = mat->col;  // Number of columns in the original matrix

    // The number of rows in the new matrix is not equal to the number of rows
    // in the original matrix, and a new application needs to be made
    if (mat->row != row)
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
            if (__IS_FUZZY_MATRIX_CREATED(&mark))
            {
                fuzzy_matrix_delete(&mark);
            }
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
    if (!(row <= ori_row && col == ori_col))
    {
        void *temp = nullptr;

        for (fuzzy_size r = 0; r < mat->row; r++)
        {
            temp = __fuzzy_matrix_realloc(mat->mat[r], col * sizeof(fuzzy_number));
            if (temp == nullptr)
            {
                // Failed to realloc for matrix, marked as damaged matrix
                __SET_FUZZY_MATRIX_AS_DAMAGED(mat);
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

bool fuzzy_matrix_reshape_s(struct fuzzy_matrix* const mat, const fuzzy_size row, const fuzzy_size col)
{
    if (mat == nullptr) return false;
    if (row <= 0 || col <= 0) return false;
    if (__IS_FUZZY_MATRIX_CREATED(mat) && __IS_FUZZY_MATRIX_DAMAGED(mat)) return false;

    if (__IS_FUZZY_MATRIX_CREATED(mat) && (mat->row == row && mat->col == col)) return true;

    if (!__IS_FUZZY_MATRIX_CREATED(mat)) return fuzzy_matrix_create(mat, row, col);

    // Let the brave go and slaughter the dragon,
    // and then the king will snatch the credit of the brave
    struct fuzzy_matrix brave;
    fuzzy_matrix_init(&brave);
    if (!fuzzy_matrix_create(&brave, row, col)) return false;
    fuzzy_matrix_clear(&brave);
    fuzzy_matrix_copy_just_elem(&brave, mat, 0, 0);

    struct fuzzy_matrix copy;
    __fuzzy_matrix_memcpy(&copy, mat, sizeof(struct fuzzy_matrix));
    fuzzy_matrix_init(mat);
    if (!fuzzy_matrix_pay_tribute(mat, &brave))
    {
        __fuzzy_matrix_memcpy(mat, &copy, sizeof(struct fuzzy_matrix));
        fuzzy_matrix_delete(&brave);
        return false;
    }
    fuzzy_matrix_delete(&copy);

    return true;
}

bool fuzzy_matrix_clear(const struct fuzzy_matrix* const mat)
{
    if (mat == nullptr) return false;
    if (!__IS_FUZZY_MATRIX_CREATED(mat)) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(mat)) return false;

    for (fuzzy_size r = 0; r < mat->row; r++)
    {
        __fuzzy_matrix_memset(mat->mat[r], 0, mat->col * sizeof(fuzzy_number));
    }

    return true;
}

bool fuzzy_matrix_set_elem(const struct fuzzy_matrix* const mat, const fuzzy_number value)
{
    if (mat == nullptr) return false;
    if (!__IS_FUZZY_MATRIX_CREATED(mat)) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(mat)) return false;

    for (fuzzy_size r = 0; r < mat->row; r++)
    {
        for (fuzzy_size c = 0; c < mat->col; c++)
        {
            mat->mat[r][c] = value;
        }
    }

    return true;
}

bool fuzzy_matrix_pay_tribute(struct fuzzy_matrix* const emperor, struct fuzzy_matrix* const vassal)
{
    if (emperor == nullptr || vassal == nullptr) return false;
    if (__IS_FUZZY_MATRIX_CREATED(emperor)) return false;
    if (!__IS_FUZZY_MATRIX_CREATED(vassal)) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(vassal)) return false;

    __fuzzy_matrix_memcpy(emperor, vassal, sizeof(struct fuzzy_matrix));
    fuzzy_matrix_init(vassal);

    return true;
}

bool fuzzy_matrix_rob(struct fuzzy_matrix* const king, struct fuzzy_matrix* const brave)
{
    if (king == nullptr || brave == nullptr) return false;
    if (!__IS_FUZZY_MATRIX_CREATED(brave)) return false;

    if (__IS_FUZZY_MATRIX_DAMAGED(brave))
    {
        if (__IS_FUZZY_MATRIX_CREATED(king))
            fuzzy_matrix_delete(king);
        fuzzy_matrix_delete(brave);
        return false;
    }

    fuzzy_matrix_delete(king);
    __fuzzy_matrix_memcpy(king, brave, sizeof(struct fuzzy_matrix));
    fuzzy_matrix_init(brave);

    return true;
}

bool fuzzy_matrix_delete(struct fuzzy_matrix* const mat)
{
    if (mat == nullptr) return false;
    if (!__IS_FUZZY_MATRIX_CREATED(mat)) return true;

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

bool fuzzy_matrix_copy(struct fuzzy_matrix* const dst, const struct fuzzy_matrix* const src)
{
    if (dst == nullptr || src == nullptr) return false;
    if (!__IS_FUZZY_MATRIX_CREATED(src)) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(src)) return false;

    if (
        !__IS_FUZZY_MATRIX_CREATED(dst)
        || !(dst->row == src->row && dst->col == src->col)
     ) {
        fuzzy_matrix_delete(dst);
        if (!fuzzy_matrix_create(dst, src->row, src->col)) return false;
    }

    for (fuzzy_size r = 0; r < dst->row; r++)
    {
        for (fuzzy_size c = 0; c < dst->col; c++)
        {
            dst->mat[r][c] = src->mat[r][c];
        }
    }

    return true;
}

bool fuzzy_matrix_copy_just_elem(struct fuzzy_matrix* const dst, const struct fuzzy_matrix* const src, const fuzzy_size offset_row, const fuzzy_size offset_col)
{
    if (dst == nullptr || src == nullptr) return false;
    if ((!__IS_FUZZY_MATRIX_CREATED(dst)) || (!__IS_FUZZY_MATRIX_CREATED(src))) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(dst)) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(src)) return false;
    if (offset_row < 0 || offset_col < 0) return false;

    fuzzy_size min_row = min(dst->row, src->row);
    fuzzy_size min_col = min(dst->col, src->col);

    for (fuzzy_size r = 0; r < min_row; r++)
    {
        if (r + offset_row >= dst->row) break;
        for (fuzzy_size c = 0; c < min_col; c++)
        {
            if (c + offset_col >= dst->col) break;
            dst->mat[r + offset_row][c + offset_col] = src->mat[r][c];
        }
    }

    return true;
}

bool fuzzy_matrix_horzcat(struct fuzzy_matrix* const dst, const struct fuzzy_matrix* src1, const struct fuzzy_matrix* src2)
{
    if (dst == nullptr) return false;
    if (src1 != nullptr && ((!__IS_FUZZY_MATRIX_CREATED(src1)) || __IS_FUZZY_MATRIX_DAMAGED(src1))) return false;
    if (src2 != nullptr && ((!__IS_FUZZY_MATRIX_CREATED(src2)) || __IS_FUZZY_MATRIX_DAMAGED(src2))) return false;
    if (src1 == nullptr || src2 == nullptr)
        if ((!__IS_FUZZY_MATRIX_CREATED(dst)) || __IS_FUZZY_MATRIX_DAMAGED(dst)) return false;

    // Stacking using oneself as a template
    struct fuzzy_matrix template1, template2;
    bool is_copy_template1 = false;
    bool is_copy_template2 = false;
    if (dst == src1 || src1 == nullptr)
    {
        fuzzy_matrix_init(&template1);
        if (!fuzzy_matrix_copy(&template1, dst)) goto error_out;
        src1 = &template1;
        is_copy_template1 = true;
    }
    if (dst == src2 || src2 == nullptr)
    {
        fuzzy_matrix_init(&template2);
        if (!fuzzy_matrix_copy(&template2, dst)) goto error_out;
        src2 = &template2;
        is_copy_template2 = true;
    }

    // Unequal number of rows, unable to concatenate
    if (src1->row != src2->row) return false;

    fuzzy_size cat_row = src1->row;
    fuzzy_size cat_col = src1->col + src2->col;
    if (cat_col <= 0 || cat_col < min(src1->col, src2->col)) return false;

    if (
        !__IS_FUZZY_MATRIX_CREATED(dst)
        || __IS_FUZZY_MATRIX_DAMAGED(dst)
        || !(dst->row == cat_row && dst->col == cat_col)
    ) {
        fuzzy_matrix_delete(dst);
        if (!fuzzy_matrix_create(dst, cat_row, cat_col)) goto error_out;
    }

    if (!fuzzy_matrix_copy_just_elem(dst, src1, 0, 0)) goto error_out;
    if (!fuzzy_matrix_copy_just_elem(dst, src2, 0, src1->col)) goto error_out;

    if (0)
    {
    error_out:
        if (is_copy_template1)
            fuzzy_matrix_delete(&template1);
        if (is_copy_template2)
            fuzzy_matrix_delete(&template2);
        return false;
    }

    if (is_copy_template1)
        fuzzy_matrix_delete(&template1);
    if (is_copy_template2)
        fuzzy_matrix_delete(&template2);

    return true;
}

bool fuzzy_matrix_vertcat(struct fuzzy_matrix* const dst, const struct fuzzy_matrix* src1, const struct fuzzy_matrix* src2)
{
    if (dst == nullptr) return false;
    if (src1 != nullptr && ((!__IS_FUZZY_MATRIX_CREATED(src1)) || __IS_FUZZY_MATRIX_DAMAGED(src1))) return false;
    if (src2 != nullptr && ((!__IS_FUZZY_MATRIX_CREATED(src2)) || __IS_FUZZY_MATRIX_DAMAGED(src2))) return false;
    if (src1 == nullptr || src2 == nullptr)
        if ((!__IS_FUZZY_MATRIX_CREATED(dst)) || __IS_FUZZY_MATRIX_DAMAGED(dst)) return false;

    // Stacking using oneself as a template
    struct fuzzy_matrix template1, template2;
    bool is_copy_template1 = false;
    bool is_copy_template2 = false;
    if (dst == src1 || src1 == nullptr)
    {
        fuzzy_matrix_init(&template1);
        if (!fuzzy_matrix_copy(&template1, dst)) goto error_out;
        src1 = &template1;
        is_copy_template1 = true;
    }
    if (dst == src2 || src2 == nullptr)
    {
        fuzzy_matrix_init(&template2);
        if (!fuzzy_matrix_copy(&template2, dst)) goto error_out;
        src2 = &template2;
        is_copy_template2 = true;
    }

    // Unequal number of rows, unable to concatenate
    if (src1->col != src2->col) return false;

    fuzzy_size cat_row = src1->row + src2->row;
    fuzzy_size cat_col = src1->col;
    if (cat_row <= 0 || cat_row < min(src1->row, src2->row)) return false;

    if (
        !__IS_FUZZY_MATRIX_CREATED(dst)
        || __IS_FUZZY_MATRIX_DAMAGED(dst)
        || !(dst->row == cat_row && dst->col == cat_col)
    ) {
        fuzzy_matrix_delete(dst);
        if (!fuzzy_matrix_create(dst, cat_row, cat_col)) goto error_out;
    }

    if (!fuzzy_matrix_copy_just_elem(dst, src1, 0, 0)) goto error_out;
    if (!fuzzy_matrix_copy_just_elem(dst, src2, src1->row, 0)) goto error_out;

    if (0)
    {
    error_out:
        if (is_copy_template1)
            fuzzy_matrix_delete(&template1);
        if (is_copy_template2)
            fuzzy_matrix_delete(&template2);
        return false;
    }

    if (is_copy_template1)
        fuzzy_matrix_delete(&template1);
    if (is_copy_template2)
        fuzzy_matrix_delete(&template2);

    return true;
}

bool fuzzy_matrix_repmat(struct fuzzy_matrix* const dst, const struct fuzzy_matrix* src, const fuzzy_size row, const fuzzy_size col)
{
    if (dst == nullptr) return false;
    if (row <= 0 || col <= 0) return false;
    if (src != nullptr && ((!__IS_FUZZY_MATRIX_CREATED(src)) || __IS_FUZZY_MATRIX_DAMAGED(src))) return false;
    if (src == nullptr && ((!__IS_FUZZY_MATRIX_CREATED(dst)) || __IS_FUZZY_MATRIX_DAMAGED(dst))) return false;

    // Stacking using oneself as a template
    struct fuzzy_matrix template;
    bool is_copy_template = false;
    if (dst == src || src == nullptr)
    {
        fuzzy_matrix_init(&template);
        if (!fuzzy_matrix_copy(&template, dst)) return false;
        src = &template;
        is_copy_template = true;
    }

    if (FUZZY_SIZE_MAX / src->row < row || FUZZY_SIZE_MAX / src->col < col)
    {
        if (is_copy_template)
            fuzzy_matrix_delete(&template);
        return false;
    }
    fuzzy_size real_row = row * src->row;
    fuzzy_size real_col = col * src->col;
    if ((real_row <= 0 || real_row < src->row) || (real_col <= 0 || real_col < src->col))
        return false;

    if (
        !__IS_FUZZY_MATRIX_CREATED(dst)
        || __IS_FUZZY_MATRIX_DAMAGED(dst)
        || !(dst->row == real_row && dst->col == real_col)
    ) {
        fuzzy_matrix_delete(dst);
        if (!fuzzy_matrix_create(dst, real_row, real_col))
        {
            if (is_copy_template)
                fuzzy_matrix_delete(&template);
            return false;
        }
    }

    for (fuzzy_size r = 0; r < row; r++)
    {
        for (fuzzy_size c = 0; c < col; c++)
        {
            for (fuzzy_size mr = 0; mr < src->row; mr++)
            {
                for (fuzzy_size mc = 0; mc < src->col; mc++)
                {
                    dst->mat[r * src->row + mr][c * src->col + mc] = src->mat[mr][mc];
                }
            }
        }
    }

    if (is_copy_template)
    {
        fuzzy_matrix_delete(&template);
    }

    return true;
}

bool fuzzy_matrix_trav(const struct fuzzy_matrix* const mat, void* const data, const fuzzy_opera_event_cb event_cb)
{
    if (mat == nullptr) return false;
    if (!__IS_FUZZY_MATRIX_CREATED(mat)) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(mat)) return false;
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

void fuzzy_matrix_print(const struct fuzzy_matrix* const mat, const char* label)
{
    if (mat == nullptr)
    {
        __FUZZY_MATRIX_PRINTF("\r\nmat is illegal!\r\n");
        return;
    }
    if (!__IS_FUZZY_MATRIX_CREATED(mat))
    {
        __FUZZY_MATRIX_PRINTF("\r\nmat is not be created!\r\n");
        return;
    }
    if (__IS_FUZZY_MATRIX_DAMAGED(mat))
    {
        __FUZZY_MATRIX_PRINTF("\r\nmat is damaged!\r\n");
        return;
    }

    __FUZZY_MATRIX_PRINTF("\r\n");
    if (label != nullptr)
    {
        __FUZZY_MATRIX_PRINTF("%s: \r\n", label);
    }
    __FUZZY_MATRIX_PRINTF("\t");
    for (fuzzy_size c = 0; c < mat->col; c++)
    {
        fuzzy_size digit = get_digit(c) + 2;
        __FUZZY_MATRIX_PRINTF("   [%llu]", c);
        if ((2 - digit / 4) < 0) continue;
        for (fuzzy_size n = 0; n < 2 - digit / 4; n++)
        {
            __FUZZY_MATRIX_PRINTF("\t");
        }
    }
    __FUZZY_MATRIX_PRINTF("\r\n");
    for (fuzzy_size r = 0; r < mat->row; r++)
    {
        fuzzy_size digit = get_digit(r) + 2;
        __FUZZY_MATRIX_PRINTF("[%llu]", r);
        for (fuzzy_size n = 0; n < 1 - digit / 4; n++)
        {
            __FUZZY_MATRIX_PRINTF("\t");
        }
        for (fuzzy_size c = 0; c < mat->col; c++)
        {
            __FUZZY_MATRIX_PRINTF("%9.4f\t", mat->mat[r][c]);
        }
        __FUZZY_MATRIX_PRINTF("\r\n");
    }
}

/*==================================================================================
    Fuzzy operation API
==================================================================================*/

bool fuzzy_opera_transpose(struct fuzzy_matrix* const matT, const struct fuzzy_matrix* mat)
{
    if (matT == nullptr) return false;
    if (mat != nullptr && ((!__IS_FUZZY_MATRIX_CREATED(mat)) || __IS_FUZZY_MATRIX_DAMAGED(mat))) return false;
    if (mat == nullptr && ((!__IS_FUZZY_MATRIX_CREATED(matT)) || __IS_FUZZY_MATRIX_DAMAGED(matT))) return false;

    struct fuzzy_matrix template;
    bool is_copy_template = false;
    if (matT == mat || mat == nullptr)
    {
        fuzzy_matrix_init(&template);
        if (!fuzzy_matrix_copy(&template, matT)) return false;
        mat = &template;
        is_copy_template = true;
    }

    // Determine if it is just right and appropriate
    if (
        !__IS_FUZZY_MATRIX_CREATED(matT)
        || !(mat->row == matT->col && mat->col == matT->row)
    ) {
        // Create a suitable matrix
        fuzzy_matrix_delete(matT);
        if (!fuzzy_matrix_create(matT, mat->col, mat->row))
        {
            if (is_copy_template)
                fuzzy_matrix_delete(&template);
            return false;
        }
    }

    // Start transpose
    for (fuzzy_size r = 0; r < mat->row; r++)
    {
        for (fuzzy_size c = 0; c < mat->col; c++)
        {
            matT->mat[c][r] = mat->mat[r][c];
        }
    }

    if (is_copy_template)
    {
        fuzzy_matrix_delete(&template);
    }

    return true;
}

bool fuzzy_opera_dir_pro(struct fuzzy_matrix* const result, const struct fuzzy_matrix* const mat1, const struct fuzzy_matrix* const mat2)
{
    if (result == nullptr || mat1 == nullptr || mat2 == nullptr) return false;
    if ((!__IS_FUZZY_MATRIX_CREATED(mat1)) || (!__IS_FUZZY_MATRIX_CREATED(mat2))) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(mat1)) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(mat2)) return false;
    if (mat2->row != 1) return false;

    if (FUZZY_SIZE_MAX / mat1->row < mat1->col) return false;
    struct fuzzy_matrix mat1T;
    fuzzy_matrix_init(&mat1T);
    if (!fuzzy_matrix_create(&mat1T, mat1->row * mat1->col, 1)) return false;
    for (fuzzy_size r = 0; r < mat1->row; r++)
    {
        for (fuzzy_size c = 0; c < mat1->col; c++)
        {
            mat1T.mat[r * mat1->col + c][0] = mat1->mat[r][c];
        }
    }

    bool ret = fuzzy_opera(result, &mat1T, mat2);

    fuzzy_matrix_delete(&mat1T);

    return ret;
}

bool fuzzy_opera_dir_pro_s(struct fuzzy_matrix* const result, const struct fuzzy_matrix* const mat1, const struct fuzzy_matrix* const mat2)
{
    if (result == nullptr || mat1 == nullptr || mat2 == nullptr) return false;
    if ((!__IS_FUZZY_MATRIX_CREATED(mat1)) || (!__IS_FUZZY_MATRIX_CREATED(mat2))) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(mat1)) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(mat2)) return false;
    if (mat2->row != 1) return false;

    if (FUZZY_SIZE_MAX / mat1->row < mat1->col) return false;
    fuzzy_size result_row = mat1->row * mat1->col;
    fuzzy_size result_col = mat2->col;

    // mat1 needs to be transposed. If mat1 is not a row vector, 
    // then the transpose of each row is treated as a column
    // and arranged in sequence as a column
    if (
        !__IS_FUZZY_MATRIX_CREATED(result)
        || !(result->row == result_row && result->col == result_col)
    ) {
        // Create a suitable matrix
        fuzzy_matrix_delete(result);
        if (!fuzzy_matrix_create(result, result_row, result_col)) return false;
    }

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

bool fuzzy_opera(struct fuzzy_matrix* const result, const struct fuzzy_matrix* const mat1, const struct fuzzy_matrix* const mat2)
{
    if (result == nullptr || mat1 == nullptr || mat2 == nullptr) return false;
    if ((!__IS_FUZZY_MATRIX_CREATED(mat1)) || (!__IS_FUZZY_MATRIX_CREATED(mat2))) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(mat1)) return false;
    if (__IS_FUZZY_MATRIX_DAMAGED(mat2)) return false;
    if (mat1->col != mat2->row) return false;

    fuzzy_size result_row = mat1->row;
    fuzzy_size result_col = mat2->col;

    if (
        !__IS_FUZZY_MATRIX_CREATED(result)
        || !(result->row == result_row && result->col == result_col)
        ) {
        // Create a suitable matrix
        fuzzy_matrix_delete(result);
        if (!fuzzy_matrix_create(result, result_row, result_col)) return false;
    }

    // Start fuzzy operation
    for (fuzzy_size r = 0; r < result_row; r++)
    {
        for (fuzzy_size c = 0; c < result_col; c++)
        {
            fuzzy_number res = __fuzzy_opera_int(mat1->mat[r][0], mat2->mat[0][c]);
            for (fuzzy_size n = 1; n < mat1->col; n++)
            {
                res = __fuzzy_opera_uni(res, __fuzzy_opera_int(mat1->mat[r][n], mat2->mat[n][c]));
            }
            result->mat[r][c] = res;
        }
    }

    return false;
}
