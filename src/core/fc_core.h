#ifndef __FC_CORE_H__
#define __FC_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "fc_def.h"
#include "fc_error.h"
#include "fc_std.h"

#include "fc_input.h"
#include "fc_output.h"
#include "fc_rules.h"
#include "fc_controler.h"

/**
 * @brief Membership function
 *
 * @param value Accurate number
 * @return fuzzy_number Membership degree
 */
#include "./../exter/fuzzy_opera.h"
typedef fuzzy_number (*fc_membership_fn)(accurate_number value);

/**
 * @brief Fuzzy set
 * 
 * @memberof label the set's name
 * @memberof ms membership function
 */
struct fuzzy_set
{
	const char* label;
	fc_membership_fn ms;
};

/**
 * @brief Inference results
 *
 * @memberof name_tag name tag pairs
 * @memberof activate activation level
 */
struct inference_result
{
	const char* name_tag;
	fuzzy_number activate;
};

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_CORE_H__
