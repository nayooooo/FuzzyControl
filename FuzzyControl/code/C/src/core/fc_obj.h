#ifndef __FC_OBJ_H__
#define __FC_OBJ_H__

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================
    Pre type
==================================================================================*/

/**
 * @brief Fuzzy controller object type
 * @details Used to indicate the type of object, facilitating automatic
 *          invocation of object parsing functions
 */
typedef enum
{
    FC_OBJ_INPUT = 0,
    FC_OBJ_OUTPUT,
    FC_OBJ_INFERENCE,
} fc_obj_type;

#define IS_FC_OBJ_TYPE(TYPE) (((TYPE) == FC_OBJ_INPUT) || \
                              ((TYPE) == FC_OBJ_OUTPUT) || \
                              ((TYPE) == FC_OBJ_INFERENCE))

/*==================================================================================
    Base object
==================================================================================*/

struct fc_obj
{
    const char* name;

    fc_obj_type type;
};

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_OBJ_H__
