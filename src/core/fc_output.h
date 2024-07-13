#ifndef __FC_OUTPUT_H__
#define __FC_OUTPUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "./../exter/fuzzy_opera.h"
#include "./../exter/list.h"

#include "fc_core.h"
struct fuzzy_set;
struct inference_result;

/*==================================================================================
    structure
==================================================================================*/

struct fc_output
{
    const char* name;
    list_head data;             // to save inference result groups
    list_head fuzzy_set;
};

/*==================================================================================
    API
==================================================================================*/

/**
 * @brief Register output
 * @note Do not use this function on fc_output that have already been registered
 * 
 * @param out object
 * @param name name
 * @return true success
 * @return false failed
 */
bool fc_output_register(struct fc_output* const out, const char* name);

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
 * @brief How many pieces of data can be obfuscated to obtain
 *
 * @param out object
 * @return fc_size
 */
fc_size fc_output_get_unfuzzing_data_num(const struct fc_output* const out);

/**
 * @brief Perform one anti-fuzzification
 *
 * @param out object
 * @param data accurate data buffer to receive
 * @param num accurate data buffer size, must be equal to or
 *            larger than the actual number of conversions
 * @return true success
 * @return false failed
 */
bool fc_output_unfuzzing(struct fc_output* const out, accurate_number* data, fc_size num);

/**
 * @brief Print blurred data
 *
 * @param out object
 * @param label label
 * @return true success
 * @return false failed
 */
bool fc_output_print_data(struct fc_output* const out, const char* label);

/**
 * @brief Print Fuzzy Sets
 *
 * @param out object
 * @param label label
 * @return true success
 * @return false failed
 */
bool fc_output_print_fuzzy_set(struct fc_output* const out, const char* label);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_OUTPUT_H__
