/* NI01 */

#ifndef __SHADER_H__
#define __SHADER_H__

#include "def.h"

/* Shder initialisation function */
UINT IN1_ShadProgInit( CHAR *VSFileName, CHAR *FSFileName );

/* Shder deinitialisation function */
VOID IN1_ShadProgClose( UINT ProgId );

#endif /* __SHADER_H__ */
