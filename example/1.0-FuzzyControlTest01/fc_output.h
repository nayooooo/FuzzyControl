#ifndef __FC_OUTPUT_H__
#define __FC_OUTPUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "fuzzy_opera.h"
#include "list.h"

#include "fc_core.h"
struct fuzzy_set;
struct inference_result;

/*==================================================================================
    micro function
==================================================================================*/

#define __FC_OUTPUT_PRINTF fc_printf
#define __FC_OUTPUT_MALLOC fc_malloc
#define __FC_OUTPUT_FREE fc_free
#define __FC_OUTPUT_STRLEN fc_strlen
#define __FC_OUTPUT_STRCPY_S fc_strcpy_s
#define __FC_OUTPUT_STRCMP fc_strcmp

/*==================================================================================
    enum
==================================================================================*/

typedef enum{
    FC_OUTPUT_MAX_VAL_METHOD,
    FC_OUTPUT_MOM_METHOD,               // Mean of Maximum
    FC_OUTPUT_AREA_CENTER_METHOD,
} fc_output_unfuzzy_method_t;

/*==================================================================================
    structure
==================================================================================*/

struct fc_output
{
    const char* name;
    list_head data;             // to save inference result groups
    list_head fuzzy_set;
    accurate_number range[2];   // output range (minimum, maximum)
    accurate_number step;       // step value
};

/*==================================================================================
    type
==================================================================================*/

/**
 * @brief Combine the activation degree list and membership function list to
 *        obtain the output result.
 * 
 * @param irh inference result list
 * @param fsh fuzzy set list
 * @param min minimum output value
 * @param max maximum output value
 * @param step step value
 * @return accurate_number result
 */
typedef accurate_number (*fc_output_unfuzzy_method_fn)(list_head irh, list_head fsh, accurate_number min, accurate_number max, accurate_number step);

/*==================================================================================
    API
==================================================================================*/

/**
 * @brief Register output
 * @note Do not use this function on fc_output that have already been registered
 * 
 * @param out object
 * @param name name
 * @param min minimum output value
 * @param max maximum output value
 * @param step step value
 * @return true success
 * @return false failed
 */
bool fc_output_register(struct fc_output* const out, const char* name, accurate_number min, accurate_number max, accurate_number step);

/**
 * @brief Unregister output
 *
 * @param out object
 * @return true success
 * @return false failed
 */
bool fc_output_unregister(struct fc_output* const out);

/**
 * @brief Add a fuzzy subset to the output
 *
 * @param out object
 * @param set fuzzy set
 * @return true success
 * @return false failed
 */
bool fc_output_add_fuzzy_set(const struct fc_output* const out, const struct fuzzy_set* const set);

/**
 * @brief Clear the fuzzy subset in the output
 *
 * @param out object
 * @return true success
 * @return false failed
 */
bool fc_output_clear_fuzzy_set(const struct fc_output* const out);

/**
 * @brief Increase a inference result group
 *
 * @param out object
 * @return true success
 * @return false failed
 */
bool fc_output_increase_a_inference_result_group(const struct fc_output* const out);

/**
 * @brief Decrease a inference result group
 *
 * @param out object
 * @return true success
 * @return false failed
 */
bool fc_output_decrease_a_inference_result_group(const struct fc_output* const out);

/**
 * @brief Delete all inference result group
 *
 * @param out object
 * @return true success
 * @return false failed
 */
bool fc_output_delete_all_inference_result_group(const struct fc_output* const out);

/**
 * @brief Get inference result groups number in the output
 *
 * @param out object
 * @return fc_size
 */
fc_size fc_output_get_group_num(const struct fc_output* const out);

/**
 * @brief Add inference results to the specified group in the output
 * 
 * @param out object
 * @param ind inference result group index, must be >= 0
 * @param ir inference result
 * @return true success
 * @return false failed
 */
bool fc_output_add_inference_result(const struct fc_output* const out, fc_index ind, const struct inference_result* const ir);

/**
 * @brief Clear all inference results from specified group in the output
 *
 * @param out object
 * @param ind inference result group index, must be >= 0, if -1, clear all inference result from all groups
 * @return true success
 * @return false failed
 */
bool fc_output_clear_inference_result(const struct fc_output* const out, fc_index ind);

/**
 * @brief Get inference result number from specified group in the output
 *
 * @param out object
 * @param ind inference result group index, must be >= 0
 * @return fc_size
 */
fc_size fc_output_get_inference_result_num(const struct fc_output* const out, fc_index ind);

/**
 * @brief Print Fuzzy Sets
 *
 * @param out object
 * @param label label
 * @return true success
 * @return false failed
 */
bool fc_output_print_fuzzy_set(const struct fc_output* const out, const char* label);

/**
 * @brief Print inference result
 *
 * @param out object
 * @param ind inference result group index, must be >= 0
 * @param label label
 * @return true success
 * @return false failed
 */
bool fc_output_print_inference_result(const struct fc_output* const out, fc_index ind, const char* label);

/**
 * @brief Unfuzzied data
 * @note Please ensure that the inference result group and membership
 *       function provided are valid, otherwise they will be skipped
 *       directly!
 *
 * @param out object
 * @param ind inference result group index, must be >= 0
 * @param num the number of inference groups that need to be output
 * @param data accurate data buffer to receive
 * @param method unfuzzy method
 * @return true success
 * @return false failed
 */
bool fc_output_unfuzzing(const struct fc_output* const out, fc_index ind, fc_size num, accurate_number* data, fc_output_unfuzzy_method_t method);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_OUTPUT_H__
