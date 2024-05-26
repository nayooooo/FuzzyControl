#ifndef __FC_STD_H__
#define __FC_STD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>

#define fc_malloc malloc
#define fc_realloc realloc
#define fc_free free

#define fc_memcpy memcpy

#define fc_strcpy strcpy

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_STD_H__
