#ifndef __FC_LIST_H__
#define __FC_LIST_H__

#include <stdbool.h>
#include "fc_def.h"

struct fc_list
{
    void *data;
    struct fc_list *next;
};
typedef struct fc_list *fc_list_node;
typedef struct fc_list **fc_list_head;

/**
 * @brief Predicate used to determine whether an operation will be executed
 * @param node The node currently being checked
 * @param data Data to be matched
 * @return bool
 *      true        will execute
 *      false       not execute
*/
typedef bool (*fc_list_pred)(fc_list_node node, void *data);

/**
 * @brief Built in total truth predicate
 * @param node No effect
 * @param data No effect
 * @return true
*/
bool fc_list_pred_true(fc_list_node node, void *data);

/**
 * @brief Built in total false predicate
 * @param node No effect
 * @param data No effect
 * @return false
*/
bool fc_list_pred_false(fc_list_node node, void *data);

/**
 * @brief Create a linked list
 * @return fc_list_head References to linked lists
*/
fc_list_head fc_list_create(void);

/**
 * @brief Clear a linked list
 * @param l The linked list to be cleared
 * @return bool
 *      true        success
 *      false       failed
*/
bool fc_list_clear(fc_list_head l);

/**
 * @brief Delete(free) a linked list
 * @param l The linked list to be delete
 * @return bool
 *      true        success
 *      false       failed
*/
bool fc_list_delete(fc_list_head l);

/**
 * @brief Find nodes through predicate
 * @param l The linked list to be searched
 * @param data The data of the node to be searched
 * @param pred Determine whether it is a predicate for a specified node
 * @return fc_list_node
 *      nullptr     not found
 *      other       found
*/
fc_list_node fc_list_find_if(fc_list_head l, void *data, fc_list_pred pred);

/**
 * @brief Push nodes through predicate
 * @details Make deep copies when creating nodes, without using data's memory
 * @param l The linked list to be pushed
 * @param data The node data, the node will be pushed
 * @param pred The predicate
 * @return bool
 *      true        success
 *      false       failed
*/
bool fc_list_push_if(fc_list_head l, void *data, fc_list_pred pred);

/**
 * @brief Pop nodes through predicate
 * @details Releasing node memory will not release data's memory
 * @param l The linked list to be popped
 * @param data The node data, the node will be popped
 * @param pred The predicate
 * @return bool
 *      true        success
 *      false       failed
*/
bool fc_list_pop_if(fc_list_head l, void *data, fc_list_pred pred);

/**
 * @details Swap the logical positions of two nodes
 * @param l The linked list to be swapped
 * @param data1 Data with exchange node 1
 * @param data2 Data with exchange node 2
 * @param pred Determine whether it is a predicate for a specified node
*/
bool fc_list_swap_if(fc_list_head l, void *data1, void *data2, fc_list_pred pred);

#endif  // !__FC_LIST_H__