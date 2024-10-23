#include "fc_core.h"

fuzzy_number fc_core_Gaussian(accurate_number value, accurate_number mu, accurate_number sigma, accurate_number k)
{
    if (sigma < 0) return FUZZY_DATA_ILLEGAL_VALUE;

    return k * __FC_CORE_EXP(- ((value - mu) * (value - mu)) / (2 * sigma * sigma));
}
