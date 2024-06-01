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
 * @details The linked list in the interface is used to store the membership
 *          function and its labels(struct membership_fn_label)
 * 
 * @memberof interface object
 * @memberof register_dev register
 * @memberof unregister_dev unregister
 * @memberof add_membership_fn add membership function
 * @memberof clear_membership_fn clear membership function
 * @memberof fuzzing fuzzing data
 * @memberof print print fuzzied data
 */
struct fc_input
{
    struct fc_interface interface;

    bool (*register_dev)(struct fc_input* const in, const char* name);
    bool (*unregister_dev)(struct fc_input* const in);
    bool (*add_membership_fn)(const struct fc_input* const in, const fc_membership_fn fn, const char* label);
    bool (*clear_membership_fn)(const struct fc_input* const in);
    bool (*fuzzing)(struct fc_input* const in, const accurate_number* const value, const fuzzy_size value_size);
    bool (*print)(const struct fc_input* const in);
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
bool fc_input_register(struct fc_input* const in, const char* name);

/**
 * @brief Unregister the input device of the fuzzy controller
 * 
 * @param in object
 * @return true success
 * @return false failed
 */
bool fc_input_unregister(struct fc_input* const in);

/**
 * @brief Add a membership function to the end
 * @note Deep copy incoming fn, But what is linked to the linked list
 *       here is the membership index function pair
 * 
 * @param in object
 * @param fn membership function
 * @param label fn's label
 * @return true success
 * @return false failed
 */
bool fc_input_add_membership_fn(const struct fc_input* const in, const fc_membership_fn fn, const char* label);

/**
 * @brief Clear membership function
 * 
 * @param in object
 * @return true success
 * @return false failed
 */
bool fc_input_clear_membership_fn(const struct fc_input* const in);

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
bool fc_input_fuzzing(struct fc_input* const in, const accurate_number* const value, const fuzzy_size value_size);

/**
 * @brief Print out the membership vector
 * 
 * @param in object
 * @return true success
 * @return false failed
 */
bool fc_input_print_membership_vector(const struct fc_input* const in);

/**
 * @brief Print out the membership vector with label
 *
 * @param in object
 * @return true success
 * @return false failed
 */
bool fc_input_print_membership_vector_with_label(const struct fc_input* const in);

/*==================================================================================
    API for obtaining parameters
==================================================================================*/

/**
 * @brief Obtain the name of the fuzzy matrix in the input component
 *
 * @param in object
 * @return const char*
 */
const char* fc_input_get_name(const struct fc_input* const in);

/**
 * @brief Get labels from input components
 * @note Retrieve the label from the input device. If the given parameter
 *       label is not empty, it will be cleared, even though the label in
 *       the input component is also empty
 * 
 * @param in object
 * @param label label, must be an empty linked list, otherwise it will be
 *              destroyed
 * @return true success
 * @return false failed
 */
bool fc_input_get_label(const struct fc_input* const in, const list_head label);

/**
 * @brief Obtaining Fuzzy Data from Input Components
 * 
 * @param in object
 * @param data fuzzy data receive buff
 * @return true success
 * @return false failed
 */
bool fc_input_get_fuzzy_data(const struct fc_input* const in, struct fuzzy_matrix* data);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_INPUT_H__
