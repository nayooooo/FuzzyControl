/*****************************************************************//**
 * \file   fuzzy_opera.h
 * \brief  This file is used to handle fuzzy matrix operations
 * \note   When using, first declare a matrix, then use
 *         fuzzy_matrix_it to initialize the matrix (you can also
 *         manually initialize it, but the risk is relatively high).
 *         After initialization, use fuzzy_matrix_create to create
 *         the matrix. After completing the above operations, you can
 *         operate on the matrix. Remember to use fuzzy_matrix_delete
 *         to destroy the matrix at the end.
 *         Except for some APIs that require matrix creation in
 *         advance, there is no need to create a matrix in advance.
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

#include <stdbool.h>
#include <stdint.h>

/*==================================================================================
    structure
==================================================================================*/

typedef float fuzzy_number;

/**
 * @brief Fuzzy matrix
 * 
 * @memberof mat matrix 
 * @memberof row matrix rows
 * @memberof col matrix columns
 */
struct fuzzy_matrix
{
    fuzzy_number** mat;
    size_t row;
    size_t col;
};

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
bool fuzzy_matrix_init(struct fuzzy_matrix* mat);

/**
 * @brief Create a fuzzy matrix
 * 
 * @param mat Pointer to the fuzzy matrix to be created
 * @param row Fuzzy matrix rows
 * @param col Fuzzy matrix columns
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_create(struct fuzzy_matrix *mat, size_t row, size_t col);

/**
 * @brief Destroy Fuzzy Matrix
 * 
 * @param mat Pointer to the fuzzy matrix to be deleted
 * @return true success or no need to destroy (i.e. no memory requested)
 * @return false failed
 */
bool fuzzy_matrix_delete(struct fuzzy_matrix* mat);

/**
 * @brief Copy the source fuzzy matrix to the target fuzzy matrix
 * @details The target fuzzy matrix will be destroyed
 * 
 * @param dst Objective Fuzzy Matrix
 * @param src Source Fuzzy Matrix
 */
bool fuzzy_matrix_copy(struct fuzzy_matrix* dst, const struct fuzzy_matrix* src);

/**
 * @brief Traversing matrix
 * 
 * @param mat Pointer to the fuzzy matrix to be traversed
 * @param data Data user gave
 * @param event_cb Event callbacks that each fuzzy number needs to execute, you must give
 * @return true success
 * @return false failed
 */
bool fuzzy_matrix_trav(struct fuzzy_matrix* mat, void *data, fuzzy_opera_event_cb event_cb);

/**
 * @brief Print Matrix
 * 
 * @param mat Pointer to the fuzzy matrix to be printed
 * @param label label
 */
void fuzzy_matrix_print(struct fuzzy_matrix* mat, const char* label);

/*==================================================================================
    Fuzzy operation API
==================================================================================*/

/**
 * @brief Invert matrices/vectors
 * @details Please note that if matT has been applied to matrix memory, it will be
 *          cleared, even if the transpose fails
 * 
 * @param mat Transposed matrix/vector, You need to create and assign values in advance
 * @param matT Memory for storing transposed matrices/vectors, it can be a matrix
 *            that has already been created or an uncreated matrix
 * @return true success
 * @return false failed
 */
bool fuzzy_opera_trans(struct fuzzy_matrix *mat, struct fuzzy_matrix *matT);

/**
 * @brief Direct product of fuzzy matrices
 * @details Please note that if matT has been applied to matrix memory, it will be
 *          cleared, even if direct product solution failed
 * 
 * @param mat1 Direct product left term
 * @param mat2 Direct product right term
 * @param result
 * @return true success
 * @return false failed
 */
bool fuzzy_opera_dir_pro(struct fuzzy_matrix* mat1, struct fuzzy_matrix* mat2, struct fuzzy_matrix* result);

#endif  // !__FUZZY_OPERA_H__


