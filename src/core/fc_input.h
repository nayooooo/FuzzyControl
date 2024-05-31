#ifndef __FC_INPUT_H__
#define __FC_INPUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "fc_interface.h"

/*==================================================================================
    structure
==================================================================================*/

/**
 * @brief Fuzzy controller input components
 * 
 * @memberof Obj object
 * @memberof register_dev register
 * @memberof unregister_dev unregister
 * @memberof add_membership_fn add membership function
 * @memberof clear_membership_fn clear membership function
 * @memberof fuzzing fuzzing data
 * @memberof print print fuzzied data
 */
struct fc_input
{
    struct fc_interface obj;

    bool (*register_dev)(struct fc_input* in, const char* name);
    bool (*unregister_dev)(struct fc_input* in);
    bool (*add_membership_fn)(struct fc_input* in, fc_membership_fn fn);
    bool (*clear_membership_fn)(struct fc_input* in);
    bool (*fuzzing)(struct fc_input* in, accurate_number* value, fuzzy_size value_size);
    bool (*print)(struct fc_input* in);
};

/*==================================================================================
    API
==================================================================================*/

/**
 * @brief Register as input object for fuzzy controller
 * @note Never execute this function on input devices of fuzzy controllers
 *       that have already been registered. If you need to execute this
 *       function, please unregister it first
 * 
 * @param in object
 * @param name the label
 * @return true success
 * @return false failed
 */
bool fc_input_register(struct fc_input* in, const char* name);

/**
 * @brief Unregister the input device of the fuzzy controller
 * 
 * @param in object
 * @return true success
 * @return false failed
 */
bool fc_input_unregister(struct fc_input* in);

/**
 * @brief Add a membership function to the end
 * 
 * @param in object
 * @param fn membership function
 * @return true success
 * @return false failed
 */
bool fc_input_add_membership_fn(struct fc_input* in, fc_membership_fn fn);

/**
 * @brief Clear membership function
 * 
 * @param in object
 * @return true success
 * @return false failed
 */
bool fc_input_clear_membership_fn(struct fc_input* in);

/**
 * @brief Fuzzy the input data and store the obtained membership vector in member data
 * @note The previously generated membership vectors will be destroyed
 * 
 * @param in object
 * @param value Accurate sending buffer
 * @param value_size Accurate sending buffer size, determine the number of rows in a fuzzy matrix
 * @return true success
 * @return false failed
 */
bool fc_input_fuzzing(struct fc_input* in, accurate_number* value, fuzzy_size value_size);

/**
 * @brief Print out the membership vector
 * 
 * @param in object
 * @return true success
 * @return false failed
 */
bool fc_input_print_membership_vector(struct fc_input* in);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_INPUT_H__
