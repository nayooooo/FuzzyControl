#include "fc_list.h"
#include "fc_std.h"

bool fc_list_pred_true(fc_list_node node, void *data)
{
    UNUSED(node);
    UNUSED(data);
    return true;
}

bool fc_list_pred_false(fc_list_node node, void *data)
{
    UNUSED(node);
    UNUSED(data);
    return false;
}

fc_list_head fc_list_create(void)
{
    fc_list_head l = (fc_list_head)fc_malloc(sizeof(fc_list_node));
    if (l == nullptr) return nullptr;

    *l = nullptr;

    return l;
}

bool fc_list_clear(fc_list_head l, fc_list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;

    while (fc_list_pop_if(l, nullptr, nullptr, deconstruct)) ;

    return false;
}

bool fc_list_delete(fc_list_head l, fc_list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;

    if (!fc_list_clear(l, deconstruct)) return false;

    fc_free(l);

    return true;
}

size_t fc_list_length(fc_list_head l)
{
    if (l == nullptr) return 0;
    
    fc_list_node n = *l;
    size_t length = 0;
    while (n)
    {
        length++;
        n = n->next;
    }

    return length;
}

bool fc_list_node_is_on_list(fc_list_head l, fc_list_node n)
{
    if (l == nullptr) return false;
    if (n == nullptr) return false;

    fc_list_node cn = *l;
    while (cn)
    {
        if (cn == n) return true;
        cn = cn->next;
    }

    return false;
}

fc_list_node fc_list_find_last_node(fc_list_head l)
{
    if (l == nullptr) return nullptr;

    fc_list_node n = *l;
    fc_list_node fn = nullptr;
    while (n)
    {
        fn = n;
        n = n->next;
    }
    return fn;
}

