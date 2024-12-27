#ifndef __FC_CONTROLLER_H__
#define __FC_CONTROLLER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "list.h"

#include "fc_core.h"
struct fc_input;
struct fc_output;
struct fc_rules;

/*==================================================================================
    micro function
==================================================================================*/

#define __FC_CONTROLLER_PRINTF fc_printf
#define __FC_CONTROLLER_MALLOC fc_malloc
#define __FC_CONTROLLER_FREE fc_free
#define __FC_CONTROLLER_STRCMP fc_strcmp
#define __FC_CONTROLLER_STRTOK_S fc_strtok_s

/*==================================================================================
    structure
==================================================================================*/

struct fc_controller
{
    list_head in;
    list_head out;
    struct fc_rules *rules;
};

/*==================================================================================
    API
==================================================================================*/

/**
 * @brief Register controller
 *
 * @param fcc object
 * @return true success
 * @return false failed
 */
bool fc_controller_register(struct fc_controller* const fcc);

/**
 * @brief Unregister controller
 *
 * @param fcc object
 * @return true success
 * @return false failed
 */
bool fc_controller_unregister(struct fc_controller* const fcc);

/**
 * @brief Increase a input
 *
 * @param fcc object
 * @param name input name
 * @return true success
 * @return false failed
 */
bool fc_controller_increase_a_input(const struct fc_controller* const fcc, const char* name);

/**
 * @brief Decrease a input
 *
 * @param fcc object
 * @param name input name, if nullptr, pop
 * @return true success
 * @return false failed
 */
bool fc_controller_decrease_a_input(const struct fc_controller* const fcc, const char* name);

/**
 * @brief Delete all input
 *
 * @param fcc object
 * @return true success
 * @return false failed
 */
bool fc_controller_delete_all_input(const struct fc_controller* const fcc);

/**
 * @brief print all input(name)
 *
 * @param fcc object
 * @param label label
 * @return true success
 * @return false failed
 */
bool fc_controller_print_input_list(const struct fc_controller* const fcc, const char *label);

/**
 * @brief Increase a output
 *
 * @param fcc object
 * @param name output name
 * @param min minimum output value
 * @param max maximum output value
 * @param step step value
 * @return true success
 * @return false failed
 */
bool fc_controller_increase_a_output(const struct fc_controller* const fcc, const char* name, accurate_number min, accurate_number max, accurate_number step);

/**
 * @brief Decrease a output
 *
 * @param fcc object
 * @param name output name, if nullptr, pop
 * @return true success
 * @return false failed
 */
bool fc_controller_decrease_a_output(const struct fc_controller* const fcc, const char* name);

/**
 * @brief Delete all output
 *
 * @param fcc object
 * @return true success
 * @return false failed
 */
bool fc_controller_delete_all_output(const struct fc_controller* const fcc);

/**
 * @brief print all output(name)
 *
 * @param fcc object
 * @param label label
 * @return true success
 * @return false failed
 */
bool fc_controller_print_output_list(const struct fc_controller* const fcc, const char* label);

/**
 * @brief Add a fuzzy set to the input
 *
 * @param fcc object
 * @param name input name
 * @param tag fuzzy set tag
 * @param ms fuzzy set membership
 * @return true success
 * @return false failed
 */
bool fc_controller_add_fuzzy_set_for_input(const struct fc_controller* const fcc, const char* name, const char* tag, fc_membership_fn ms);

/**
 * @brief Clear the fuzzy set in the input
 *
 * @param fcc object
 * @param name input name
 * @return true success
 * @return false failed
 */
bool fc_controller_clear_fuzzy_set_for_input(const struct fc_controller* const fcc, const char* name);

/**
 * @brief Print the fuzzy set in the input
 *
 * @param fcc object
 * @param name input name
 * @param label label
 * @return true success
 * @return false failed
 */
bool fc_controller_print_input_fuzzy_set(const struct fc_controller* const fcc, const char* name, const char* label);

/**
 * @brief Add a fuzzy set to the output
 *
 * @param fcc object
 * @param name output name
 * @param tag fuzzy set tag
 * @param ms fuzzy set membership
 * @return true success
 * @return false failed
 */
bool fc_controller_add_fuzzy_set_for_output(const struct fc_controller* const fcc, const char* name, const char* tag, fc_membership_fn ms);

/**
 * @brief Clear the fuzzy set in the output
 *
 * @param fcc object
 * @param name output name
 * @return true success
 * @return false failed
 */
bool fc_controller_clear_fuzzy_set_for_output(const struct fc_controller* const fcc, const char* name);

/**
 * @brief Print the fuzzy set in the output
 *
 * @param fcc object
 * @param name output name
 * @return true success
 * @return false failed
 */
bool fc_controller_print_output_fuzzy_set(const struct fc_controller* const fcc, const char* name, const char* label);

/**
 * @brief Add a rule to the rule controller
 *
 * @param fcc object
 * @param rule rule
 * @return true success
 * @retrun false failed
 */
bool fc_controller_add_rule(const struct fc_controller* const fcc, fc_rule_item rule);

/**
 * @brief Clear rules in the rule controller
 *
 * @param fcc object
 * @return true success
 * @retrun false failed
 */
bool fc_controller_clear_rule(const struct fc_controller* const fcc);

/**
 * @brief Print rules in the rule controller
 *
 * @param fcc object
 * @param label label
 * @return true success
 * @retrun false failed
 */
bool fc_controller_print_rule(const struct fc_controller* const fcc, const char* label);

/**
 * @brief Perform a deduction once
 *
 * @param fcc object
 * @param in input value
 * @param in_num input value number
 * @param out output value
 * @param out_num output value number
 * @return true success
 * @retrun false failed
 */
bool fc_controller_reasoning(const struct fc_controller* const fcc, const struct tag_data* in, fc_size in_num, struct tag_data *out, fc_size out_num);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_CONTROLLER_H__
