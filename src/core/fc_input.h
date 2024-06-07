#ifndef __FC_INPUT_H__
#define __FC_INPUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "./../exter/fuzzy_opera.h"
#include "./../exter/list.h"

/*==================================================================================
    structure
==================================================================================*/

struct fc_input
{
    const char* name;
    struct fuzzy_matrix data;
    list_head fuzzy_set;
};

/*==================================================================================
    API
==================================================================================*/

bool fc_input_register(struct fc_input* const in, const char* name);

bool fc_input_unregister(struct fc_input* const in);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_INPUT_H__