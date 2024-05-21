#ifndef __FC_LIST_H__
#define __FC_LIST_H__

/**
 * @file fc_list.h
 * @author nayooooo (YEWANhup@outlook.com)
 * @brief This is a linked list library
 * @details When creating a new node, this library will dynamically apply for memory
 *          to create a node, and at the same time apply for data memory based on
 *          the numerical value of the data size you pass in. Therefore, the data
 *          template you pass in is only used as a copy source, not a data source.
 *          In terms of node data design, I have imitated the construction and
 *          destruction characteristics of objects in C++, providing you with
 *          options for constructing callback functions and destructing callback
 *          functions, making it convenient for you to operate on deeply copied node
 *          data.
 * @version 0.1
 * @date 2024-05-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdbool.h>
#include <stdint.h>
#include "fc_def.h"

/*==================================================================================
    structure
==================================================================================*/

struct fc_list
{
    void *data;
    struct fc_list *next;
};
typedef struct fc_list *fc_list_node;
typedef struct fc_list **fc_list_head;

/*==================================================================================
    predicate and callback
==================================================================================*/

/**
 * @brief Predicate used to determine whether an operation will be executed
 * 
 * @param node The node currently being checked
 * @param data Data to be matched
 * @return bool
 *      true        will execute
 *      false       not execute
 */
typedef bool (*fc_list_pred)(fc_list_node node, void *data);

/**
 * @brief Constructing callback functions for node data
 * 
 * @param data Created node data
 * @return true success
 * @return false failed
 */
typedef bool (*fc_list_node_data_construct_cb)(void *data);

/**
 * @brief Deconstructive callback function for node data
 * 
 * @param data Node data to be deleted
 * @return true success
 * @return false failed
 */
typedef bool (*fc_list_node_data_deconstruct_cb)(void *data);

/*==================================================================================
    built in predicate and callback
==================================================================================*/

/**
 * @brief Built in total truth predicate
 * 
 * @param node No effect
 * @param data No effect
 * @return true 
 */
bool fc_list_pred_true(fc_list_node node, void *data);

/**
 * @brief Built in total false predicate
 * 
 * @param node No effect
 * @param data No effect
 * @return false 
 */
bool fc_list_pred_false(fc_list_node node, void *data);

/**
 * @brief Default empty construction, do nothing
 * 
 * @param data Node data to be constructed
 * @return true success
 * @return false failed
 */
bool fc_list_node_data_construct_null(void *data);

/**
 * @brief Default empty deconstruction, do nothing
 * 
 * @param data Node data to be deconstruct
 * @return true success
 * @return false failed
 */
bool fc_list_node_data_deconstruct_null(void *data);

/*==================================================================================
    list operation function
==================================================================================*/

/**
 * @brief Create a linked list
 * 
 * @return fc_list_head References to linked lists
 */
fc_list_head fc_list_create(void);

/**
 * @brief Clear a linked list
 * 
 * @param l The linked list to be cleared
 * @param deconstruct Node data deconstruction callback function
 * @return true success
 * @return false failed
 */
bool fc_list_clear(fc_list_head l, fc_list_node_data_deconstruct_cb deconstruct);

/**
 * @brief Delete(free) a linked list
 * 
 * @param l The linked list to be delete
 * @param deconstruct Node data deconstruction callback function
 * @return true success
 * @return false failed
 */
bool fc_list_delete(fc_list_head l, fc_list_node_data_deconstruct_cb deconstruct);

/**
 * @brief Obtain the length of the linked list
 * 
 * @param l The linked list
 * @return size_t
 */
size_t fc_list_length(fc_list_head l);

/*==================================================================================
    internal operation function of linked list
==================================================================================*/

/**
 * @brief Find the last node
 * 
 * @param l The linked list
 * @return fc_list_node 
 *      nullptr     Not found or the linked list is empty
 *      other       found
 */
fc_list_node fc_list_find_last_node(fc_list_head l);

/**
 * @brief Find nodes through predicate
 * 
 * @param l The linked list to be searched
 * @param data The data provided to the predicate, related to search
 * @param pred Determine whether it is a predicate for a specified node
 * @return fc_list_node
 *      nullptr     not found
 *      other       found
 */
fc_list_node fc_list_find_if(fc_list_head l, void *data, fc_list_pred pred);

/**
 * @brief Push nodes through predicate
 * 
 * @details Make deep copies when creating nodes, without using data's memory
 * @param l The linked list to be pushed
 * @param data The copy source of node data, the node will be pushed
 * @param data_size The data's size, unit in bytes
 * @param pred The predicate
 * @param construct Node data construction callback function
 * @return true success
 * @return false failed
 */
bool fc_list_push_if(fc_list_head l, void *data, size_t data_size, fc_list_pred pred, fc_list_node_data_construct_cb construct);

/**
 * @brief Pop nodes through predicate
 * 
 * @details Releasing node memory will not release data's memory
 * @param l The linked list to be popped
 * @param data The data provided to the predicate, related to deletion
 * @param pred The predicate
 * @param deconstruct Node data deconstruction callback function
 * @return true success
 * @return false failed
 */
bool fc_list_pop_if(fc_list_head l, void *data, fc_list_pred pred, fc_list_node_data_deconstruct_cb deconstruct);

/**
 * @details Swap the logical positions of two nodes
 * 
 * @param l The linked list to be swapped
 * @param data1 Data with exchange node 1
 * @param data2 Data with exchange node 2
 * @param pred1 Determine if it is a predicate for node 1
 * @param pred2 Determine if it is a predicate for node 2
 */
bool fc_list_swap_if(fc_list_head l, void *data1, void *data2, fc_list_pred pred1, fc_list_pred pred2);

#endif  // !__FC_LIST_H__