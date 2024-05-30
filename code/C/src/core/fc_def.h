#ifndef __FC_DEF_H__
#define __FC_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef nullptr
#   define nullptr ((void*)0)
#endif

#ifndef UNUSED
#   define UNUSED(x) ((void)x)
#endif

#ifndef FUZZY_DATA_ILLEGAL_VALUE
#   define FUZZY_DATA_ILLEGAL_VALUE (-1)
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_DEF_H__