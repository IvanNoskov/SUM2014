/* NI01, GOST.C */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "units.h"

#include "anim.h"
                 
/* GOST unit
 * initialization function
 * base unit initialization arguments */
static VOID GOSTUnitInit( in1UNIT_GOST *Unit, in1ANIM *Ani )
{
} 

/* GOST unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID GOSTUnitClose( in1UNIT_GOST *Unit, in1ANIM *Ani )
{
} 

/* GOST unit
 * Collision function
 * base unit Collision arguments */
static VOID GOSTUnitCollision( in1UNIT_GOST *Unit, in1ANIM *Ani )
{
  if (Unit->Key_found >= Unit->Key_Needet)
    DestroyWindow( Ani->hWnd );
} 

/* GOST unit
 * response function
 * base unit response arguments */
static VOID GOSTUnitResponse( in1UNIT_GOST *Unit, in1ANIM *Ani )
{
  MATRIXd R;
  VEC M;                         
  if((Ani->KeysClick[VK_NUMPAD9] || Ani->JsButClick[4]) && !Ani->IsPause)
  {
    Unit->Head.Dir = VecNormalize( VecNeg( Unit->Head.Loc ) );
    Unit->Head.Right = VecNormalize( VecCrossVec( Unit->Head.Dir, VecSet( 0, 1, 0 ) ) );
    Unit->Head.Up = VecCrossVec( Unit->Head.Right, Unit->Head.Dir );
  }
  R = MatrMulMatr( MatrMulMatr( MatrRotateVec( Ani->DeltaTime * Ani->JsR * 30, Unit->Head.Dir.X,  Unit->Head.Dir.Y,  Unit->Head.Dir.Z ),
                                MatrRotateVec( Ani->DeltaTime * Ani->JsY * 30, Unit->Head.Right.X,  Unit->Head.Right.Y,  Unit->Head.Right.Z ) ), 
                   MatrRotateVec( Ani->DeltaTime * Ani->JsX * 30, Unit->Head.Up.X,  Unit->Head.Up.Y,  Unit->Head.Up.Z ) );
  M = VecSet( Unit->Head.Dir.X * Ani->JsZ * 3 * Ani->DeltaTime, 
              Unit->Head.Dir.Y * Ani->JsZ * 3 * Ani->DeltaTime, 
              Unit->Head.Dir.Z * Ani->JsZ * 3 * Ani->DeltaTime );
  if (Ani->JsPOV == 1 || Ani->JsPOV == 2 || Ani->JsPOV == 8)
    M = VecAddVec( M, VecMulNum( Unit->Head.Dir,  Ani->DeltaTime * 3 ) );
  if (Ani->JsPOV == 4 || Ani->JsPOV == 5 || Ani->JsPOV == 6)
    M = VecAddVec( M, VecMulNum( Unit->Head.Dir, -Ani->DeltaTime * 3 ) );
  if (Ani->JsPOV == 2 || Ani->JsPOV == 3 || Ani->JsPOV == 4)
    M = VecAddVec( M, VecMulNum( Unit->Head.Right,  Ani->DeltaTime * 3 ) );  
  if (Ani->JsPOV == 6 || Ani->JsPOV == 7 || Ani->JsPOV == 8)
    M = VecAddVec( M, VecMulNum( Unit->Head.Right, -Ani->DeltaTime * 3 ) );  
  Unit->Head.Dir = VectorTransformer( Unit->Head.Dir, R );
  Unit->Head.Right = VectorTransformer( Unit->Head.Right, R );
  Unit->Head.Up = VectorTransformer( Unit->Head.Up, R );
  Unit->Head.Loc = VecAddVec( Unit->Head.Loc, M );

} 

/* GOST unit
 * render function
 * base unit render arguments */
static VOID GOSTUnitRender( in1UNIT_GOST *Unit, in1ANIM *Ani )
{
}

/* GOST unit
 * creation function */
in1UNIT * IN1_GOSTUnitCreate( INT ID, INT NeedKeys )
{
  /* GOST unit cpeation pointer */
  in1UNIT_GOST *Unit;

  /* getting memory | defolte unit model for GOST unit */
  if ((Unit = (in1UNIT_GOST *)IN1_AnimUnitCreate(sizeof(in1UNIT_GOST))) == NULL)
  /* no awailabe memory | GOST unit model ERROR */
    return NULL;
  /* GOST unit buse function initialization */
  Unit->ID = ID;
  Unit->TipeID = IN1_UNIT_GOST;
  Unit->Init = (VOID *)GOSTUnitInit;
  Unit->Close = (VOID *)GOSTUnitClose;
  Unit->Response = (VOID *)GOSTUnitResponse;
  Unit->Render = (VOID *)GOSTUnitRender;
  Unit->Collision = (VOID *)GOSTUnitCollision;
  /* GOST unit data initialization */
  Unit->Head.Dir = VecSet( 1, 0, 0);
  Unit->Head.Right = VecSet( 0, 0, -1);
  Unit->Head.Up = VecSet( 0, 1, 0);
  Unit->Head.Loc = VecSet( 0, 0, 0);
  Unit->Key_found = 0;
  Unit->Key_Needet = NeedKeys;
  /* ready GOST unit pointer return */
  return (in1UNIT *)Unit;
} 
