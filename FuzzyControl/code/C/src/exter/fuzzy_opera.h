/*****************************************************************//**
 * \file   fuzzy_opera.h
 * \brief  Here we only discuss the operations between different
 *         fuzzy subsets/relationships
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

/*==================================================================================
    Internal Fuzzy Operation API
==================================================================================*/

/**
 * @brief The addition operation of a single fuzzy number
 * 
 * @param add1 Addend 1
 * @param add2 Addend 2
 * @return fuzzy_number result
 */
static fuzzy_number _fuzzy_opera_add(fuzzy_number add1, fuzzy_number add2);

/**
 * @brief The subtraction operation of a single fuzzy number
 * 
 * @param sub1 Subtracted number
 * @param sub2 Subtraction
 * @return fuzzy_number result
 */
static fuzzy_number _fuzzy_opera_sub(fuzzy_number sub1, fuzzy_number sub2);

/**
 * @brief The multiplication operation of a single fuzzy number
 * 
 * @param mul1 Multiplicand
 * @param mul2 Multiplier
 * @return fuzzy_number result
 */
static fuzzy_number _fuzzy_opera_mul(fuzzy_number mul1, fuzzy_number mul2);

/**
 * @brief The division operation of a single fuzzy number
 * 
 * @param div1 Dividend
 * @param div2 Divisor
 * @return fuzzy_number result
 */
static fuzzy_number _fuzzy_opera_div(fuzzy_number div1, fuzzy_number div2);

/**
 * @brief The union operation of a single fuzzy number
 *
 * @param uni1
 * @param uni2
 * @return fuzzy_number result
 */
static fuzzy_number _fuzzy_opera_uni(fuzzy_number uni1, fuzzy_number uni2);

/**
 * @brief The intersection operation of a single fuzzy number
 *
 * @param int1
 * @param int2
 * @return fuzzy_number result
 */
static fuzzy_number _fuzzy_opera_int(fuzzy_number int1, fuzzy_number int2);

/**
 * @brief The complement operation of a single fuzzy number
 *
 * @param com
 * @return fuzzy_number result
 */
static fuzzy_number _fuzzy_opera_com(fuzzy_number com);

/*==================================================================================
    Fuzzy operation API
==================================================================================*/

/**
 * @brief Invert matrices/vectors
 * @details This function does not request memory, you need to apply for the memory
 *          of MVT before running this function
 * @note A two-dimensional array that can only handle requests for second level
 *       pointers
 * 
 * @param mv Transposed matrix/vector
 * @param row The number of rows in the transposed matrix/vector
 * @param col The number of columns in the transposed matrix/vector
 * @param mvt Memory for storing transposed matrices/vectors
 * @return true success
 * @return false failed
 */
bool fuzzy_opera_trans(fuzzy_number **mv, size_t row, size_t col, fuzzy_number **mvt);

#endif  // !__FUZZY_OPERA_H__


