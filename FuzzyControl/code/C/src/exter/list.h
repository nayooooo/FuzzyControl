/**
 * @file list.h
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
 * @version 1.0.0
 * @date 2024-05-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <stdbool.h>
#include <stdint.h>

/*==================================================================================
    structure
==================================================================================*/

struct list
{
    void *data;
    struct list *next;
};
typedef struct list *list_node;
typedef struct list **list_head;

/*==================================================================================
    predicate and callback
==================================================================================*/

// Index type of linked list
typedef unsigned int list_index;

/**
 * @brief Predicate used to determine whether an operation will be executed
 * 
 * @param node The node currently being checked
 * @param data Data user gave
 * @return bool
 *      true        will execute
 *      false       not execute
 */
typedef bool (*list_pred)(list_node node, void *data);

/**
 * @brief Constructing callback functions for node data
 * 
 * @param data Created node data
 * @return true success
 * @return false failed
 */
typedef bool (*list_node_data_construct_cb)(void *data);

/**
 * @brief Deconstructive callback function for node data
 * 
 * @param data Node data to be deleted
 * @return true success
 * @return false failed
 */
typedef bool (*list_node_data_deconstruct_cb)(void *data);

/**
 * @brief event callback function
 * 
 * @param node The node currently being checked
 * @param data Data user gave
 * @return bool
 *      true        success
 *      false       failed
 */
typedef bool (*list_event_cb)(list_node node, void *data);

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
bool list_pred_true(list_node node, void *data);

/**
 * @brief Built in total false predicate
 * 
 * @param node No effect
 * @param data No effect
 * @return false 
 */
bool list_pred_false(list_node node, void *data);

/*==================================================================================
    list operation function
==================================================================================*/

/**
 * @brief Create a linked list
 * 
 * @return list_head References to linked lists
 */
list_head list_create(void);

/**
 * @brief Clear a linked list
 * @details This is not secure because custom node data may have a risk of memory leakage
 * 
 * @param l The linked list to be cleared
 * @param deconstruct Node data deconstruction callback function, if not required, fill in nullptr
 * @return true success
 * @return false failed
 */
bool list_clear(list_head l, list_node_data_deconstruct_cb deconstruct);

/**
 * @brief Delete(free) a linked list
 * @details This is not secure because custom node data may have a risk of memory leakage
 * 
 * @param l The linked list to be delete
 * @param deconstruct Node data deconstruction callback function, if not required, fill in nullptr
 * @return true success
 * @return false failed
 */
bool list_delete(list_head l, list_node_data_deconstruct_cb deconstruct);

/**
 * @brief Obtain the length of the linked list
 * 
 * @param l The linked list
 * @return list_index
 */
list_index list_length(list_head l);

/*==================================================================================
    internal operation function of linked list
==================================================================================*/

/**
 * @brief Determine whether a node is linked to a linked list
 * 
 * @param l The linked list
 * @param n Nodes to be determined
 * @return true if on list
 * @return false not on list or the list and node at least one of them is illegal
 */
bool list_node_is_on_list(list_head l, list_node n);

/**
 * @brief Find the previous node
 * 
 * @param l The linked list
 * @param n Find based on this node
 * @param list_node
 *      nullptr     not found, this is an empty/illegal linked list
 *                  or this node is not in the linked list(or node is illegal)
 *                  or this node is the 1st node in the list
 *      others      found
 */
list_node list_find_prev_node(list_head l, list_node n);

/**
 * @brief Find the next node
 *
 * @param l The linked list
 * @param n Find based on this node
 * @param list_node
 *      nullptr     not found, this is an empty/illegal linked list
 *                  or this node is not in the linked list(or node is illegal)
 *                  or this node is the last node in the list
 *      others      found
 */
list_node list_find_next_node(list_head l, list_node n);

/**
 * @brief Push nodes
 * @details Make deep copies when creating nodes, without using data's memory
 * 
 * @param l The linked list to be pushed
 * @param data The copy source of node data, the node will be pushed
 * @param data_size The data's size, unit in bytes
 * @return true success
 * @return false failed
 */
bool list_push(list_head l, void* data, size_t data_size);

/**
 * @brief Pop nodes
 * @details This is not secure because custom node data may have a risk of memory leakage
 * 
 * @param l The linked list to be popped
 * @return true success
 * @return false failed
 */
