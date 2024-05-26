#include "list.h"

#ifndef nullptr
#   define nullptr ((void*)0)
#endif

#ifndef UNUSED
#   define UNUSED(x) ((void)x)
#endif

#include <stdlib.h>
#include <string.h>

#define __list_malloc malloc
#define __list_free free

#define __list_memcpy memcpy

bool list_pred_true(list_node node, void *data)
{
    UNUSED(node);
    UNUSED(data);
    return true;
}

bool list_pred_false(list_node node, void *data)
{
    UNUSED(node);
    UNUSED(data);
    return false;
}

list_head list_create(void)
{
    list_head l = (list_head)__list_malloc(sizeof(list_node));
    if (l == nullptr) return nullptr;

    *l = nullptr;

    return l;
}

bool list_clear(list_head l, list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;

    while (list_pop_if(l, nullptr, nullptr, deconstruct)) ;

    return true;
}

bool list_delete(list_head l, list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;

    if (!list_clear(l, deconstruct)) return false;

    __list_free(l);
    l = nullptr;

    return true;
}

list_index list_length(list_head l)
{
    if (l == nullptr) return 0;
    
    list_node n = *l;
    list_index length = 0;
    while (n)
    {
        length++;
        n = n->next;
    }

    return length;
}

bool list_node_is_on_list(list_head l, list_node n)
{
    if (l == nullptr) return false;
    if (n == nullptr) return false;

    list_node cn = *l;
    while (cn)
    {
        if (cn == n) return true;
        cn = cn->next;
    }

    return false;
}

list_node list_find_prev_node(list_head l, list_node n)
{
    if (l == nullptr) return nullptr;
    if (n == nullptr) return nullptr;

    list_node cn = *l;  // current node
    list_node fcn = nullptr;  // the cn's front node, if the 1st node is n, then reture nullptr
    while (cn)
    {
        if (cn == n) break;
        fcn = cn;
        cn = cn->next;
    }

    // if the list is empty or fcn is the last node, explain the list no link node n
    if (cn == nullptr) return nullptr;

    return fcn;
}

list_node list_find_next_node(list_head l, list_node n)
{
    if (l == nullptr) return nullptr;
    if (n == nullptr) return nullptr;

    list_node cn = *l;  // current node
    list_node ncn = nullptr;  // the cn's next node, if the list is empty then return nullptr

    while (cn)
    {
        ncn = cn->next;
        if (cn == n) break;
        cn = ncn;
    }

    return ncn;
}

bool list_push(list_head l, void* data, size_t data_size)
{
    if (l == nullptr) return false;
    if (data == nullptr) return false;
    if (data_size <= 0) return false;
    
    // Find the last node
    list_node ln = list_get_last_node(l);

    // Create a new node
    list_node n = (list_node)__list_malloc(sizeof(struct list));
    if (n == nullptr) return false;
    n->data = (void*)__list_malloc(data_size);
    if (n->data == nullptr)
    {
        __list_free(n);
        n = nullptr;
        return false;
    }
    __list_memcpy(n->data, data, data_size);
    n->next = nullptr;

    // Link the created node to the end of the linked list
    if (ln == nullptr)
    {
        *l = n;
    }
    else
    {
        ln->next = n;
    }

    return true;
}

bool list_pop(list_head l)
{
    if (l == nullptr) return false;

    // Find the last node and determine whether to pop it
    list_node ln = list_get_last_node(l);
    if (ln == nullptr) return false;  // The list is empty, can not pop node

    list_node fln = list_find_prev_node(l, ln);
    if (fln == nullptr) *l = nullptr;  // the list has only 1 node
    else fln->next = nullptr;
    __list_free(ln->data);
    __list_free(ln);
    ln->data = nullptr;
    ln = nullptr;

    return true;
}

list_node list_find_if(list_head l, void *data, list_pred pred)
{
    if (l == nullptr) return nullptr;
    if (pred == nullptr) return nullptr;

    list_node cn = *l;
    while (cn)
    {
        if (pred(cn, data)) break;
        cn = cn->next;
    }

    return cn;
}

