/* NI01, KEY.C */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "anim.h"
#include "units.h"
                 
typedef struct tagin1UNIT_KEY
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data        */
  VEC Pos;              /* Key Position               */
  MATRIXd Rot,          /* Key Rotation matrix        */
          Trans;        /* Key Translation matrix     */
  BOOL isFound;         /* TRUE if key has been found */
} in1UNIT_KEY;

/* KEY unit
 * initialization function
 * base unit initialization arguments */
static VOID KEYUnitInit( in1UNIT_KEY *Unit, in1ANIM *Ani )
{
} 

/* KEY unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID KEYUnitClose( in1UNIT_KEY *Unit, in1ANIM *Ani )
{
} 

/* KEY unit
 * Collision function
 * base unit Collision arguments */
static VOID KEYUnitCollision( in1UNIT_KEY *Unit, in1ANIM *Ani )
{
  static INT i = -1;
  if (i == -1 || i >= Ani->NumOfUnits)
    for (i = 0; i < IN1_MAX_UNITS; i++)
      if (Ani->Units[i]->TipeID == IN1_UNIT_GOST)
        break;
  if (i >= 0 && i < Ani->NumOfUnits)
    if (VecLen2( VecSubVec( Unit->Pos, ((in1UNIT_GOST *)(Ani->Units[i]))->Head.Loc ) ) < 3 && !Unit->isFound)
    {
      Unit->isFound = TRUE;
      ((in1UNIT_GOST *)(Ani->Units[i]))->Key_found++;
    }
} 

/* KEY unit
 * response function
 * base unit response arguments */
static VOID KEYUnitResponse( in1UNIT_KEY *Unit, in1ANIM *Ani )
{
} 

/* KEY unit
 * render function
 * base unit render arguments */
static VOID KEYUnitRender( in1UNIT_KEY *Unit, in1ANIM *Ani )
{
  if (Unit->isFound)
    return;
  Unit->Rot = MatrMulMatr( Unit->Rot, MatrRotateVec( (rand() * 30.0 * 30 / RAND_MAX) * Ani->DeltaTime, tan( rand() ), tan( rand() ), tan( rand() ) ) );
  glLoadMatrixd( MatrMulMatr( MatrMulMatr( Unit->Rot, Unit->Trans ), Ani->PrjM_WVP ).A[0] );
  glLineWidth( 3 );
  glUseProgram( 0 );
  glBegin( GL_LINES );
  {
    glColor4d( sin( tan( cos( tan( rand() ) ) ) ) / 2 + 0.5, sin( tan( cos( tan( rand() ) ) ) ) / 2 + 0.5, sin( tan( cos( tan( rand() ) ) ) ) / 2 + 0.5, 0.5 );
    glVertex3d( -0.45, -0.45, -0.45 );
    glVertex3d( -0.45,  0.45,  0.45 );

    glVertex3d( -0.45, -0.45, -0.45 );
    glVertex3d(  0.45, -0.45,  0.45 );

    glVertex3d( -0.45, -0.45, -0.45 );
    glVertex3d(  0.45,  0.45, -0.45 );

    glVertex3d(  0.45,  0.45, -0.45 );
    glVertex3d( -0.45,  0.45,  0.45 );

    glVertex3d(  0.45, -0.45,  0.45 );
    glVertex3d( -0.45,  0.45,  0.45 );

    glVertex3d(  0.45, -0.45,  0.45 );
    glVertex3d(  0.45,  0.45, -0.45 );

    glColor4d( -sin( tan( cos( tan( rand() ) ) ) ) / 2 + 0.5, -sin( tan( cos( tan( rand() ) ) ) ) / 2 + 0.5, -sin( tan( cos( tan( rand() ) ) ) ) / 2 + 0.5, 0.5 );
    glVertex3d(  0.45,  0.45,  0.45 );
    glVertex3d(  0.45, -0.45, -0.45 );

    glVertex3d(  0.45,  0.45,  0.45 );
    glVertex3d( -0.45, -0.45,  0.45 );

    glVertex3d(  0.45,  0.45,  0.45 );
    glVertex3d( -0.45,  0.45, -0.45 );

    glVertex3d(  0.45, -0.45, -0.45 );
    glVertex3d( -0.45, -0.45,  0.45 );

    glVertex3d(  0.45, -0.45, -0.45 );
    glVertex3d( -0.45,  0.45, -0.45 );

    glVertex3d( -0.45, -0.45,  0.45 );
    glVertex3d( -0.45,  0.45, -0.45 );
  }
  glEnd();
}

/* KEY unit
 * creation function */
in1UNIT * IN1_KEYUnitCreate( INT ID, VEC Position )
{
  /* KEY unit cpeation pointer */
  in1UNIT_KEY *Unit;

  /* getting memory | defolte unit model for KEY unit */
  if ((Unit = (in1UNIT_KEY *)IN1_AnimUnitCreate(sizeof(in1UNIT_KEY))) == NULL)
  /* no awailabe memory | KEY unit model ERROR */
    return NULL;
  /* KEY unit buse function initialization */
  Unit->ID = ID;
  Unit->TipeID = IN1_UNIT_KEY;
  Unit->Init = (VOID *)KEYUnitInit;
  Unit->Close = (VOID *)KEYUnitClose;
  Unit->Response = (VOID *)KEYUnitResponse;
  Unit->Render = (VOID *)KEYUnitRender;
  Unit->Collision = (VOID *)KEYUnitCollision;
  /* KEY unit data initialization */
  Unit->isFound = FALSE;
  Unit->Rot = MatrIdenity();
  Unit->Trans = MatrTranslate( Position.X, Position.Y, Position.Z );
  Unit->Pos = Position;
  /* ready KEY unit pointer return */
  return (in1UNIT *)Unit;
} 
