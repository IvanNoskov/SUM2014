/* NI01, UNIT.C */

#include <stdlib.h>
#include <string.h>

#include "anim.h"


/* base unit initialization | deinitialization | response | render arguments
 * *Unit - pointer to worcing unit;
 * *Ani - pointer to active animation data;
 */

/* defolte ( void ) unit
 * initialization function
 * base unit initialization arguments */
static VOID IN1_AnimUnitInit( in1UNIT *Unit, in1ANIM *Ani )
{
}

/* defolte ( void ) unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID IN1_AnimUnitClose( in1UNIT *Unit, in1ANIM *Ani )
{
}

/* defolte ( void ) unit
 * response function
 * base unit response arguments */
static VOID IN1_AnimUnitResponse( in1UNIT *Unit, in1ANIM *Ani )
{
} 

/* defolte ( void ) unit
 * render function
 * base unit render arguments */
static VOID IN1_AnimUnitRender( in1UNIT *Unit, in1ANIM *Ani )
{
} 

/* (animation) unit
 * creation function
 * set defolte unit data to defolte ( void ) unit functions
 * Size - Size of generating unit tipe
 */
in1UNIT * IN1_AnimUnitCreate( INT Size )
{
  /* unit cpeation pointer */
  in1UNIT *Unit;

  /* getting memory for unit model */
  if (Size < sizeof(in1UNIT) || (Unit = malloc(Size)) == NULL)
  /* no awailabe memory | unawailable Size */
    return NULL;
  /* Fill All memory defolte ( 0 ) data */
  memset(Unit, 0, Size);
  /* unit buse defolt function initialization */
  Unit->Size = Size;
  Unit->Init = IN1_AnimUnitInit;
  Unit->Close = IN1_AnimUnitClose;
  Unit->Response = IN1_AnimUnitResponse;
  Unit->Render = IN1_AnimUnitRender;
  return Unit;
}