#ifndef __FC_MEM_H__
#define __FC_MEM_H__

#include <stdint.h>
#include <stdlib.h>

inline void *fc_mem_malloc(size_t _Size)
{
    return malloc(_Size);
}

inline void *fc_mem_realloc(void *_Memory, size_t _NewSize)
{
    return realloc(_Memory, _NewSize);
}

inline void fc_mem_free(void *_Memory)
{
    free(_Memory);
}

#endif  // !__FC_MEM_H__
