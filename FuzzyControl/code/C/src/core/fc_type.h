/**
 * @file fc_type.h
 * @author nayooooo (YEWANhup@outlook.com)
 * @brief This file defines some structures and enumeration types related to fuzzy
 *        sets and fuzzy operations based on template style one-way linked lists.
 * @details Only supports fuzzy sets and fuzzy relationships
 * @version 0.1
 * @date 2024-05-22
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __FC_TYPE_H__
#define __FC_TYPE_H__

/*==================================================================================
    structure and enumeration
==================================================================================*/

typedef const char* label;

#include "list.h"
typedef list_node fuzzy_set_rela;  // fuzzy set or relation
typedef list_head fuzzy_set_list;

typedef float membership;  // [0,1]

// Fuzzy data type
typedef enum
{
    FUZZY_DATA_NULL = 0,
    FUZZY_DATA_INPUT,           // fuzzy input data
    FUZZY_DATA_OUTPUT,          // fuzzy output data
    FUZZY_DATA_RELATION,        // fuzzy relation
}fuzzy_data_type;

// membership vecotr/matrix type
typedef enum
{
    MS_VM_ROW_VECTOR = 0,
    MS_VM_COL_VECTOR,
    MS_VM_MATRIX,
}membership_vm_type;

/**
 * @brief Fuzzy data structures can be fuzzy sets or fuzzy relationships
 * 
 * @memberof eval Evaluation criteria, which is a label (string) array pointer that
 *                can store multiple labels
 *                   Number of tags      Representative data type
 *                      1                   fuzzy set
 *                      >=2                 fuzzy relation
 * @memberof eval_num Evaluation criteria number, you can use it to determine
 *                    whether the fuzzy output was successfully obtained
 * @memberof level The level of evaluation criteria, the same row belongs to the
 *                 same evaluation criterion, the 0th row belongs to the 0th
 *                 evaluation criterion, and the 1st row belongs to the 1st
 *                 evaluation criterion
 * @memberof fdt fuzzy data type
 * @memberof ms_type ms's type
 * @memberof ms Membership degree vector/matrix
 */
struct fuzzy_data
{
    label *eval;  // evaluation criterion
    size_t eval_num;
    label **level;

    fuzzy_data_type fdt;
    membership_vm_type ms_type;

    membership **ms;
};

#endif  // !__FC_TYPE_H__
