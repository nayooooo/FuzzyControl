/*****************************************************************//**
 * \file   fuzzy_opera.h
 * \brief  This file is used to handle fuzzy matrix operations
 * \note   When using, first declare a matrix, then use
 *         fuzzy_matrix_init to initialize the matrix (you can also
 *         manually initialize it, but the risk is relatively high).
 *         After initialization, use fuzzy_matrix_create to create
 *         the matrix. After completing the above operations, you can
 *         operate on the matrix. Remember to use fuzzy_matrix_delete
 *         to destroy the matrix at the end.
 *         Except for some APIs that require matrix creation in
 *         advance, there is no need to create a matrix in advance
 *         for others.
 *         To reduce the number of memory application operations,
 *         this file chooses to believe that the matrix entered by
 *         the user is safe (initialized, created and destroyed
 *         using the provided API).
 * 
 * \author nayooooo
 * \date   May 2024
 *********************************************************************/

#ifndef __FUZZY_OPERA_H__
#define __FUZZY_OPERA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <limits.h>

/*==================================================================================
    define
==================================================================================*/

#define FUZZY_MATRIX_DAMAGED_COLUMN_FEATURE (-1)

/*==================================================================================
    micro
==================================================================================*/

#include "fc_config.h"
#define __FUZZY_MATRIX_MALLOC fc_malloc
#define __FUZZY_MATRIX_FREE fc_free
#define __FUZZY_MATRIX_REALLOC fc_realloc
#define __FUZZY_MATRIX_MEMSET fc_memset
#define __FUZZY_MATRIX_MEMCPY fc_memcpy
#define __FUZZY_MATRIX_PRINTF fc_printf

#define __IS_FUZZY_MATRIX_CREATED(m) ((m)->mat != nullptr)
#define __IS_FUZZY_MATRIX_DAMAGED(m) ((m)->col == FUZZY_MATRIX_DAMAGED_COLUMN_FEATURE)
#define __SET_FUZZY_MATRIX_AS_DAMAGED(m) ((m)->col = FUZZY_MATRIX_DAMAGED_COLUMN_FEATURE)

/*==================================================================================
    structure
==================================================================================*/

typedef float decimal_number;
typedef decimal_number accurate_number;
typedef decimal_number fuzzy_number;
typedef long long fuzzy_size;

#define FUZZY_SIZE_MAX LLONG_MAX

/**
 * @brief Fuzzy matrix
 * 
 * @memberof mat matrix 
 * @memberof row matrix rows
 * @memberof col matrix columns
 */
#pragma pack (4)
struct fuzzy_matrix
{
    fuzzy_size row;
    fuzzy_size col;
    fuzzy_number** mat;
};
#pragma pack ()

/**
 * @brief event callback function
 *
 * @param fn Fuzzy number
 * @param data Data user gave
 * @return bool
 *      true        success
 *      false       failed
 */
typedef bool (*fuzzy_opera_event_cb)(fuzzy_number fn, void* data);

/*==================================================================================
    Fuzzy Matrix API
==================================================================================*/

/**
 * @brief Initialize Fuzzy Matrix
 * @details Please note that do not call this function directly
 *          after creating a matrix to avoid memory leaks!
 * 
 * @param mat Pointer to the fuzzy matrix to be initialized
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_init(struct fuzzy_matrix* const mat);

/**
 * @brief Create a fuzzy matrix
 * 
 * @param mat Pointer to the fuzzy matrix to be created
 * @param row Fuzzy matrix rows
 * @param col Fuzzy matrix columns
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_create(struct fuzzy_matrix* const mat, const fuzzy_size row, const fuzzy_size col);

/**
 * @brief Apply for a matrix with a different shape, which will still
 *        be stored in the passed in object, and the original matrix
 *        will be destroyed
 * @details If realloc fails, it may damage the matrix and render it unusable.
 *          Specifically, there are two places where realloc may fail. The first
 *          is the request for a row vector pointer array of a matrix, where
 *          failure does not damage the matrix. The other is the realloc for each
 *          row vector. If a row vector request fails, it will be marked as damaged.
 *          The damaged matrix can be destroyed by calling fuzzy_matrix_delete, but
 *          it cannot continue to be used
 * @note If an empty matrix is passed in, it will be downgraded to create
 * 
 * @param mat Pointer to the fuzzy matrix to be reshaped, if it is an empty matrix,
 *            the function will be downgraded to "create", must not be damaged
 * @param row The number of rows in the new matrix
 * @param col The number of columns in the new matrix
 * @return true success, if the number of rows and columns remains the same,
 *              memory will not be reapplied for and the original memory will
 *              not be manipulated
 * @return false failed, not realloc or damaged matrix
 */
