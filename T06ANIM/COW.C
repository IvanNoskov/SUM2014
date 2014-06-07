/* NI01, COW.C */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"

/* COW unit
 * data tipe */
typedef struct tagin1UNIT_COW
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data */
  DBL ShiftX, ShiftY;   /* move */
  INT Type;             /* Tipe of cow */
} in1UNIT_COW;

/* COW unit
 * initialization function
 * base unit initialization arguments */
static VOID CowUnitInit( in1UNIT_COW *Unit, in1ANIM *Ani )
{
} 

/* COW unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID CowUnitClose( in1UNIT_COW *Unit, in1ANIM *Ani )
{
} 

/* COW unit
 * response function
 * base unit response arguments */
static VOID CowUnitResponse( in1UNIT_COW *Unit, in1ANIM *Ani )
{
} 

/* COW unit
 * render function
 * base unit render arguments */
static VOID CowUnitRender( in1UNIT_COW *Unit, in1ANIM *Ani )
{
  DBL
    x = (sin(Ani->Time + Unit->ShiftX) + 1) / 2 * Ani->W,
    y = (cos(Ani->Time + Unit->ShiftY) + 1) / 2 * Ani->H;

  if (Unit->Type)
    Rectangle(Ani->hDC, x - 30, y - 30, x + 30, y + 30);
  else
    Ellipse(Ani->hDC, x - 30, y - 30, x + 30, y + 30);
} 

/* COW unit
 * creation function */
in1UNIT * IN1_CowUnitCreate( VOID )
{
  /* COW unit cpeation pointer */
  in1UNIT_COW *Unit;

  /* getting memory | defolte unit model for COW unit */
  if ((Unit = (in1UNIT_COW *)IN1_AnimUnitCreate(sizeof(in1UNIT_COW))) == NULL)
  /* no awailabe memory | COW unit model ERROR */
    return NULL;
  /* COW unit buse function initialization */
  Unit->Init = (VOID *)CowUnitInit;
  Unit->Close = (VOID *)CowUnitClose;
  Unit->Response = (VOID *)CowUnitResponse;
  Unit->Render = (VOID *)CowUnitRender;
  /* COW unit data initialization */
  Unit->ShiftX = 30 * 30.59 * rand() / RAND_MAX;
  Unit->ShiftY = 30 * 30.59 * rand() / RAND_MAX;
  Unit->Type = rand() % 2;
  /* ready COW unit pointer return */
  return (in1UNIT *)Unit;
} 