#ifndef __FC_CORE_H__
#define __FC_CORE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "fc_config.h"

#define __FC_CORE_EXP fc_exp

#include "fc_error.h"

#include "fc_input.h"
#include "fc_output.h"
#include "fc_rules.h"

#ifndef FLT_EPSILON
#define FLT_EPSILON      1.192092896e-07F        // smallest such that 1.0+FLT_EPSILON != 1.0
#endif // !FLT_EPSILON

#define __FC_CORE_NUMBER_EQ(n1, n2)				(fc_fabs(n1 - n2) < FLT_EPSILON)

#define __FC_CORE_OPERA_UNI(uni1, uni2)			(fc_fmaxf(uni1, uni2))
#define __FC_CORE_OPERA_INT(int1, int2)			(fc_fminf(int1, int2))
#define __FC_CORE_OPERA_COM(com)				(1 - com)

/**
 * @brief Membership function
 *
 * @param value Accurate number
 * @return fuzzy_number Membership degree
 */
#include "fuzzy_opera.h"
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

struct tag_data
{
	const char* tag;
	decimal_number data;
};

bool fc_core_verify_inference_result_is_effective(list_node node, const struct inference_result* const ir);

/**
 * @brief Gaussian curve
 * 
 * @param value pending value
 * @param mu mean value
 * @param sigma standard deviation
 * @param k amplification factor
 * @return fuzzy_number gaussian value
 */
fuzzy_number fc_core_Gaussian(accurate_number value, accurate_number mu, accurate_number sigma, accurate_number k);

/**
 * @brief Triangle curve
 * 
 * @param value pending value
 * @param l left zero point
 * @param m maximum value point
 * @param r right zero point
 * @param k amplification factor
 * @return fuzzy_number triangle value
 */
fuzzy_number fc_core_Triangle(accurate_number value, accurate_number l, accurate_number m, accurate_number r, accurate_number k);

/**
 * @brief Trapezoid curve
 * 
 * @param value pending value
 * @param l left zero point
 * @param r right zero point
 * @param m1 maximum value1 point(left)
 * @param m2 maximum value2 point(right)
 * @param k amplification factor
 * @return fuzzy_number triangle value
 */
fuzzy_number fc_core_Trapezoid(accurate_number value, accurate_number l, accurate_number r, accurate_number m1, accurate_number m2, accurate_number k);

/**
 * @brief Provide a composite output function by combining
 *		  a set of inference results (activation level) and
 *		  membership functions.
 * 
 * @param value value(x)
 * @param irh inference result list
 * @param fsh fuzzy set list
 * @return fuzzy_number result
 */
fuzzy_number fc_core_ir_fs_composite_function(accurate_number value, list_head irh, list_head fsh);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_CORE_H__
