#ifndef __FC_OPERATION_H__
#define __FC_OPERATION_H__

#include "fc_type.h"

/*==================================================================================
    Fuzzy operation class
==================================================================================*/

// It may be necessary to hide these basic addition and multiplication techniques (static)
// and provide only 5 APIs: fuzzy data input, fuzzy relation input, fuzzy output input,
// fuzzy relation calculation, and fuzzy output calculation

// TODO
/**
 * @brief Fuzzy addition
 * 
 */
typedef bool (*fuzzy_operation_add)(fuzzy_set_list l);

// TODO
/**
 * @brief Fuzzy multiplication
 * 
 */
typedef bool (*fuzzy_operation_mul)();

// TODO
/**
 * @brief Design a fuzzy operation class
 * 
 * @memberof obj Data object, which stores fuzzy data, including fuzzy input data,
 *               fuzzy relationships, and fuzzy output data
 */
struct fuzzy_operation
{
    fuzzy_set_list obj;

    fuzzy_operation_add add;
    fuzzy_operation_mul mul;
};

#endif  // !__FC_OPERATION_H__
