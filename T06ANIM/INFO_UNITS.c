/* NI01, INFO_UNITS.C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "anim.h"

/*--- Frame Per Second Display unit ---*/

/* COW unit
 * render function
 * base unit render arguments */
static VOID FPSDisplayUnitRender( in1UNIT *Unit, in1ANIM *Ani )
{
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, 10, 10, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
}

/* Frame Per Second Display unit
 * creation function */
in1UNIT * IN1_FPSDisplayUnitCreate( VOID )
{
  /* FPSDisplay unit cpeation pointer */
  in1UNIT *Unit;

  /* getting memory | defolte unit model for FPSDisplay unit */
  if ((Unit = IN1_AnimUnitCreate(sizeof(in1UNIT))) == NULL)
  /* no awailabe memory | FPSDisplay unit model ERROR */
    return NULL;
  /* FPSDisplay unit buse function initialization */
  Unit->Render = (VOID *)FPSDisplayUnitRender;
  /* ready FPSDisplay unit pointer return */
  return Unit;
}



/*--- Logo unit ---*/



/* LOGO unit
 * data tipe */
typedef struct tagin1UNIT_LOGO
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data */
  IMAGE XOR_mask, AND_mask;   /* move */
  INT X, Y;             /* Tipe of cow */
} in1UNIT_LOGO;

/* LOGO unit
 * initialization function
 * base unit initialization arguments */
static VOID LOGOUnitInit( in1UNIT *Unit, in1ANIM *Ani )
{
}

/* LOGO unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID LOGOUnitClose( in1UNIT *Unit, in1ANIM *Ani )
{
}

/* LOGO unit
 * response function
 * base unit response arguments */
static VOID LOGOUnitResponse( in1UNIT *Unit, in1ANIM *Ani )
{
} 

/* LOGO unit
 * render function
 * base unit render arguments */
static VOID LOGOUnitRender( in1UNIT *Unit, in1ANIM *Ani )
{
} 

/* LOGO unit
 * creation function
 * set defolte unit data to defolte ( void ) unit functions
 * Size - Size of generating unit tipe
 */
in1UNIT * LOGOUnitCreate( INT Size )
{
  /* LOGO unit cpeation pointer */
  in1UNIT_LOGO *Unit;

  /* getting memory | defolte unit model for LOGO unit */
  if ((Unit = (in1UNIT_LOGO *)IN1_AnimUnitCreate(sizeof(in1UNIT_LOGO))) == NULL)
  /* no awailabe memory | LOGO unit model ERROR */
    return NULL;
  /* LOGO unit buse function initialization */
  Unit->Init = (VOID *)LOGOUnitInit;
  Unit->Close = (VOID *)LOGOUnitClose;
  Unit->Response = (VOID *)LOGOUnitResponse;
  Unit->Render = (VOID *)LOGOUnitRender;
  /* ready LOGO unit pointer return */
  return (in1UNIT *)Unit;
}