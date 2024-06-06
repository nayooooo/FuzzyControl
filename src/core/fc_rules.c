#include "fc_rules.h"
#include "fc_def.h"

static const fc_rule_keyword __fc_rules_keyword_table[] = {
    "IF", "THEN",            // conditional segmentation keywords
    "AND", "OR",             // conditional relationship keywords
    "?",                     // conditional ignore keywords
    "|",                     // multiple output annotations for results
};

static fc_size __count_sub_string_num(const char* str, const char* substr)
{
    fc_size num = 0;

    while (*str != '\0')
    {
        const char* mark = fc_strstr(str, substr);
        if (mark == nullptr) break;
        num++;
        str = mark + 1;
    }

    return num;
}

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

bool is_fc_rules_legal_rule(const fc_rule_item rule, const fc_rule_keyword* const keyword_table, const fc_size num)
{
    if (!__IS_FC_RULES_RULE_ITEM_EXIST(rule) || keyword_table == nullptr || num <= FC_RULES_KEYWORD_TABLE_MAX_INDEX)
        return false;

    // Only one occurrence of "IF" is allowed
    if (__count_sub_string_num(rule, keyword_table[KEYWORD_IF_INDEX]) != 1) return false;
    // Only one occurrence of "THEN" is allowed
    if (__count_sub_string_num(rule, keyword_table[KEYWORD_THEN_INDEX]) != 1) return false;

    if (fc_strstr(rule, keyword_table[KEYWORD_IF_INDEX]) != rule) return false;

    // Create Rule Copy
    fc_rule_item rule_copy = (fc_rule_item)fc_malloc(fc_strlen(rule) + 1);
    if (!__IS_FC_RULES_RULE_ITEM_EXIST(rule_copy)) return false;
    fc_strcpy_s((char*)rule_copy, fc_strlen(rule) + 1, rule);

    struct
    {
        uint8_t exist_IF : 1;
        uint8_t exist_THEN : 1;
        uint8_t exist_condition : 1;
        uint8_t exist_result : 1;
        uint8_t is_rule_ok : 1;
    }flag = { 0, 0, 0, 0, 1, };
    const char* word = nullptr;
    char* context = nullptr;
    fc_index ind = KEYWORD_NOT_FOUND_INDEX;
    fc_index count = 0;

    // check
    word = fc_strtok_s((char*)rule_copy, " ", &context);
    while (__IS_FC_RULES_RULE_ITEM_EXIST(word))
    {
        ind = is_fc_fules_keyword(word, keyword_table, num);

        // keyword
        if ((!(count % 2)) && (ind == KEYWORD_NOT_FOUND_INDEX || ind == KEYWORD_QM_INDEX)) { flag.is_rule_ok = 0; break; }
        // user
        if ((count % 2) && (ind != KEYWORD_NOT_FOUND_INDEX && (flag.exist_THEN || ind != KEYWORD_QM_INDEX))) { flag.is_rule_ok = 0; break; }

        // "IF"
        if (!flag.exist_IF)
        {
            if (count == 0 && ind == KEYWORD_IF_INDEX) flag.exist_IF = 1;
            else { flag.is_rule_ok = 0; break; }
        }

        // "THEN"
        if (!flag.exist_THEN) if ((!(count % 2)) && ind == KEYWORD_THEN_INDEX) flag.exist_THEN = 1;

        // "condition"
        if (!flag.exist_condition)
        {
            if (count == 1 && (ind == KEYWORD_NOT_FOUND_INDEX || ind == KEYWORD_QM_INDEX))
                flag.exist_condition = 1;
            else if (count == 1) { flag.is_rule_ok = 0; break; }
        }

        // "result"
        if ((count % 2) && flag.exist_THEN && ind == KEYWORD_NOT_FOUND_INDEX) flag.exist_result = 1;

        // check user
        if (count % 2)
        {
            const char* word_context = fc_strchr(word, '-');
            if (word_context == nullptr || *(word_context + 1) == '\0')
            {
                flag.is_rule_ok = 0;
                break;
            }
        }

        word = fc_strtok_s(nullptr, " ", &context);
        count++;
    }

    fc_free((void*)rule_copy);
    rule_copy = nullptr;

    if (flag.exist_IF && flag.exist_THEN && flag.exist_condition && flag.exist_result && flag.is_rule_ok)
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

bool fc_rules_add_rule(const struct fc_rules* const obj, fc_rule_item rule)
{
    if (obj == nullptr || !__IS_FC_RULES_RULE_ITEM_EXIST(rule)) return false;

    if (!is_fc_rules_legal_rule(rule, obj->rule_keyword_table, obj->rule_keyword_num)) return false;

    if (!list_push(obj->rules, rule, fc_strlen(rule) + 1)) return false;

    return true;
}

bool fc_rules_clear_rule(const struct fc_rules* const obj)
{
    if (obj == nullptr) return false;

    if (!list_clear(obj->rules, nullptr)) return false;

    return true;
}

static bool __fc_rules_print_rule_cb(list_node node, void* data)
{
    UNUSED(data);

    __FC_RULES_PRINTF("\"%s\"\r\n", node->data);

    return true;
}

bool fc_rules_print_rule(const struct fc_rules* const obj, const char* label)
{
    if (obj == nullptr) return false;

    __FC_RULES_PRINTF("%s: \r\n", label ? label : "(unset label)");
    if (!list_trav(obj->rules, nullptr, __fc_rules_print_rule_cb)) return false;

    return true;
}

bool fc_rules_export_calculation(const struct fc_rules* const obj, struct fc_calculation* const cal)
{
    if (obj == nullptr || cal == nullptr) return false;

    if (list_length(obj->rules) <= 0) return false;

    return false;
}
