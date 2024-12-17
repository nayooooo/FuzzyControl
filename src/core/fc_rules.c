#include "fc_rules.h"

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
        const char* mark = __FC_RULES_STRSTR(str, substr);
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

    for (; (fc_size)index < num; index++)
    {
        if (__FC_RULES_STRCMP(word, keyword_table[index]) == 0)
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

    if (__FC_RULES_STRSTR(rule, keyword_table[KEYWORD_IF_INDEX]) != rule) return false;

    // Create Rule Copy
    fc_rule_item rule_copy = (fc_rule_item)__FC_RULES_MALLOC((__FC_RULES_STRLEN(rule) + 1) * sizeof(char));
    if (!__IS_FC_RULES_RULE_ITEM_EXIST(rule_copy)) return false;
    __FC_RULES_STRCPY_S((char*)rule_copy, (__FC_RULES_STRLEN(rule) + 1) * sizeof(char), rule);

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
    word = __FC_RULES_STRTOK_S((char*)rule_copy, " ", &context);
    while (word != nullptr)
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

        word = __FC_RULES_STRTOK_S(nullptr, " ", &context);
        count++;
    }

    __FC_RULES_FREE((void*)rule_copy);
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
    obj->rules = nullptr;

    return true;
}

bool fc_rules_add_rule(const struct fc_rules* const obj, fc_rule_item rule)
{
    if (obj == nullptr || !__IS_FC_RULES_RULE_ITEM_EXIST(rule)) return false;

    if (!is_fc_rules_legal_rule(rule, obj->rule_keyword_table, obj->rule_keyword_num)) return false;

    if (!list_push(obj->rules, (void*)rule, __FC_RULES_STRLEN(rule) + 1)) return false;

    return true;
}

bool fc_rules_clear_rule(const struct fc_rules* const obj)
{
    if (obj == nullptr) return false;

    if (!list_clear(obj->rules, nullptr)) return false;

    return true;
}

fc_size fc_rules_get_rule_num(const struct fc_rules* const obj)
{
    if (obj == nullptr) return 0;

    return list_length(obj->rules);
}

static bool __fc_rules_print_rule_cb(list_node node, void* data)
{
    UNUSED(data);

    __FC_RULES_PRINTF("\"%s\"\r\n", (char*)(node->data));

    return true;
}

bool fc_rules_print_rule(const struct fc_rules* const obj, const char* label)
{
    if (obj == nullptr) return false;

    __FC_RULES_PRINTF("%s: \r\n", label ? label : "(unset label)");
    if (!list_trav(obj->rules, nullptr, __fc_rules_print_rule_cb)) return false;

    return true;
}

static bool __fc_rules_calculation_construct_cb(void* data)
{
    if (data == nullptr) return false;

    struct __fc_calculation_unit* unit = data;
    if (unit->cr == nullptr) return false;

    fc_rule_consition_result cr = (fc_rule_consition_result)__FC_RULES_MALLOC((__FC_RULES_STRLEN(unit->cr) + 1) * sizeof(char));
    if (cr == nullptr) return false;
    __FC_RULES_STRCPY_S((char*)cr, (__FC_RULES_STRLEN(unit->cr) + 1) * sizeof(char), unit->cr);

    unit->cr = cr;

    return true;
}

static bool __fc_rules_calculation_deconstruct_cb(void* data)
{
    if (data == nullptr) return false;

    struct __fc_calculation_unit* unit = data;

    if (unit->cr == nullptr) return false;
    __FC_RULES_FREE((void*)(unit->cr));
    unit->cr = nullptr;

    return true;
}

bool fc_rules_create_calculation(struct fc_calculation* const cal)
{
    if (cal == nullptr) return false;

    cal->condition = list_create();
    if (cal->condition == nullptr) goto _error_out;
    cal->result = list_create();
    if (cal->result == nullptr) goto _error_out;

    return true;

_error_out:
    list_delete(cal->condition, nullptr);
    list_delete(cal->result, nullptr);
    cal->condition = nullptr;
    cal->result = nullptr;
    return false;
}

