#ifndef __FC_LIST_H__
#define __FC_LIST_H__

struct fc_list{
    void *userdata;
    fc_list *next;
};
typedef struct fc_list *fc_list_t;

#endif  // !__FC_LIST_H__