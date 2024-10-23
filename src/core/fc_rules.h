#ifndef __FC_RULES_H__
#define __FC_RULES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "./../exter/list.h"

#include "fc_core.h"

/*==================================================================================
    type
==================================================================================*/

// rule keyword
typedef const char* fc_rule_keyword;

// rule item
typedef const char* fc_rule_item;

// rule consition
typedef const char* fc_rule_consition;

// rule result
typedef const char* fc_rule_result;

// rule consition/result
typedef const char* fc_rule_consition_result;

/*==================================================================================
    micro function
==================================================================================*/

#define __FC_RULES_PRINTF fc_printf
#define __FC_RULES_MALLOC fc_malloc
#define __FC_RULES_FREE fc_free
#define __FC_RULES_STRLEN fc_strlen
#define __FC_RULES_STRCPY_S fc_strcpy_s
#define __FC_RULES_STRCMP fc_strcmp
#define __FC_RULES_STRTOK_S fc_strtok_s
#define __FC_RULES_STRCHR fc_strchr
#define __FC_RULES_STRSTR fc_strstr

#define __IS_FC_RULES_RULE_ITEM_EXIST(RULE) (RULE != nullptr)

#define __IS_FC_RULES_RULE_KEYWORD_EMPTY(KEYWORD) (KEYWORD == nullptr)
#define __IS_FC_RULES_RULE_ITEM_EMPTY(RULE) (fc_strlen(RULE) <= 0)

/*==================================================================================
    enum
==================================================================================*/

// Index enumeration of each keyword in the keyword table
typedef enum
{
    KEYWORD_NOT_FOUND_INDEX = -1,
    KEYWORD_IF_INDEX = 0,       // IF
    KEYWORD_THEN_INDEX,         // THEN
    KEYWORD_AND_INDEX,          // AND
    KEYWORD_OR_INDEX,           // OR
    KEYWORD_QM_INDEX,           // ?
    KEYWORD_VB_INDEX,           // |
}fc_rules_keyword_table_index;
#define FC_RULES_KEYWORD_TABLE_MAX_INDEX KEYWORD_VB_INDEX

// Operation type
typedef enum
{
    OPERA_AND = 0,
    OPERA_OR,
    OPERA_VB,
}fc_rules_opera_type;

/*==================================================================================
    structure
==================================================================================*/

/**
 * @brief The rule controller
 * 
 * @memberof rule_keyword_table
 * @memberof rule_keyword_num
 * @memberof rules rule library
 */
struct fc_rules
{
    const fc_rule_keyword* rule_keyword_table;
    fc_size rule_keyword_num;

    list_head rules;
};

struct __fc_calculation_unit
{
    fc_rule_consition_result cr;
    fc_rules_opera_type opera;
};

struct fc_calculation
{
    list_head condition;
    list_head result;
};

/*==================================================================================
    Auxiliary API
==================================================================================*/

/**
 * @brief Check if the word is a keyword, and if so, return the index in the given
 *        keyword table
 * 
 * @param word Word to be confirmed
 * @param keyword_table The keyword table
 * @param num The number of keywords in the keyword table
 * @return fc_index 
 *              -1      ->      not found
 *              >=0     ->      index
 */
fc_index is_fc_fules_keyword(const fc_rule_keyword word, const fc_rule_keyword* const keyword_table, const fc_size num);

/**
 * @brief Check if it is a legal rule
 * @note Legal Rule Style: 
 *       "IF A-1 AND B-2 OR C-4 THEN U-2"
 *       "IF a-1 AND b-2 OR c-4 THEN u-2"
 *       "IF A-1 AND ? AND C-4 THEN U-2"
 *       "IF A-1 AND ? OR C-4 THEN U-2"
 *       "IF A-1 AND ? OR C-4 THEN U-2 | V-3"
 *       "IF A-1 AND ? OR C-4 THEN U-2 | U-3"
 *       "IF A-1-2-3-1 AND ? OR C-4 THEN U-2-1-4-6 | V-3"
 *       ......
 * 
 * @param rule Rules to be checked
 * @param keyword_table The keyword table
 * @param num The number of keywords in the keyword table
 * @return true yes
 * @return false no or param error
 */
bool is_fc_rules_legal_rule(const fc_rule_item rule, const fc_rule_keyword* const keyword_table, const fc_size num);

/*==================================================================================
    API
==================================================================================*/

/**
 * @brief Register rule controller
 * @note Do not use this function on fc_rules that have already been registered
 * 
 * @param obj object
 * @return true success
 * @retrun false failed
 */
bool fc_rules_register(struct fc_rules* const obj);

/**
 * @brief Unregister rule controller
 * 
 * @param obj object
 * @return true success
 * @retrun false failed
 */
bool fc_rules_unregister(struct fc_rules* const obj);

/**
 * @brief Add a rule to the rule controller
 * 
 * @param obj object
 * @param rule rule
 * @return true success
 * @retrun false failed
 */
bool fc_rules_add_rule(const struct fc_rules* const obj, fc_rule_item rule);

/**
 * @brief Clear rules in the rule controller
 *
 * @param obj object
 * @return true success
 * @retrun false failed
 */
bool fc_rules_clear_rule(const struct fc_rules* const obj);

/**
 * @brief Get rules number
 * 
 * @param obj obect
 * @return fc_size the rules number
 */
fc_size fc_rules_get_rule_num(const struct fc_rules* const obj);

/**
 * @brief Print rules in the rule controller
 *
 * @param obj object
 * @param label label
 * @return true success
 * @retrun false failed
 */
bool fc_rules_print_rule(const struct fc_rules* const obj, const char* label);

/**
 * @brief Create calculation
 * @note Do not use this function on calculations that have already been created
 * 
 * @param cal calculation structure
 * @return true success
 * @retrun false failed
 */
bool fc_rules_create_calculation(struct fc_calculation* const cal);

/**
 * @brief Export calculation structure
 * 
 * @param cal calculation structure
 * @param obj rules
 * @param ind the rule index(0, 1, 2, ...)
 * @return true success
 * @retrun false failed
 */
bool fc_rules_export_calculation(struct fc_calculation* const cal, const struct fc_rules* const rules, const fc_index ind);

/**
 * @brief Delete calculation structure
 *
 * @param cal calculation structure
 * @return true success
 * @retrun false failed
 */
bool fc_rules_delete_calculation(struct fc_calculation* const cal);

/**
 * @brief Clear calculation structure
 *
 * @param cal calculation structure
 * @return true success
 * @retrun false failed
 */
bool fc_rules_clear_calculation(struct fc_calculation* const cal);

/**
 * @brief Print calculation structure
 *
 * @param cal calculation structure
 * @param label label
 * @return true success
 * @retrun false failed
 */
bool fc_rules_print_calculation(const struct fc_calculation* const cal, const char* label);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_RULES_H__
