#ifndef __FC_CONTROLER_H__
#define __FC_CONTROLER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "./../exter/list.h"

#include "fc_core.h"
struct fc_input;
struct fc_output;
struct fc_rules;

/*==================================================================================
    micro function
==================================================================================*/

#define __FC_CONTROLER_PRINTF fc_printf
#define __FC_CONTROLER_MALLOC fc_malloc
#define __FC_CONTROLER_FREE fc_free

/*==================================================================================
    structure
==================================================================================*/

struct fc_controler
{
    list_head in;
    list_head out;
    struct fc_rules *rules;
};

/*==================================================================================
    API
==================================================================================*/

/**
 * @brief Register controler
 *
 * @param fcc object
 * @return true success
 * @return false failed
 */
bool fc_controler_register(struct fc_controler* fcc);

/**
 * @brief Unregister controler
 *
 * @param fcc object
 * @return true success
 * @return false failed
 */
bool fc_controler_unregister(struct fc_controler* fcc);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_CONTROLER_H__
