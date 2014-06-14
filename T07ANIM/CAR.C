#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "RENDER.H"
#include "Shader.H"


typedef struct tagin1UNIT_CAR
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data */
  in1GOBJ Smf;
  INT Shader;
} in1UNIT_CAR;

/* CAR unit
 * initialization function
 * base unit initialization arguments */
static VOID CARUnitInit( in1UNIT_CAR *Unit, in1ANIM *Ani )
{
  IN1_RndGObjLoad( &(Unit->Smf), "Porsche_911_GT2.obj" );
  Unit->Shader = IN1_ShadProgInit( "a.vert", "a.frag" ); 
} 

/* CAR unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID CARUnitClose( in1UNIT_CAR *Unit, in1ANIM *Ani )
{
  IN1_RndGObjFree( &(Unit->Smf) );
  IN1_ShadProgClose( Unit->Shader );
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
  INT I, loc;
  MATRIXd M = MatrMulMatr( MatrTranslate( -3, -3, -0 ), MatrRotateY( Ani->Time * -90 ) ), 
          WVP = MatrMulMatr( MatrMulMatr( Ani->PrjMWorld, Ani->PrjMView ), Ani->PrjMProjection );
  glDepthMask( 1 );
  glEnable( GL_DEPTH_TEST );
  glUseProgram( Unit->Shader );
  loc = glGetUniformLocation( Unit->Shader, "Matr" );
  if (loc != -1)
    glUniformMatrix4fv( loc, 1, FALSE, ShaderMatrTransfom( WVP ).A[0] );
  loc = glGetUniformLocation( Unit->Shader, "Time");
  if (loc != -1)
    glUniform1f( loc, Ani->Time );

  glColor3d( 1, 1, 1 );
  IN1_RndGObjDraw( &Unit->Smf, Ani->hDC, WVP );
  glColor3d( 1, 0, 0 );
  loc = glGetUniformLocation( Unit->Shader, "Matr" );
  if (loc != -1)
    glUniformMatrix4fv( loc, 1, FALSE, ShaderMatrTransfom( MatrMulMatr( M, WVP ) ).A[0] );
  IN1_RndGObjDraw( &Unit->Smf, Ani->hDC, MatrMulMatr( M, WVP ) );
  M = MatrMulMatr( M, M );
  glColor3d( 0, 1, 0 );
  loc = glGetUniformLocation( Unit->Shader, "Matr" );
  if (loc != -1)
    glUniformMatrix4fv( loc, 1, FALSE, ShaderMatrTransfom( MatrMulMatr( M, WVP ) ).A[0] );
  IN1_RndGObjDraw( &Unit->Smf, Ani->hDC, MatrMulMatr( M, WVP ) );
  M = MatrMulMatr( M, M );
  glColor3d( 0, 0, 1 );
  loc = glGetUniformLocation( Unit->Shader, "Matr" );
  if (loc != -1)
    glUniformMatrix4fv( loc, 1, FALSE, ShaderMatrTransfom( MatrMulMatr( M, WVP ) ).A[0] );
  IN1_RndGObjDraw( &Unit->Smf, Ani->hDC, MatrMulMatr( M, WVP ) );
  glDisable( GL_DEPTH_TEST );
  glUseProgram( 0 );
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
  /* ready CAR unit pointer return */
  return (in1UNIT *)Unit;
} 