bool fc_rules_export_calculation(struct fc_calculation* const cal, const struct fc_rules* const rules, const fc_index ind)
{
    if (rules == nullptr || cal == nullptr) return false;

    if (!fc_rules_clear_calculation(cal)) return false;

    if (ind < 0) return false;
    if (list_length(rules->rules) <= (fc_size)ind) return false;

    // get rule
    fc_rule_item rule = list_get_node_data(rules->rules, ind);
    if (rule == nullptr) return false;

    // create rule copy
    fc_rule_item rule_copy = (fc_rule_item)__FC_RULES_MALLOC((__FC_RULES_STRLEN(rule) + 1) * sizeof(char));
    if (rule_copy == nullptr) return false;
    __FC_RULES_STRCPY_S((char*)rule_copy, (__FC_RULES_STRLEN(rule) + 1) * sizeof(char), rule);

    bool is_result = false;
    const char* word = nullptr;
    char* context = nullptr;
    fc_index count = 0;
    word = __FC_RULES_STRTOK_S((char*)rule_copy, " ", &context);

    // set consition and result
    struct __fc_calculation_unit unit = { "", OPERA_OR };  // The first input should be taken as larger
    while (word != nullptr)
    {
        fc_index word_ind = is_fc_fules_keyword(word, rules->rule_keyword_table, rules->rule_keyword_num);

        // find THEN
        if (!(count % 2) && word_ind == KEYWORD_THEN_INDEX) is_result = true;

        // set opera
        if (!(count % 2))
        {
            switch (word_ind)
            {
            case KEYWORD_AND_INDEX:
                unit.opera = OPERA_AND;
                break;
            case KEYWORD_OR_INDEX:
                unit.opera = OPERA_OR;
                break;
            case KEYWORD_VB_INDEX:
                unit.opera = OPERA_VB;
                break;
            }
        }

        // set consition and result
        if (count % 2)
        {
            unit.cr = word;
            if (!is_result)  // consition
            {
                list_push_if(cal->condition, &unit, sizeof(struct __fc_calculation_unit), list_pred_true, __fc_rules_calculation_construct_cb);
            }
            else  // result
            {
                list_push_if(cal->result, &unit, sizeof(struct __fc_calculation_unit), list_pred_true, __fc_rules_calculation_construct_cb);
            }
        }

        word = __FC_RULES_STRTOK_S(nullptr, " ", &context);
        count++;
    }

    __FC_RULES_FREE((void*)rule_copy);
    rule_copy = nullptr;

    return false;
}

bool fc_rules_delete_calculation(struct fc_calculation* const cal)
{
    if (cal == nullptr) return false;

    if (cal->condition != nullptr)
    {
        list_delete(cal->condition, __fc_rules_calculation_deconstruct_cb);
        cal->condition = nullptr;
    }
    if (cal->result != nullptr)
    {
        list_delete(cal->result, __fc_rules_calculation_deconstruct_cb);
        cal->result = nullptr;
    }

    return true;
}

bool fc_rules_clear_calculation(struct fc_calculation* const cal)
{
    if (cal == nullptr) return false;
    if (cal->condition == nullptr || cal->result == nullptr) return false;

    if (!list_clear(cal->condition, __fc_rules_calculation_deconstruct_cb)) return false;
    if (!list_clear(cal->result, __fc_rules_calculation_deconstruct_cb)) return false;

    return true;
}

bool fc_rules_print_calculation(const struct fc_calculation* const cal, const char* label)
{
    if (cal == nullptr) return false;
    if (cal->condition == nullptr || cal->result == nullptr) return false;

    __FC_RULES_PRINTF("%s: \r\n", label ? label : "(unset label)");
    for (fc_index i = 0; (fc_size)i < list_length(cal->condition); i++)
    {
        struct __fc_calculation_unit* unit = list_get_node_data(cal->condition, i);
        if (unit == nullptr) return false;
        if (i != 0)
        {
            switch (unit->opera)
            {
            case OPERA_AND:
                __FC_RULES_PRINTF(" /\\ ");
                break;
            case OPERA_OR:
                __FC_RULES_PRINTF(" \\/ ");
                break;
            }
        }
        __FC_RULES_PRINTF("\"%s\"", unit->cr);
    }
    __FC_RULES_PRINTF(" => ");
    for (fc_index i = 0; (fc_size)i < list_length(cal->result); i++)
    {
        struct __fc_calculation_unit* unit = list_get_node_data(cal->result, i);
        if (unit == nullptr) return false;
        if (i != 0) __FC_RULES_PRINTF(" | ");
        __FC_RULES_PRINTF("\"%s\"", unit->cr);
    }

    __FC_RULES_PRINTF("\r\n");

    return true;
}
