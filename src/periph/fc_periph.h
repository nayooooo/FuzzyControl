#ifndef __FC_PERIPH_H__
#define __FC_PERIPH_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifndef FC_PERIPH_USE_FILE_SAVE
#define FC_PERIPH_USE_FILE_SAVE 0
#endif

#if FC_PERIPH_USE_FILE_SAVE
#ifndef FC_PERIPH_USE_FILE_SAVE_STD
#define FC_PERIPH_USE_FILE_SAVE_STD 1
#endif
#endif

#if FC_PERIPH_USE_FILE_SAVE_STD
#include "fc_fileSave.h"
#endif

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  // !__FC_PERIPH_H__
