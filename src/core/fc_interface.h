#ifndef __FC_INTERFACE_H__
#define __FC_INTERFACE_H__

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

/**
 * @brief Fuzzy controller interface components
 * 
 * @memberof Obj object
 * @memberof data fuzzy Data
 * @memberof l list
 */
struct fc_interface
{
    struct fc_obj obj;

    struct fuzzy_matrix data;
    list_head l;
};

/*==================================================================================
    API
==================================================================================*/

/**
 * @brief Register as interface object for fuzzy controller
 * @note Never execute this function on interface devices of fuzzy controllers
 *       that have already been registered. If you need to execute this
 *       function, please unregister it first
 * 
 * @param interface object
 * @param name the label
 * @param type interface type
 * @return true success
 * @return false failed
 */
bool fc_interface_register(struct fc_interface* const interface, const char* name, const fc_obj_type type);

/**
 * @brief Unregister the interface device of the fuzzy controller
 * 
 * @param interface object
 * @return true success
 * @return false failed
 */
bool fc_interface_unregister(struct fc_interface* const interface);

/**
 * @brief Print out the data
 * 
 * @param interface object
 * @return true success
 * @return false failed
 */
bool fc_interface_print_data(const struct fc_interface* const interface);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_INTERFACE_H__
