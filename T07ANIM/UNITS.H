/* NI01, UNITS.H */


#ifndef __UNITS_H__
#define __UNITS_H__

#include "anim.h"

/* INFO Display unit creation function 
 * Tipe_ID = IN1_UNIT_INFO*/
in1UNIT * IN1_INFODisplayUnitCreate( INT ID );

/* INFO Display unit creation function 
 * Tipe_ID = IN1_UNIT_EYE */
in1UNIT * IN1_CAMERAUnitCreate( INT ID );

in1UNIT * IN1_KEYUnitCreate( INT ID, VEC Position );

typedef struct tagin1UNIT_GOST
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data        */
  in1CAMERA Head;       /* position data              */
  in1GEOM Model;        /* Gost Model                 */
  INT Key_found,        /* Number of foundet keys     */
      Key_Needet;       /* How many keys gost need    */
} in1UNIT_GOST;

in1UNIT * IN1_GOSTUnitCreate( INT ID );

in1UNIT * IN1_HOUSEUnitCreate( INT ID );



#endif /* __UNITS_H__ */