fc_list_node fc_list_find_prev_node(fc_list_head l, fc_list_node n)
{
    if (l == nullptr) return nullptr;
    if (n == nullptr) return nullptr;

    fc_list_node cn = *l;  // current node
    fc_list_node fcn = nullptr;  // the cn's front node, if the 1st node is n, then reture nullptr
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

fc_list_node fc_list_find_next_node(fc_list_head l, fc_list_node n)
{
    if (l == nullptr) return nullptr;
    if (n == nullptr) return nullptr;

    fc_list_node cn = *l;  // current node
    fc_list_node ncn = nullptr;  // the cn's next node, if the list is empty then return nullptr

    while (cn)
    {
        ncn = cn->next;
        if (cn == n) break;
        cn = ncn;
    }

    return ncn;
}

bool fc_list_push(fc_list_head l, void* data, size_t data_size)
{
    if (l == nullptr) return false;
    if (data == nullptr) return false;
    if (data_size <= 0) return false;
    
    // Find the last node
    fc_list_node ln = fc_list_find_last_node(l);

    // Create a new node
    fc_list_node n = (fc_list_node)fc_malloc(sizeof(struct fc_list));
    if (n == nullptr) return false;
    n->data = (void*)fc_malloc(data_size);
    if (n->data == nullptr)
    {
        fc_free(n);
        return false;
    }
    fc_memcpy(n->data, data, data_size);
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

bool fc_list_pop(fc_list_head l)
{
    if (l == nullptr) return false;

    // Find the last node and determine whether to pop it
    fc_list_node ln = fc_list_find_last_node(l);
    if (ln == nullptr) return false;  // The list is empty, can not pop node

    fc_list_node fln = fc_list_find_prev_node(l, ln);
    if (fln == nullptr) *l = nullptr;  // the list has only 1 node
    else fln->next = nullptr;
    fc_free(ln->data);
    fc_free(ln);

    return true;
}

fc_list_node fc_list_find_if(fc_list_head l, void *data, fc_list_pred pred)
{
    if (l == nullptr) return nullptr;
    if (pred == nullptr) return nullptr;

    fc_list_node cn = *l;
    while (cn)
    {
        if (pred(cn, data)) break;
        cn = cn->next;
    }

    return cn;
}

bool fc_list_push_if(fc_list_head l, void *data, size_t data_size, fc_list_pred pred, fc_list_node_data_construct_cb construct)
{
    if (l == nullptr) return false;
    if (data == nullptr) return false;
    if (data_size <= 0) return false;

    // Find the last node and determine whether to create a new node
    fc_list_node ln = fc_list_find_last_node(l);
    if (pred != nullptr) if (!pred(ln, data)) return false;

    // Push node and construct it
    if (!fc_list_push(l, data, data_size)) return false;
    ln = fc_list_find_last_node(l);
    if (construct != nullptr) construct(ln->data);

    return true;
}

bool fc_list_pop_if(fc_list_head l, void *data, fc_list_pred pred, fc_list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;

    // Find the last node and determine whether to pop the node
    fc_list_node ln = fc_list_find_last_node(l);
    if (pred != nullptr) if (!pred(ln, data)) return false;
    if (deconstruct != nullptr) if (!deconstruct(ln->data)) return false;

    return fc_list_pop(l);
}

bool fc_list_remove_if(fc_list_head l, void* data, fc_list_pred pred, fc_list_node_data_deconstruct_cb deconstruct)
{
    if (l == nullptr) return false;
    if (pred == nullptr) return false;

    fc_list_node rn = fc_list_find_if(l, data, pred);  // find the node will be removed
    // There are two situations: the linked list is empty
    // or there are no matching nodes in the linked list
    if (rn == nullptr)
        return false;
    if (deconstruct != nullptr) if (!deconstruct(rn->data)) return false;
    // to find the front node
    // if frn is nullptr, then the rn is the 1st node in the list
    // others, it must not be nullptr
    fc_list_node frn = fc_list_find_prev_node(l, rn);
    // to find the next node
    // if nrn is nullptr, then the rn is the last node in the list
    // others, it must not be nullptr
    fc_list_node nrn = fc_list_find_next_node(l, rn);

    //if (frn == nullptr && nrn != nullptr)  // rn is the 1st
    //{
    //    *l = nrn;
    //}
    //else if (frn != nullptr && nrn == nullptr)  // rn is the last
    //{
    //    frn->next = nullptr;
    //}
    //else if (frn != nullptr && nrn != nullptr)  // rn in mid
    //{
    //    frn->next = nrn;
    //}
    //else/* if (frn == nullptr && nrn == nullptr) */  // only 1 node
    //{
    //    *l = nullptr;
    //}
    if (frn == nullptr) *l = nrn;
    else frn->next = nrn;
    fc_free(rn->data);
    fc_free(rn);

    return true;
}

bool fc_list_swap_if(fc_list_head l, void *data1, void *data2, fc_list_pred pred1, fc_list_pred pred2)
{
    if (l == nullptr) return false;
    if (data1 == nullptr || data2 == nullptr) return false;
    if (pred1 == nullptr || pred2 == nullptr) return false;

    fc_list_node n1 = fc_list_find_if(l, data1, pred1);
    fc_list_node n2 = fc_list_find_if(l, data2, pred2);
    if (n1 == nullptr || n2 == nullptr) return false;
    if (n1 == n2) return true;  // The same node does not need to be swapped

    // exchange nodes
    fc_list_node fn1 = fc_list_find_prev_node(l, n1);
    fc_list_node fn2 = fc_list_find_prev_node(l, n2);
    if (fn1 == nullptr) *l = n2;
    else fn1->next = n2;
    if (fn2 == nullptr) *l = n1;
    else fn2->next = n1;
    fc_list_node temp = n1->next;
    n1->next = n2->next;
    n2->next = temp;

    return true;
}

bool fc_list_trav(fc_list_head l, void* data, fc_list_event_cb event_cb)
{
    if (l == nullptr) return false;
    if (event_cb == nullptr) return false;

    fc_list_node n = fc_list_find_if(l, nullptr, fc_list_pred_true);
    while (n)
    {
        if (!event_cb(n, data)) return false;
        n = n->next;
    }

    return true;
}