bool list_pop(list_head l);

/**
 * @brief Find nodes through predicate
 * @details you can use `fc_list_find_if(l, null, list_pred_true)` to get the 1st node
 * 
 * @param l The linked list to be searched
 * @param data The data provided to the predicate, related to search
 * @param pred Determine whether it is a predicate for a specified node, you must give a predicate
 * @return list_node
 *      nullptr     not found or the list is illegal
 *                  or not have a predicate
 *                  or the list is empty
 *      others      found
 */
list_node list_find_if(list_head l, void *data, list_pred pred);

/**
 * @brief Push nodes through predicate
 * @details Make deep copies when creating nodes, without using data's memory
 * 
 * @param l The linked list to be pushed
 * @param data The copy source of node data, the node will be pushed
 * @param data_size The data's size, unit in bytes
 * @param pred The predicate, receiving the last node and incoming data, if not required, fill in nullptr
 * @param construct Node data construction callback function, if not required, fill in nullptr
 * @return true success
 * @return false failed
 */
bool list_push_if(list_head l, void *data, size_t data_size, list_pred pred, list_node_data_construct_cb construct);

/**
 * @brief Pop nodes through predicate
 * @details Releasing node memory will not release data's memory
 * 
 * @param l The linked list to be popped
 * @param data The data provided to the predicate, related to deletion, if not required, fill in nullptr
 * @param pred The predicate, receiving the last node and incoming data, if not required, fill in nullptr
 * @param deconstruct Node data deconstruction callback function, if not required, fill in nullptr
 * @return true success
 * @return false failed
 */
bool list_pop_if(list_head l, void *data, list_pred pred, list_node_data_deconstruct_cb deconstruct);

/**
 * @brief Remove a node
 * @details This is not secure because custom node data may have a risk of memory leakage
 * 
 * @param l The linked list to be removed
 * @param data The data provided to the predicate, related to deletion
 * @param pred Determine whether it is a predicate for a specified node, you must give a predicate
 * @param deconstruct Node data deconstruction callback function
 * @return true success
 * @return false failed
 */
bool list_remove_if(list_head l, void *data, list_pred pred, list_node_data_deconstruct_cb deconstruct);

/**
 * @details Swap the logical positions of two nodes
 * 
 * @param l The linked list to be swapped
 * @param data1 Data with exchange node 1, you must give a node
 * @param data2 Data with exchange node 2, you must give a node
 * @param pred1 Determine if it is a predicate for node 1, you must give a predicate
 * @param pred2 Determine if it is a predicate for node 2, you must give a predicate
 * @return true Returns true only when the exchange is successful or when the same node is passed in
 * @return false
 */
bool list_swap_if(list_head l, void *data1, void *data2, list_pred pred1, list_pred pred2);

/**
 * @brief Traversing linked lists
 * 
 * @param l The linked list to be traversed
 * @param data user data
 * @param event_cb Event callbacks that each node needs to execute, you must give
 * @param true      success
 * @param false     failed
 */
bool list_trav(list_head l, void *data, list_event_cb event_cb);

/*==================================================================================
    parameter acquisition API
==================================================================================*/

/**
 * @brief Find the first node
 *
 * @param l The linked list
 * @return list_node
 *      nullptr     The linked list is illegal or the linked list is empty
 *      others      found
 */
list_node list_get_first_node(list_head l);

/**
 * @brief Find the last node
 *
 * @param l The linked list
 * @return list_node
 *      nullptr     The linked list is illegal or the linked list is empty
 *      others      found
 */
list_node list_get_last_node(list_head l);

/**
 * @brief Obtain data for the node at the specified index
 * 
 * @param l The linked list
 * @param ind index(0,1,2,...,max(list_index))
 * @return void*
 *      nullptr     Index out of bounds or the list is illegal
 *      others      The data of the node at the index
 */
void* list_get_node_data(list_head l, list_index ind);

/**
 * @brief Obtain data at a specified node through a predicate
 * 
 * @param l The linked list
 * @param data user data
 * @param pred Predicate used to determine whether it is a specified node, you must give
 * @return void*
 *      nullptr     not found or the list is illegal
 *                  or not have predicate
 *                  or the list is empty
 *      others      The data of the node
 */
void* list_get_node_data_if(list_head l, void *data, list_pred pred);

#endif  // !__LIST_H__