bool list_push_if(list_head l, void *data, size_t data_size, list_pred pred, list_node_data_construct_cb construct)
{
    if (l == nullptr) return false;
    if (data == nullptr) return false;
    if (data_size <= 0) return false;

    // Find the last node and determine whether to create a new node
    list_node ln = list_get_last_node(l);
    if (pred != nullptr) if (!pred(ln, data)) return false;

    // Push node and construct it
    if (!list_push(l, data, data_size)) return false;
    ln = list_get_last_node(l);
    if (construct != nullptr) construct(ln->data);

    return true;
}

bool list_pop_if(list_head l, void *data, list_pred pred, list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;

    // Find the last node and determine whether to pop the node
    list_node ln = list_get_last_node(l);
    if (pred != nullptr) if (!pred(ln, data)) return false;
    if (deconstruct != nullptr) if (!deconstruct(ln->data)) return false;

    return list_pop(l);
}

bool list_remove_if(list_head l, void* data, list_pred pred, list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;
    if (pred == nullptr) return false;

    list_node rn = list_find_if(l, data, pred);  // find the node will be removed
    // There are two situations: the linked list is empty
    // or there are no matching nodes in the linked list
    if (rn == nullptr)
        return false;
    if (deconstruct != nullptr) if (!deconstruct(rn->data)) return false;
    // to find the front node
    // if frn is nullptr, then the rn is the 1st node in the list
    // others, it must not be nullptr
    list_node frn = list_find_prev_node(l, rn);
    list_node nrn = list_find_next_node(l, rn);

    if (frn == nullptr) *l = nrn;
    else frn->next = nrn;
    __list_free(rn->data);
    __list_free(rn);
    rn->data = nullptr;
    rn = nullptr;

    return true;
}

bool list_swap_if(list_head l, void *data1, void *data2, list_pred pred1, list_pred pred2)
{
    if (l == nullptr) return false;
    if (data1 == nullptr || data2 == nullptr) return false;
    if (pred1 == nullptr || pred2 == nullptr) return false;

    list_node n1 = list_find_if(l, data1, pred1);
    list_node n2 = list_find_if(l, data2, pred2);
    if (n1 == nullptr || n2 == nullptr) return false;
    if (n1 == n2) return true;  // The same node does not need to be swapped

    // exchange nodes
    list_node fn1 = list_find_prev_node(l, n1);
    list_node fn2 = list_find_prev_node(l, n2);
    if (fn1 == nullptr) *l = n2;
    else fn1->next = n2;
    if (fn2 == nullptr) *l = n1;
    else fn2->next = n1;
    list_node temp = n1->next;
    n1->next = n2->next;
    n2->next = temp;

    return true;
}

bool list_trav(list_head l, void* data, list_event_cb event_cb)
{
    if (l == nullptr) return false;
    if (event_cb == nullptr) return false;

    list_node n = list_find_if(l, nullptr, list_pred_true);
    while (n)
    {
        if (!event_cb(n, data)) return false;
        n = n->next;
    }

    return true;
}

list_node list_get_first_node(list_head l)
{
    if (l == nullptr) return nullptr;
    return *l;
}

list_node list_get_last_node(list_head l)
{
    if (l == nullptr) return nullptr;

    list_node n = *l;
    list_node fn = nullptr;
    while (n)
    {
        fn = n;
        n = n->next;
    }
    return fn;
}

void* list_get_node_data(list_head l, list_index ind)
{
    if (l == nullptr) return nullptr;
    if (ind >= list_length(l)) return nullptr;

    list_node cn = list_get_first_node(l);
    for (list_index i = 0; i < ind; i++)
    {
        cn = cn->next;
    }

    return cn->data;
}

void* list_get_node_data_if(list_head l, void* data, list_pred pred)
{
    if (l == nullptr) return nullptr;
    if (pred == nullptr) return nullptr;

    if (list_length(l) == 0) return nullptr;

    list_node n = list_find_if(l, data, pred);
    if (n == nullptr) return nullptr;  // list is empty

    return n->data;
}
