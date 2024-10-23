#ifndef __FC_CONFIG__
#define __FC_CONFIG__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef nullptr
#   define nullptr ((void*)0)
#endif

#ifndef UNUSED
#   define UNUSED(x) ((void)x)
#endif

#ifndef FUZZY_DATA_ILLEGAL_VALUE
#   define FUZZY_DATA_ILLEGAL_VALUE (-1)
#endif

typedef int32_t fc_index;
typedef uint32_t fc_size;

#ifdef ARDUINO
    extern int arduino_printf(const char* format, ...);
#   define fc_printf arduino_printf
#else
#   define fc_printf printf
#endif

#define fc_malloc malloc
#define fc_realloc realloc
#define fc_free free

#define fc_memcpy memcpy
#define fc_memset memset

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

#ifndef nullptr
#   define nullptr ((void*)0)
#endif

#ifndef UNUSED
#   define UNUSED(x) ((void)x)
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_CONFIG__
