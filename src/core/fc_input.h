#ifndef __FC_INPUT_H__
#define __FC_INPUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "./../exter/fuzzy_opera.h"
#include "./../exter/list.h"

#include "fc_core.h"
struct fuzzy_set;

/*==================================================================================
    micro function
==================================================================================*/

#define __FC_INPUT_PRINTF fc_printf
#define __FC_INPUT_MALLOC fc_malloc
#define __FC_INPUT_FREE fc_free
#define __FC_INPUT_STRLEN fc_strlen
#define __FC_INPUT_STRCPY_S fc_strcpy_s
#define __FC_INPUT_STRCMP fc_strcmp

/*==================================================================================
    structure
==================================================================================*/

struct fc_input
{
    const char* name;
    list_head fuzzy_set;
    struct fuzzy_matrix data;
};

/*==================================================================================
    API
==================================================================================*/

/**
 * @brief Register input
 * @note Do not use this function on fc_input that have already been registered
 * 
 * @param in object
 * @param name name
 * @return true success
 * @return false failed
 */
bool fc_input_register(struct fc_input* const in, const char* name);

/**
 * @brief Unregister input
 *
 * @param in object
 * @return true success
 * @return false failed
 */
bool fc_input_unregister(struct fc_input* const in);

/**
 * @brief Add a fuzzy subset to the input
 *
 * @param in object
 * @param set fuzzy set
 * @return true success
 * @return false failed
 */
bool fc_input_add_fuzzy_set(const struct fc_input* const in, const struct fuzzy_set* const set);

/**
 * @brief Clear the fuzzy subset in the input
 *
 * @param in object
 * @return true success
 * @return false failed
 */
bool fc_input_clear_fuzzy_set(const struct fc_input* const in);

/**
 * @brief Perform one fuzzification
 * 
 * @param in object
 * @param data accurate data
 * @param num accurate data number
 * @return true success
 * @return false failed
 */
bool fc_input_fuzzing(struct fc_input* const in, accurate_number* data, fc_size num);

/**
 * @brief Get a fuzzy number by label
 *
 * @param in object
 * @param label fuzzy set label
 * @param row row index
 * @return true success
 * @return false failed
 */
fuzzy_number fc_input_get_fuzzy_number_by_label(const struct fc_input* const in, const char *label, fc_index row);

/**
 * @brief Perform fuzzification by fuzzy set label
 *
 * @param in object
 * @param data accurate data
 * @param num accurate data number
 * @param fuzzy_data fuzzy data
 * @param label fuzzy set label
 * @return true success
 * @return false failed
 */
bool fc_input_fuzzing_by_label(
    const struct fc_input* const in,
    accurate_number* data, fc_size num,
    fuzzy_number* fuzzy_data,
    const char* label
);

/**
 * @brief Print blurred data
 * 
 * @param in object
 * @param label label
 * @return true success
 * @return false failed
 */
bool fc_input_print_data(const struct fc_input* const in, const char* label);

/**
 * @brief Print Fuzzy Sets
 * 
 * @param in object
 * @param label label
 * @return true success
 * @return false failed
 */
bool fc_input_print_fuzzy_set(const struct fc_input* const in, const char* label);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_INPUT_H__
