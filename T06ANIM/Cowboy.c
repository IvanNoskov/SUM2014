/* NI01, COWboy.C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* COWboy unit
 * data tipe */
typedef struct tagin1UNIT_COWboy
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data */
} in1UNIT_COWboy;

/* COWboy unit
 * initialization function
 * base unit initialization arguments */
static VOID CowboyUnitInit( in1UNIT_COWboy *Unit, in1ANIM *Ani )
{
} 

/* COWboy unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID CowboyUnitClose( in1UNIT_COWboy *Unit, in1ANIM *Ani )
{
} 

/* COWboy unit
 * response function
 * base unit response arguments */
static VOID CowboyUnitResponse( in1UNIT_COWboy *Unit, in1ANIM *Ani )
{
} 

/* COWboy unit
 * render function
 * base unit render arguments */
static VOID CowboyUnitRender( in1UNIT_COWboy *Unit, in1ANIM *Ani )
{
  DBL
    x = (Ani->W / 2) + (Ani->W / 2) * Ani->JsX,
    y = (Ani->H / 2) + (Ani->H / 2) * Ani->JsY;
  POINT X[4] = {{x + 30, y}, {x, y + 30}, {x - 30, y}, {x, y - 30}};
  SelectObject(Ani->hDC, GetStockObject(DC_BRUSH));
  SelectObject(Ani->hDC, GetStockObject(DC_PEN));
  SetDCBrushColor(Ani->hDC, RGB(255, 0, 0));
  SetDCPenColor(Ani->hDC, RGB(0, 0, 0));
  Polygon( Ani->hDC, X, 4);
} 

/* COWboy unit
 * creation function */
in1UNIT * IN1_CowboyUnitCreate( VOID )
{
  /* COW unit cpeation pointer */
  in1UNIT_COWboy *Unit;

  /* getting memory | defolte unit model for COW unit */
  if ((Unit = (in1UNIT_COWboy *)IN1_AnimUnitCreate(sizeof(in1UNIT_COWboy))) == NULL)
  /* no awailabe memory | COWboy unit model ERROR */
    return NULL;
  /* COWboy unit buse function initialization */
  Unit->Init = (VOID *)CowboyUnitInit;
  Unit->Close = (VOID *)CowboyUnitClose;
  Unit->Response = (VOID *)CowboyUnitResponse;
  Unit->Render = (VOID *)CowboyUnitRender;
  /* ready COWboy unit pointer return */
  return (in1UNIT *)Unit;
}