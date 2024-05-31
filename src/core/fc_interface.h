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
 * @brief Membership Index and Function Pairs
 * 
 * @memberof index membership index
 * @memberof fn membership fn
 */
struct membership_index_fn
{
    list_index index;
    fc_membership_fn fn;
};

/**
 * @brief Fuzzy controller interface components
 * 
 * @memberof Obj object
 * @memberof data fuzzy Data
 * @memberof l Membership index and function linked list
 *                                The membership index starts from 0
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
bool fc_interface_register(struct fc_interface* interface, const char* name, fc_obj_type type);

/**
 * @brief Unregister the interface device of the fuzzy controller
 * 
 * @param interface object
 * @return true success
 * @return false failed
 */
bool fc_interface_unregister(struct fc_interface* interface);

/*---------------------------------- Only supports input devices start ----------------------------------*/

/**
 * @brief Add a membership function to the end
 * @note Only supports input devices
 * 
 * @param interface object
 * @param fn membership function
 * @param type interface type
 * @return true success
 * @return false failed
 */
bool fc_interface_add_membership_fn(struct fc_interface* interface, fc_membership_fn fn, fc_obj_type type);

/**
 * @brief Clear membership function
 * @note Only supports input devices
 * 
 * @param interface object
 * @param type interface type
 * @return true success
 * @return false failed
 */
bool fc_interface_clear_membership_fn(struct fc_interface* interface, fc_obj_type type);

/**
 * @brief Fuzzy the input data and store the obtained membership vector in member data
 * @note The previously generated membership vectors will be destroyed
 *       Only supports input devices
 * 
 * @param interface object
 * @param value Accurate sending buffer
 * @param value_size Accurate sending buffer size, determine the number of rows in a fuzzy matrix
 * @return true success
 * @return false failed
 */
bool fc_interface_fuzzing(struct fc_interface* interface, accurate_number* value, fuzzy_size value_size);

/*----------------------------------- Only supports input devices end -----------------------------------*/

/*--------------------------------- Only supports output devices start ----------------------------------*/

/**
 * @brief Perform anti fuzzing on the output data and return the exact value obtained
 * @note Only supports output devices
 * 
 * @param interface object
 * @param value Accurate receive buffer
 * @param value_size Accurate receive buffer size
 * @return true success
 * @return false failed
 */
bool fc_interface_unfuzzing(struct fc_interface* interface, accurate_number* value, fuzzy_size value_size);

/*---------------------------------- Only supports output devices end -----------------------------------*/

/**
 * @brief Print out the membership vector
 * 
 * @param interface object
 * @return true success
 * @return false failed
 */
bool fc_interface_print_membership_vector(struct fc_interface* interface);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_INTERFACE_H__