bool fuzzy_matrix_reshape(struct fuzzy_matrix* const mat, const fuzzy_size row, const fuzzy_size col);

/**
 * @brief Applying for matrices with different shapes is safer,
 *        but it takes up more memory
 * @details During this process, it is necessary to destroy the incoming matrix.
 *          If the matrix application is not successful, the input matrix will be retained
 * @note If an empty matrix is passed in, it will be downgraded to create
 * 
 * @param mat Pointer to the fuzzy matrix to be reshaped, if it is an empty matrix,
 *            the function will be downgraded to "create", must not be damaged
 * @param row The number of rows in the new matrix
 * @param col The number of columns in the new matrix
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_reshape_s(struct fuzzy_matrix* const mat, const fuzzy_size row, const fuzzy_size col);

/**
 * @brief Clear matrix data (set the values of each element to 0)
 * 
 * @param mat Pointer to the fuzzy matrix to be cleared
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_clear(const struct fuzzy_matrix* const mat);

/**
 * @brief Set all elements in the matrix to the specified values
 * 
 * @param mat Pointer to the fuzzy matrix to be set
 * @param value
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_set_elem(const struct fuzzy_matrix* const mat, const fuzzy_number value);

/**
 * @brief The vassal states pay tribute to the Chinese emperor (voluntarily
 *        offering their matrix to the emperor, they will lose control of the matrix)
 * 
 * @param emperor it needs to be an empty matrix, used to receive the matrix
 *                contributed by the vassal country to show respect
 * @param vassal
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_pay_tribute(struct fuzzy_matrix* const emperor, struct fuzzy_matrix* const vassal);

/**
 * @brief Seize the resources of the matrix
 * @details The king looks down on warriors without spoils (matrices),
 *          but if warriors have spoils, even if they are incomplete
 *          (damaged matrices), the king will greedily plunder them.
 *          However, the king can only safely seize intact spoils and
 *          then throw away (destroy) his own belongings. If the spoils
 *          of war are incomplete, the warrior will perish together with
 *          the king
 * 
 * @param king Predator, accept damaged matrices
 * @param brave The Brave Being Plundered, accept damaged matrices
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_rob(struct fuzzy_matrix* const king, struct fuzzy_matrix* const brave);

/**
 * @brief Destroy Fuzzy Matrix
 * 
 * @param mat Pointer to the fuzzy matrix to be deleted
 * @return true success or no need to destroy (i.e. no memory requested)
 * @return false failed
 */
bool fuzzy_matrix_delete(struct fuzzy_matrix* const mat);

/**
 * @brief Copy the source fuzzy matrix to the target fuzzy matrix
 * @details The target fuzzy matrix will be destroyed, if the size does not match
 * 
 * @param dst Objective Fuzzy Matrix, accept damaged matrices
 * @param src Source Fuzzy Matrix, matrix must be created and assigned values
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_copy(struct fuzzy_matrix* const dst, const struct fuzzy_matrix* const src);

/**
 * @brief Copy the values of elements in src to dst without involving
 *        smemory application and destruction
 * 
 * @param dst Objective Fuzzy Matrix, must not be damaged
 * @param src Source Fuzzy Matrix, matrix must be created and assigned values
 * @param offset_row Row offset
 * @param offset_col Column offset
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_copy_just_elem(struct fuzzy_matrix* const dst, const struct fuzzy_matrix* const src, const fuzzy_size offset_row, const fuzzy_size offset_col);

/**
 * @brief Horizontal splicing matrix
 * @note If the input source matrix and target matrix are the same matrix,
 *       a deep copy of the source matrix will be made in the function
 *       If src1 or src2 do not exist (as null ptr), The dst will be used
 *       as a template for splicing, and it is required that the dst has
 *       already been created
 *
 * @param dst Matrices after concatenation
 * @param src1 Matrix 1 to be spliced, must be nullptr or created and not damaged
 * @param src2 Matrix 2 to be spliced, must be nullptr or created and not damaged
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_horzcat(struct fuzzy_matrix* const dst, const struct fuzzy_matrix* src1, const struct fuzzy_matrix* src2);

/**
 * @brief Vertical concatenation matrix
 * @note If the input source matrix and target matrix are the same matrix,
 *       a deep copy of the source matrix will be made in the function
 *       If src1 or src2 do not exist (as null ptr), The dst will be used
 *       as a template for splicing, and it is required that the dst has
 *       already been created
 *
 * @param dst Matrices after concatenation
 * @param src1 Matrix 1 to be spliced, must be nullptr or created and not damaged
 * @param src2 Matrix 2 to be spliced, must be nullptr or created and not damaged
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_vertcat(struct fuzzy_matrix* const dst, const struct fuzzy_matrix* src1, const struct fuzzy_matrix* src2);

/**
 * @brief Stacking matrices in two-dimensional space
 * @note If stacked using oneself as a template, a deep copy of the template will be
 *       made in the function
 *       If src does not exist (as nullptr), dst will be used as a template for
 *       stacking, and it is required that dst has been created
 * 
 * @param dst Stacked Matrix, accept damaged matrices
 * @param src Stacking templates, must be nullptr or created and not damaged
 * @param row Stacked Rows
 * @param col Stacked Columns
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_repmat(struct fuzzy_matrix* const dst, const struct fuzzy_matrix* src, const fuzzy_size row, const fuzzy_size col);

/**
 * @brief Traversing matrix
 * 
 * @param mat Pointer to the fuzzy matrix to be traversed, matrix must be created and assigned values
 * @param data Data user gave
 * @param event_cb Event callbacks that each fuzzy number needs to execute, you must give
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_trav(const struct fuzzy_matrix* const mat, void* const data, const fuzzy_opera_event_cb event_cb);

/**
 * @brief Print Matrix
 * 
 * @param mat Pointer to the fuzzy matrix to be printed, matrix must be created and assigned values
 * @param label label
 */
