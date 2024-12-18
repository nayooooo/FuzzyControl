#include "fc_core.h"

bool fc_core_verify_inference_result_is_effective(list_node node, const struct inference_result* ir)
{
    if (ir == nullptr) return false;
    if (ir->name_tag == nullptr) return false;

    const struct fuzzy_set* fs = node->data;
    if (fs == nullptr) return false;
    if (fs->label == nullptr) return false;

    return !__FC_OUTPUT_STRCMP(ir->name_tag, fs->label);
}

fuzzy_number fc_core_Gaussian(accurate_number value, accurate_number mu, accurate_number sigma, accurate_number k)
{
    if (sigma < 0) return FUZZY_DATA_ILLEGAL_VALUE;

    return (fuzzy_number)(k * __FC_CORE_EXP(- ((value - mu) * (value - mu)) / (2 * sigma * sigma)));
}

fuzzy_number fc_core_Triangle(accurate_number value, accurate_number l, accurate_number r, accurate_number m, accurate_number k)
{
    if (l > m || l > r) return FUZZY_DATA_ILLEGAL_VALUE;
    if (m > r) return FUZZY_DATA_ILLEGAL_VALUE;

    if (value < m)
    {
        if (value <= l) return 0;
        return (fuzzy_number)(k * ((value - l) / (m - l)));
    }
    else if (value > m)
    {
        if (value >= r) return 0;
        return (fuzzy_number)(k * ((r - value) / (r - m)));
    }
    else return (fuzzy_number)k;
}

fuzzy_number fc_core_Trapezoid(accurate_number value, accurate_number l, accurate_number r, accurate_number m1, accurate_number m2, accurate_number k)
{
    if (l > m1 || l > m2 || l > r) return FUZZY_DATA_ILLEGAL_VALUE;
    if (m1 > m2 || m1 > r) return FUZZY_DATA_ILLEGAL_VALUE;
    if (m2 > r) return FUZZY_DATA_ILLEGAL_VALUE;

    if (value < l) return 0;
    else if (value < m1) return (fuzzy_number)(k * (value - l) / (m1 - l));
    else if (value < m2) return (fuzzy_number)k;
    else if (value < r) return (fuzzy_number)(k * (r - value) / (r - m2));
    else return 0;
}

fuzzy_number fc_core_ir_fs_composite_function(accurate_number value, list_head irh, list_head fsh)
{
    if (irh == nullptr || fsh == nullptr) return 0;

    fuzzy_number ms = 0;
    list_node irn = list_get_first_node(irh);
    while (irn != nullptr)
    {
        const struct inference_result* ir = irn->data;
        if (ir == nullptr) continue;
        list_node fsn = list_find_if(fsh, (void*)ir, (list_pred)fc_core_verify_inference_result_is_effective);
        if (fsn == nullptr) continue;
        const struct fuzzy_set* fs = fsn->data;

        fuzzy_number temp = fs->ms(value);
        if (temp > ir->activate) temp = ir->activate;
        if (ms < temp) ms = temp;

        irn = list_find_next_node(irh, irn);
    }

    return ms;
}
