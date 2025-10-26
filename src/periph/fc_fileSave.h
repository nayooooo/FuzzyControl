#ifndef __FC_FILESAVE__
#define __FC_FILESAVE__

#if FC_PERIPH_USE_FILE_SAVE

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================
    micro function
==================================================================================*/

#define FC_FILESAVE_RULES_FILE_APPEND_STRING            "rules"

/*==================================================================================
    enmu
==================================================================================*/

typedef enum {
    FC_FILESAVE_TYPE_BINARY = 0,
    FC_FILESAVE_TYPE_MAX
} fc_fileSave_type_e;

/*==================================================================================
    structure
==================================================================================*/

struct fc_fileSave
{
    const char *route;
    const char *name;  // this is data file name, the rules will be save as
                       // {name}_{FC_FILESAVE_RULES_FILE_APPEND_STRING}
    fc_fileSave_type_e type;
};

/*==================================================================================
    API
==================================================================================*/

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  /* FC_PERIPH_USE_FILE_SAVE */

#endif  // !__FC_FILESAVE__
