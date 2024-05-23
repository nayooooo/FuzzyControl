#include "fuzzy_opera.h"

#ifndef nullptr
#   define nullptr ((void*)0)
#endif // !nullptr


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
static inline bool is_fuzzy_number_eq(fuzzy_number n1, fuzzy_number n2)
{
    return fabs(n1 - n2) < FLT_EPSILON;
}

static fuzzy_number _fuzzy_opera_add(fuzzy_number add1, fuzzy_number add2)
{
    return fminf(add1, add2);
}

static fuzzy_number _fuzzy_opera_sub(fuzzy_number sub1, fuzzy_number sub2)
{
    return fminf(sub1, sub2);
}

static fuzzy_number _fuzzy_opera_mul(fuzzy_number mul1, fuzzy_number mul2)
{
    return fminf(mul1, mul1);
}

static fuzzy_number _fuzzy_opera_div(fuzzy_number div1, fuzzy_number div2)
{
    return fminf(div1, div2);
}

static fuzzy_number _fuzzy_opera_uni(fuzzy_number uni1, fuzzy_number uni2)
{
    return fmaxf(uni1, uni2);
}

static fuzzy_number _fuzzy_opera_int(fuzzy_number int1, fuzzy_number int2)
{
    return fminf(int1, int2);
}

static fuzzy_number _fuzzy_opera_com(fuzzy_number com)
{
    return 1 - com;
}

bool fuzzy_opera_trans(fuzzy_number **mv, size_t row, size_t col, fuzzy_number **mvt)
{
    if (mv == nullptr || mvt == nullptr) return false;
    if (row <= 0 && col <= 0) return false;

    for (size_t c = 0; c < col; c++)
    {
        for (size_t r = 0; r < row; r++)
        {
            mvt[c][r] = mv[r][c];
        }
    }

    return true;
}
