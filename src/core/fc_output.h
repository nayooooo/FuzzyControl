#ifndef __FC_OUTPUT_H__
#define __FC_OUTPUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

#include "fc_obj.h"

#include "./../exter/fuzzy_opera.h"
#include "./../exter/list.h"

/*==================================================================================
    structure
==================================================================================*/

struct fc_output
{
    struct fc_obj obj;

    struct fuzzy_matrix data;
    list_head membership_fns;
};

/*==================================================================================
    API
==================================================================================*/

bool fc_output_register(struct fc_output* out);

bool fc_output_unregister(struct fc_output* out);

bool fc_output_add_membership_fn(struct fc_output* out, fc_membership_fn fn);

bool fc_output_clear_membership_fn(struct fc_output* out);

accurate_number fc_output_unfuzzing(struct fc_output* out);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif // !__FC_OUTPUT_H__
