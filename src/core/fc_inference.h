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
    struct fc_interface interface;

    bool (*register_dev)(struct fc_inference* const infer, const char* name);
    bool (*unregister_dev)(struct fc_inference* const infer);
    bool (*add_fuzzy_input_data)(
        const struct fc_inference* const infer, const struct fuzzy_matrix data,
        const char* name, list_head label
    );
    bool (*clear_fuzzy_input_data)(const struct fc_inference* const infer);
    bool (*inference)(const struct fc_inference* const infer);
    bool (*print)(const struct fc_inference* const infer);
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
bool fc_inference_register(struct fc_inference* const infer, const char* name);

/**
 * @brief Unregister the inference device of the fuzzy controller
 * 
 * @param infer object
 * @return true success
 * @return false failed
 */
bool fc_inference_unregister(struct fc_inference* const infer);

/**
 * @brief Add the input fuzzy matrix to the inference component
 * @note Deep copy incoming fuzzy matrix, name and label, store the structure in the form of fuzzy
 *       input data description, and then provide it to the linked list
 *       Note that the name belongs to the literal quantity, so take the literal quantity address directly
 * 
 * @param infer object
 * @param data fuzzy input matrix
 * @param name fuzzy input matrix name
 * @param label The label of each element of the fuzzy input matrix (i.e. the name of the fuzzy subset)
 * @return true success
 * @return false failed
 */
bool fc_inference_add_fuzzy_input_data(
    const struct fc_inference* const infer, const struct fuzzy_matrix* data,
    const char* name, list_head label
);

/**
 * @brief Clear fuzzy input data
 * 
 * @param infer object
 * @return true success
 * @return false failed
 */
bool fc_inference_clear_fuzzy_input_data(const struct fc_inference* const infer);

/**
 * @brief Perform fuzzy inference
 * 
 * @param infer object
 * @return true success
 * @return false failed
 */
bool fc_inference_inference(const struct fc_inference* const infer);

/**
 * @brief Print out the fuzzy relationships
 * 
 * @param infer object
 * @return true success
 * @return false failed
 */
bool fc_inference_print(const struct fc_inference* const infer);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_INFERENCE_H__