void fuzzy_matrix_print(const struct fuzzy_matrix* const mat, const char* label);

/*==================================================================================
    Fuzzy operation API
==================================================================================*/

/**
 * @brief Transpose matrices
 * @details Note that if `matT` is dimensionally suitable for the transpose, it
 *          will be reused directly without needing recreation. However, please
 *          be aware that even in the event of a transpose failure, `matT` will
 *          still undergo cleanup (deletion) if it was recreated during this process.
 * @note If `mat` is nullptr, `matT` will serve as the template for the transpose
 *       operation,  requiring it to have been previously created and valid.
 * 
 * @param matT Memory for storing transposed matrices, accept damaged matrices
 * @param mat The matrix to be transposed must be nullptr or created and not damaged
 * @return true success
 * @return false failed
 */
bool fuzzy_opera_transpose(struct fuzzy_matrix* const matT, const struct fuzzy_matrix *mat);

/**
 * @brief Direct product of fuzzy matrices
 * @details Use fuzzy'opera for operations, in which memory is allocated to create
 *          transposes of matrices unfolded by rows
 * @note If `mat1` or `mat2` is nullptr, `matT` will serve as the template for the
 *       transpose operation,  requiring it to have been previously created and valid.
 * 
 * @param result, accept damaged matrices
 * @param mat1 Direct product left term, matrix must be created and assigned values
 * @param mat2 Direct product right term, matrix must be created and assigned values
 * @return true success
 * @return false failed
 */
bool fuzzy_opera_dir_pro(struct fuzzy_matrix* const result, const struct fuzzy_matrix* mat1, const struct fuzzy_matrix* mat2);

/**
 * @brief Direct product of fuzzy matrices(safe)
 * @details This function performs a direct product between "mat1" and "mat2". The
 *          right operand ("mat2") should be a single row matrix. The result is
 *          stored in "result". If the "result" already exists and matches the
 *          expected size of the product, it will be reused; Otherwise, it will be
 *          resized or newly created as needed.
 * @note This function handles memory management for "results" and may release
 *       existing data.
 *
 * @param result, accept damaged matrices
 * @param mat1 Direct product left term, matrix must be created and assigned values
 * @param mat2 Direct product right term, matrix must be created and assigned values
 * @return true success
 * @return false failed
 */
bool fuzzy_opera_dir_pro_s(struct fuzzy_matrix* const result, const struct fuzzy_matrix* const mat1, const struct fuzzy_matrix* const mat2);

/**
 * @brief Perform fuzzy operations on matrices
 * 
 * @param result, accept damaged matrices
 * @param mat1 Fuzzy operations left term, matrix must be created and assigned values
 * @param mat2 Fuzzy operations right term, matrix must be created and assigned values
 * @return true success
 * @return false failed
 */
bool fuzzy_opera(struct fuzzy_matrix* const result, const struct fuzzy_matrix* const mat1, const struct fuzzy_matrix* const mat2);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FUZZY_OPERA_H__


