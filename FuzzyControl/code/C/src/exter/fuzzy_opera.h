#ifndef __FUZZY_OPERA_H__
#define __FUZZY_OPERA_H__

#include <stdbool.h>

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

/*==================================================================================
    Fuzzy operation API
==================================================================================*/

#endif  // !__FUZZY_OPERA_H__
