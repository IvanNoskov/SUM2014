
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "VCT.h"
#include "RENDER.H"


typedef struct tagin1EDGE
{
  INT P0, P1;
} EDGE;

typedef struct tagin1UNIT_CUBE
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data */
  VEC cubeP[8];   
  EDGE cubeE[12];  
  INT Seed,
      Tipe;
} in1UNIT_CUBE;

static EDGE setEDGE (INT P0, INT P1)
{
  EDGE AQ;
  AQ.P0 = P0;
  AQ.P1 = P1;
  return AQ;
}

/* CUBE unit
 * initialization function
 * base unit initialization arguments */
static VOID CUBEUnitInit( in1UNIT_CUBE *Unit, in1ANIM *Ani )
{
} 

/* CUBE unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID CUBEUnitClose( in1UNIT_CUBE *Unit, in1ANIM *Ani )
{
} 

/* CUBE unit
 * response function
 * base unit response arguments */
static VOID CUBEUnitResponse( in1UNIT_CUBE *Unit, in1ANIM *Ani )
{
 INT j;
  MATRIX M;
  srand( Unit->Seed++ );
  M = MatrRotateVec( 180 * Ani->DeltaTime * Ani->JsZ, sin( rand( ) ), sin( rand( ) ), sin( rand( ) ) );
  for (j = 0; j < 8; j++)
    Unit->cubeP[j] = PointTransformer( Unit->cubeP[j], M );/**/
} 

/* CUBE unit
 * render function
 * base unit render arguments */
static VOID CUBEUnitRender( in1UNIT_CUBE *Unit, in1ANIM *Ani )
{
  POINT pts[8];
  INT i;
  SelectObject( Ani->hDC, GetStockObject(NULL_BRUSH) );
  SelectObject( Ani->hDC, GetStockObject(DC_PEN) );
  srand( Unit->Seed );
  switch (Unit->Tipe)
  {
  case 0:
  case 4:
    SetDCPenColor( Ani->hDC, RGB( rand( ), rand( ), rand( ) ) );
    break;
  case 1:
  case 5:
  case 7:
    SetDCPenColor( Ani->hDC, RGB( 255 * sin( rand( ) ), 255 * cos( rand( ) ), 255 * tan( rand( ) ) ) );
    break;
  case 2:
  case 6:
    SetDCPenColor( Ani->hDC, RGB( 255 - rand( ), 255 - rand( ), 255 - rand( ) ) );
    break;
  case 3:
    SetDCPenColor( Ani->hDC, RGB( 200, 200, 200 ) );
    break;
  }
  for (i = 0; i < 8; i++)
  {
    pts[i] = IN1_WorldToScreen( Unit->cubeP[i] );
    Ellipse( Ani->hDC, pts[i].x - 6, pts[i].y - 6, pts[i].x + 6, pts[i].y + 6);
  }
  for (i = 0; i < 12; i++)
  {
    MoveToEx( Ani->hDC, pts[Unit->cubeE[i].P0].x, pts[Unit->cubeE[i].P0].y, NULL );
    LineTo( Ani->hDC, pts[Unit->cubeE[i].P1].x, pts[Unit->cubeE[i].P1].y );
    /*Line( pts[Unit->cubeE[i].P0].x, pts[Unit->cubeE[i].P0].y, pts[Unit->cubeE[i].P1].x, pts[Unit->cubeE[i].P1].y );*/
  }

} 

/* CUBE unit
 * creation function */
in1UNIT * IN1_CUBEUnitCreate( VOID )
{
  /* CUBE unit cpeation pointer */
  in1UNIT_CUBE *Unit;

  /* getting memory | defolte unit model for CUBE unit */
  if ((Unit = (in1UNIT_CUBE *)IN1_AnimUnitCreate(sizeof(in1UNIT_CUBE))) == NULL)
  /* no awailabe memory | CUBE unit model ERROR */
    return NULL;
  /* CUBE unit buse function initialization */
  Unit->Init = (VOID *)CUBEUnitInit;
  Unit->Close = (VOID *)CUBEUnitClose;
  Unit->Response = (VOID *)CUBEUnitResponse;
  Unit->Render = (VOID *)CUBEUnitRender;
  /* CUBE unit data initialization */
  Unit->cubeP[0] = VecSet( 2, 2, 2 );
  Unit->cubeP[1] = VecSet( 2, 2, -2 );
  Unit->cubeP[2] = VecSet( 2, -2, 2 );
  Unit->cubeP[3] = VecSet( 2, -2, -2 );
  Unit->cubeP[4] = VecSet( -2, 2, 2 );
  Unit->cubeP[5] = VecSet( -2, 2, -2 );
  Unit->cubeP[6] = VecSet( -2, -2, 2 );
  Unit->cubeP[7] = VecSet( -2, -2, -2 );
  Unit->cubeE[0] = setEDGE( 0, 1 );
  Unit->cubeE[1] = setEDGE( 0, 2 );
  Unit->cubeE[2] = setEDGE( 1, 3 );
  Unit->cubeE[3] = setEDGE( 2, 3 );
  Unit->cubeE[4] = setEDGE( 4, 5 );
  Unit->cubeE[5] = setEDGE( 4, 6 );
  Unit->cubeE[6] = setEDGE( 5, 7 );
  Unit->cubeE[7] = setEDGE( 6, 7 );
  Unit->cubeE[8] = setEDGE( 0, 4 );
  Unit->cubeE[9] = setEDGE( 1, 5 );
  Unit->cubeE[10] = setEDGE( 2, 6 );
  Unit->cubeE[11] = setEDGE( 3, 7 );
  Unit->Seed = rand();
  Unit->Tipe = rand() % 7;
  /* ready CUBE unit pointer return */
  return (in1UNIT *)Unit;
} 
