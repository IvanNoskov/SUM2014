#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "RENDER.H"


typedef struct tagin1UNIT_CAR
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data */
  in1GOBJ Smf;
  INT N;
} in1UNIT_CAR;

/* CAR unit
 * initialization function
 * base unit initialization arguments */
static VOID CARUnitInit( in1UNIT_CAR *Unit, in1ANIM *Ani )
{
  IN1_RndGObjLoad( &(Unit->Smf), "Porsche_911_GT2.obj" );
} 

/* CAR unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID CARUnitClose( in1UNIT_CAR *Unit, in1ANIM *Ani )
{
  IN1_RndGObjFree( &(Unit->Smf) );
} 

/* CAR unit
 * response function
 * base unit response arguments */
static VOID CARUnitResponse( in1UNIT_CAR *Unit, in1ANIM *Ani )
{
  return;
} 

/* CAR unit
 * render function
 * base unit render arguments */
static VOID CARUnitRender( in1UNIT_CAR *Unit, in1ANIM *Ani )
{
  INT I;
  MATRIX M = MatrMulMatr( MatrTranslate( -5, -5, -0 ), MatrRotateY( Ani->Time * -90 ) );
  IN1_RndGObjDraw( &Unit->Smf, Ani->hDC, MatrMulMatr( MatrMulMatr( Ani->PrjMWorld, Ani->PrjMView ), Ani->PrjMProjection ) );
  IN1_RndGObjDraw( &Unit->Smf, Ani->hDC,MatrMulMatr( M, MatrMulMatr( MatrMulMatr( Ani->PrjMWorld, Ani->PrjMView ), Ani->PrjMProjection ) ) );
}

/* CAR unit
 * creation function */
in1UNIT * IN1_CARUnitCreate( INT I )
{
  /* CAR unit cpeation pointer */
  in1UNIT_CAR *Unit;

  /* getting memory | defolte unit model for CAR unit */
  if ((Unit = (in1UNIT_CAR *)IN1_AnimUnitCreate(sizeof(in1UNIT_CAR))) == NULL)
  /* no awailabe memory | CAR unit model ERROR */
    return NULL;
  /* CAR unit buse function initialization */
  Unit->Init = (VOID *)CARUnitInit;
  Unit->Close = (VOID *)CARUnitClose;
  Unit->Response = (VOID *)CARUnitResponse;
  Unit->Render = (VOID *)CARUnitRender;
  /* CAR unit data initialization */
  Unit->N = I;
  /* ready CAR unit pointer return */
  return (in1UNIT *)Unit;
} 
