/* NI01, HOUSE.C */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "shader.h"
                 
typedef struct tagin1UNIT_HOUSE
{
  IN1_UNIT_BASE_FIELDS;        /* base anim unit data        */
  MATRIXd Position[3];         /* House Pos matrix           */
  in1GEOM Model;
} in1UNIT_HOUSE;

/* HOUSE unit
 * initialization function
 * base unit initialization arguments */
static VOID HOUSEUnitInit( in1UNIT_HOUSE *Unit, in1ANIM *Ani )

{
  IN1_GeomLoad( &(Unit->Model), "Models\\Houses\\house1.object" );
///  IN1_GeomLoad( &(Unit->Model), "Z:\\sum2014\\T07ANIM\\Models\\Avent\\avent.object" );
///  IN1_GeomLoad( &(Unit->Model), "Z:\\sum2014\\T07ANIM\\Models\\cow.object" );
} 

/* HOUSE unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID HOUSEUnitClose( in1UNIT_HOUSE *Unit, in1ANIM *Ani )
{
  IN1_GeomFree( &(Unit->Model) );
} 

/* HOUSE unit
 * Collision function
 * base unit Collision arguments */
static VOID HOUSEUnitCollision( in1UNIT_HOUSE *Unit, in1ANIM *Ani )
{
} 

/* HOUSE unit
 * response function
 * base unit response arguments */
static VOID HOUSEUnitResponse( in1UNIT_HOUSE *Unit, in1ANIM *Ani )
{
} 

/* HOUSE unit
 * render function
 * base unit render arguments */
static VOID HOUSEUnitRender( in1UNIT_HOUSE *Unit, in1ANIM *Ani )
{
  IN1_ShaderProg = IN1_ShadProgInit( "a.vert" ,"a.frag" );
  
  srand( 30 );
  Ani->PrjMWorld = MatrMulMatr( Unit->Position[0], MatrRotateX( 90 * (rand() % 3) * Ani->Time ) );
  IN1_AnimPrjMResponse();
  glLoadMatrixd( Ani->PrjM_WVP.A[0] );
  IN1_GeomDraw(&Unit->Model);
  Ani->PrjMWorld = MatrMulMatr( MatrRotateZ( 30 * (rand() % 3) * Ani->Time ), Unit->Position[1] );
  IN1_AnimPrjMResponse();
  glLoadMatrixd( Ani->PrjM_WVP.A[0] );
  IN1_GeomDraw(&Unit->Model);
  Ani->PrjMWorld = MatrMulMatr( MatrRotateVec( -45 * (rand() % 3) * Ani->Time, rand() % 5 - 2, rand() % 5 - 2, rand() % 5 - 2 ), Unit->Position[2] );
  IN1_AnimPrjMResponse();
  glLoadMatrixd( Ani->PrjM_WVP.A[0] );
  IN1_GeomDraw(&Unit->Model);
  Ani->PrjMWorld = MatrIdenity();
  IN1_AnimPrjMResponse();
  glLoadMatrixd( MatrIdenity().A[0] );

  IN1_ShadProgClose( IN1_ShaderProg );
}

/* HOUSE unit
 * creation function */
in1UNIT * IN1_HOUSEUnitCreate( INT ID )
{
  /* HOUSE unit cpeation pointer */
  in1UNIT_HOUSE *Unit;

  /* getting memory | defolte unit model for HOUSE unit */
  if ((Unit = (in1UNIT_HOUSE *)IN1_AnimUnitCreate(sizeof(in1UNIT_HOUSE))) == NULL)
  /* no awailabe memory | HOUSE unit model ERROR */
    return NULL;
  /* HOUSE unit buse function initialization */
  Unit->ID = ID;
  Unit->TipeID = IN1_UNIT_HOUSE;
  Unit->Init = (VOID *)HOUSEUnitInit;
  Unit->Close = (VOID *)HOUSEUnitClose;
  Unit->Response = (VOID *)HOUSEUnitResponse;
  Unit->Render = (VOID *)HOUSEUnitRender;
  Unit->Collision = (VOID *)HOUSEUnitCollision;
  /* HOUSE unit data initialization */
  Unit->Position[0] = MatrTranslate( -5, -6, 8 );
  Unit->Position[1] = MatrTranslate( 5, -16, 0 );
  Unit->Position[2] = MatrTranslate( 0, -5, -3 );
  /* ready HOUSE unit pointer return */
  return (in1UNIT *)Unit;
} 
