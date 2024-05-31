#ifndef __FC_INFERENCE_H__
#define __FC_INFERENCE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "fc_interface.h"

/*==================================================================================
    structure
==================================================================================*/

/**
 * @brief Fuzzy controller inference components
 * 
 * @memberof Obj object
 * @memberof register_dev register
 * @memberof unregister_dev unregister
 * @memberof inference fuzzy inference
 * @memberof print print fuzzy relationships
 */
struct fc_inference
{
    struct fc_interface obj;

    bool (*register_dev)(struct fc_inference* infer, const char* name);
    bool (*unregister_dev)(struct fc_inference* infer);
    bool (*inference)(struct fc_inference* infer);
    bool (*print)(struct fc_inference* infer);
};

/*==================================================================================
    API
==================================================================================*/

/**
 * @brief Register as inference object for fuzzy controller
 * @note Never execute this function on inference devices of fuzzy controllers
 *       that have already been registered. If you need to execute this
 *       function, please unregister it first
 * 
 * @param infer object
 * @param name the label
 * @return true success
 * @return false failed
 */
bool fc_inference_register(struct fc_inference* infer, const char* name);

/**
 * @brief Unregister the inference device of the fuzzy controller
 * 
 * @param infer object
 * @return true success
 * @return false failed
 */
bool fc_inference_unregister(struct fc_inference* infer);

/**
 * @brief Perform fuzzy reasoning
 * 
 * @param infer object
 * @return true success
 * @return false failed
 */
bool fc_inference_inference(struct fc_inference* infer);

/**
 * @brief Print out the fuzzy relationships
 * 
 * @param infer object
 * @return true success
 * @return false failed
 */
bool fc_inference_print(struct fc_inference* infer);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_INFERENCE_H__
