#ifndef __FC_ERROR_H__
#define __FC_ERROR_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum{
    FC_EOK = 0,
    FC_ERROR,
    FC_ERROR_INPUT_RAW_DATA,
    FC_ERROR_INPUT_FUZZY_DATA,
    FC_ERROR_OUTPUT_RAW_DATA,
    FC_ERROR_OUTPUT_FUZZY_DATA,
}fc_err_t;

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_ERROR_H__
