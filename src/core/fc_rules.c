#include "fc_rules.h"
#include "fc_def.h"

static const fc_rule_keyword __fc_rules_keyword_table[] = {
    "IF", "THEN",            // conditional segmentation keywords
    "AND", "OR",             // conditional relationship keywords
    "?",                     // conditional ignore keywords
    "|",                     // multiple output annotations for results
};

fc_index is_fc_fules_keyword(const fc_rule_keyword word, const fc_rule_keyword* const keyword_table, const fc_size num)
{
    if (__IS_FC_RULES_RULE_KEYWORD_EMPTY(word) || keyword_table == nullptr || num <= 0) return -1;

    fc_index index = 0;

    for (; index < num; index++)
    {
        if (fc_strcmp(word, keyword_table[index]) == 0)
            return index;
    }

    return KEYWORD_NOT_FOUND_INDEX;
}

// TODO(me) Need to be more concise
bool is_fc_rules_legal_rule(const fc_rule_item rule, const fc_rule_keyword* const keyword_table, const fc_size num)
{
    if (!__IS_FC_RULES_RULE_ITEM_EXIST(rule) || keyword_table == nullptr || num <= FC_RULES_KEYWORD_TABLE_MAX_INDEX)
        return false;

    fc_rule_item rule_copy = (fc_rule_item)fc_malloc(fc_strlen(rule) + 1);
    if (!__IS_FC_RULES_RULE_ITEM_EXIST(rule_copy)) return false;
    fc_strcpy_s((char*)rule_copy, fc_strlen(rule) + 1, rule);

    struct
    {
        uint8_t exist_IF : 1;
        uint8_t exist_THEN : 1;
        uint8_t exist_condition : 1;
        uint8_t exist_result : 1;
    }flag = { 0, 0, 0, 0 };
    fc_index count = 0;  // the index of word
    const char* temp = nullptr;
    char* context = nullptr;

    // Check if the first word is IF
    temp = fc_strtok_s((char*)rule_copy, " ", &context);
    if (fc_strcmp(temp, keyword_table[KEYWORD_IF_INDEX]) == 0)
        flag.exist_IF = 1;
    else goto out;

    // Check the words between IF and THEN (including THEN)
    temp = fc_strtok_s(nullptr, " ", &context);
    count++;
    fc_index kw_ind;
    while (temp != nullptr)
    {
        // Ensure that the words at the corresponding positions are legal
        kw_ind = is_fc_fules_keyword(temp, keyword_table, num);
        if (!(count % 2) && kw_ind == KEYWORD_NOT_FOUND_INDEX) goto out;  // Must be a keyword, but not
        if ((count % 2) && (kw_ind != KEYWORD_NOT_FOUND_INDEX && kw_ind != KEYWORD_QM_INDEX)) goto out;  // Must be a condition or "?", but not

        // Determine whether the format of the condition is legal
        if (count % 2)
        {
            const char* word_context = fc_strchr(temp, '-');
            if (word_context == nullptr) goto out;
            if (*(word_context + 1) == '\0') goto out;
        }

        if (count == 1) flag.exist_condition = 1;

        // find THEN
        if (!(count % 2) && kw_ind == KEYWORD_THEN_INDEX)
        {
            flag.exist_THEN = 1;
            break;
        }

        temp = fc_strtok_s(nullptr, " ", &context);
        count++;
    }

    // Check the words after THEN (cannot be keywords, including "?")
    temp = fc_strtok_s(nullptr, " ", &context);
    count++;
    if (temp != nullptr) flag.exist_result = 1;
    while (temp != nullptr)
    {
        // Ensure that the words at the corresponding positions are legal
        kw_ind = is_fc_fules_keyword(temp, keyword_table, num);
        if (!(count % 2) && kw_ind != KEYWORD_VB_INDEX) goto out;  // Must be "|", but not
        if ((count % 2) && kw_ind != KEYWORD_NOT_FOUND_INDEX) goto out;  // Must be a condition, but not

        if (count % 2)
        {
            const char* word_context = fc_strchr(temp, '-');
            if (word_context == nullptr || *(word_context + 1) == '\0')
            {
                fc_free((void*)rule_copy);
                rule_copy = nullptr;
                return false;
            }
        }

        temp = fc_strtok_s(nullptr, " ", &context);
        count++;
    }

out:

    fc_free((void*)rule_copy);
    rule_copy = nullptr;

    if (flag.exist_IF && flag.exist_THEN && flag.exist_condition && flag.exist_result)
        if (!(count % 2))
            return true;
    return false;
}

bool fc_rules_register(struct fc_rules* const obj)
{
    if (obj == nullptr) return false;

    obj->rule_keyword_table = __fc_rules_keyword_table;
    obj->rule_keyword_num = sizeof(__fc_rules_keyword_table) / sizeof(__fc_rules_keyword_table[0]);

    obj->rules = list_create();
    if (obj->rules == nullptr) return false;

    return true;
}

bool fc_rules_unregister(struct fc_rules* const obj)
{
    if (obj == nullptr) return false;

    obj->rule_keyword_table = nullptr;
    obj->rule_keyword_num = 0;

    list_delete(obj->rules, nullptr);

    return true;
}
