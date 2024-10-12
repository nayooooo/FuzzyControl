#ifndef __FC_STD_H__
#define __FC_STD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int32_t fc_index;
typedef uint32_t fc_size;

#define fc_printf printf

#define fc_malloc malloc
#define fc_realloc realloc
#define fc_free free

#define fc_memcpy memcpy

#define fc_strlen strlen
#define fc_strcpy strcpy
#define fc_strcpy_s strcpy_s
#define fc_strcmp strcmp
#define fc_strtok strtok
#define fc_strtok_s strtok_s
#define fc_strchr strchr
#define fc_strstr strstr

#include <math.h>

#define fc_exp exp

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_STD_H__
