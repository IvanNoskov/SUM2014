#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "VCT.h"
#include "RENDER.H"


typedef struct tagin1UNIT_SMTH
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data */
  vg4GOBJ Smf;
  INT N;
} in1UNIT_SMTH;

/* SMTH unit
 * initialization function
 * base unit initialization arguments */
static VOID SMTHUnitInit( in1UNIT_SMTH *Unit, in1ANIM *Ani )
{
  VG4_RndGObjLoad( &(Unit->Smf), "Porsche_911_GT2.obj" );
} 

/* SMTH unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID SMTHUnitClose( in1UNIT_SMTH *Unit, in1ANIM *Ani )
{
  VG4_RndGObjFree( &(Unit->Smf) );
} 

/* SMTH unit
 * response function
 * base unit response arguments */
static VOID SMTHUnitResponse( in1UNIT_SMTH *Unit, in1ANIM *Ani )
{
  return;
} 

/* SMTH unit
 * render function
 * base unit render arguments */
static VOID SMTHUnitRender( in1UNIT_SMTH *Unit, in1ANIM *Ani )
{
  INT I;
  MATRIX M;
/*  M = MatrRotateX( Ani->DeltaTime * 30 );
  for (I = 0; I < Unit->Smf.NumOfV; I++)
    Unit->Smf.V[I] = PointTransformer( Unit->Smf.V[I], M ); */
  VG4_RndGObjDraw( &Unit->Smf, Ani->hDC );
  M = MatrTranslate( -10, 0, -30 );
  for (I = 0; I < Unit->Smf.NumOfV; I++)
    Unit->Smf.V[I] = PointTransformer( Unit->Smf.V[I], M );
  VG4_RndGObjDraw( &Unit->Smf, Ani->hDC );
  M = MatrInverse( M );
  for (I = 0; I < Unit->Smf.NumOfV; I++)
    Unit->Smf.V[I] = PointTransformer( Unit->Smf.V[I], M );
} 

/* SMTH unit
 * creation function */
in1UNIT * IN1_SMTHUnitCreate( INT I )
{
  /* SMTH unit cpeation pointer */
  in1UNIT_SMTH *Unit;

  /* getting memory | defolte unit model for SMTH unit */
  if ((Unit = (in1UNIT_SMTH *)IN1_AnimUnitCreate(sizeof(in1UNIT_SMTH))) == NULL)
  /* no awailabe memory | SMTH unit model ERROR */
    return NULL;
  /* SMTH unit buse function initialization */
  Unit->Init = (VOID *)SMTHUnitInit;
  Unit->Close = (VOID *)SMTHUnitClose;
  Unit->Response = (VOID *)SMTHUnitResponse;
  Unit->Render = (VOID *)SMTHUnitRender;
  /* SMTH unit data initialization */
  Unit->N = I;
  /* ready SMTH unit pointer return */
  return (in1UNIT *)Unit;
} 
