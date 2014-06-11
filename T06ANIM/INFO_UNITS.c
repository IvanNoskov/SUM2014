/* NI01, INFO_UNITS.C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "anim.h"

/*--- INFO Display unit ---*/

/* INFODisplay unit
 * render function
 * base unit render arguments */
static VOID INFODisplayUnitRender( in1UNIT *Unit, in1ANIM *Ani )
{
  static CHAR Buf[1000];

  SetBkMode(Ani->hDC, TRANSPARENT);
  SetTextColor(Ani->hDC, RGB(255, 255, 155));
  TextOut(Ani->hDC, Ani->W - 100, 0, Buf, sprintf(Buf, "FPS: %.3f", Ani->FPS));
  SetTextColor(Ani->hDC, RGB(155, 255, 255));
  TextOut(Ani->hDC, Ani->W - 300, 16, Buf, sprintf(Buf, "X: %.3f, Y: %.3f, Z: %.3f, R: %.3f, POV%i", Ani->JsX, Ani->JsY, Ani->JsZ, Ani->JsR, Ani->JsPOV));
}

/* INFODisplay unit
 * Response function
 * base unit render arguments */
static VOID INFODisplayUnitResponse( in1UNIT *Unit, in1ANIM *Ani )
{ 
  if (Ani->KeysClick[VK_ESCAPE])
    DestroyWindow( Ani->hWnd );
  if (Ani->KeysClick['F'])
    IN1_AnimFlipFullScreen( );
  if (Ani->KeysClick['P'])
    IN1_AnimSetPause( !Ani->IsPause );
}


/* INFO Display unit
 * creation function */
in1UNIT * IN1_INFODisplayUnitCreate( VOID )
{
  /* INFODisplay unit cpeation pointer */
  in1UNIT *Unit;

  /* getting memory | defolte unit model for INFODisplay unit */
  if ((Unit = IN1_AnimUnitCreate(sizeof(in1UNIT))) == NULL)
  /* no awailabe memory | INFODisplay unit model ERROR */
    return NULL;
  /* INFODisplay unit buse function initialization */
  Unit->Render = (VOID *)INFODisplayUnitRender;
  Unit->Response = (VOID *)INFODisplayUnitResponse;
  /* ready INFODisplay unit pointer return */
  return Unit;
}



/*--- Logo unit ---*/



/* LOGO unit
 * data tipe */
typedef struct tagin1UNIT_LOGO
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data */
  in1IMAGE XOR_mask, AND_mask;   /* picture masks */
} in1UNIT_LOGO;

/* LOGO unit
 * initialization function
 * base unit initialization arguments */
static VOID LOGOUnitInit( in1UNIT_LOGO *Unit, in1ANIM *Ani )
{
  IN1_ImageLoad ( &(Unit->AND_mask), "AND_mask.bmp" );
  IN1_ImageLoad ( &(Unit->XOR_mask), "XOR_mask.bmp" );
}

/* LOGO unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID LOGOUnitClose( in1UNIT_LOGO *Unit, in1ANIM *Ani )
{
  IN1_ImageFree ( &(Unit->AND_mask) );
  IN1_ImageFree ( &(Unit->XOR_mask) );
}

/* LOGO unit
 * response function
 * base unit response arguments */
static VOID LOGOUnitResponse( in1UNIT_LOGO *Unit, in1ANIM *Ani )
{
} 

/* LOGO unit
 * render function
 * base unit render arguments */
static VOID LOGOUnitRender( in1UNIT_LOGO *Unit, in1ANIM *Ani )
{
  IN1_ImagePaint( Ani->hDC, Ani->W - Unit -> AND_mask.W, 10, &(Unit->AND_mask), SRCAND);
  IN1_ImagePaint( Ani->hDC, Ani->W - Unit -> XOR_mask.W, 10, &(Unit->XOR_mask), SRCINVERT);
} 

/* LOGO unit
 * creation function
 * set defolte unit data to defolte ( void ) unit functions
 * Size - Size of generating unit tipe
 */
in1UNIT * IN1_LOGOUnitCreate( VOID )
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