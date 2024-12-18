#ifndef __FC_CONTROLER_H__
#define __FC_CONTROLER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "fc_core.h"

/*==================================================================================
    structure
==================================================================================*/

struct fc_controler
{
    struct fc_input in;
    struct fc_output out;
    struct fc_rules rules;
};

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_CONTROLER_H__
