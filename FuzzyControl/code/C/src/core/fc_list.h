#ifndef __FC_LIST_H__
#define __FC_LIST_H__

#include <stdbool.h>
#include "fc_def.h"

struct fc_list
{
    void *userdata;
    struct fc_list *next;
};

/**
 * @brief Predicate used to determine whether an operation will be executed
 * @param node The node currently being checked
 * @param data Data to be matched
 * @return bool
 *      true        will execute
 *      false       not execute
*/
typedef bool (*fc_list_pred)(struct fc_list *node, void *data);

/**
 * @brief Find nodes through predicate
 * @param l The linked list to be searched
 * @param pred The predicate
 * @return struct fc_list*
 *      nullptr     not found
 *      other       found
*/
struct fc_list *fc_list_find_if(struct fc_list *l, fc_list_pred pred);

/**
 * @brief Push nodes through predicate
 * @param l The linked list to be pushed
 * @param pred The predicate
 * @return bool
 *      true        success
 *      false       failed
*/
bool fc_list_push_if(struct fc_list *l, fc_list_pred pred);

/**
 * @brief Pop nodes through predicate
 * @param l The linked list to be popped
 * @param pred The predicate
 * @return bool
 *      true        success
 *      false       failed
*/
bool fc_list_pop_if(struct fc_list *l, fc_list_pred pred);

#endif  // !__FC_LIST_H__