#ifndef __FC_RULES_H__
#define __FC_RULES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "./../exter/list.h"

#include "fc_std.h"

/*==================================================================================
    type
==================================================================================*/

// rule keyword
typedef const char* fc_rule_keyword;

// rule item
typedef const char* fc_rule_item;

/*==================================================================================
    micro function
==================================================================================*/

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
#define FC_RULES_KEYWORD_TABLE_MAX_INDEX KEYWORD_QM_INDEX

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
    fc_rule_keyword* rule_keyword_table;
    fc_size rule_keyword_num;

    list_head rules;
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
bool fc_fules_add_rule(const struct fc_rules* const obj, fc_rule_item rule);

/**
 * @brief Clear rules in the rule controller
 *
 * @param obj object
 * @return true success
 * @retrun false failed
 */
bool fc_fules_clear_rule(const struct fc_rules* const obj);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_RULES_H__
