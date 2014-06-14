/* NI01, STARTUP.C */

#include "anim.h"

typedef struct tagin1UNIT_CAMERA
{
  IN1_UNIT_BASE_FIELDS; /* base anim unit data */
  INT MODE;             /* Camera mode: 1 - orbit by (0, 0, 0); 0 - free*/
} in1UNIT_CAMERA;

/* CAMERA unit
 * initialization function
 * base unit initialization arguments */
static VOID CAMERAUnitInit( in1UNIT_CAMERA *Unit, in1ANIM *Ani )
{
} 

/* CAMERA unit
 * deinitialization function
 * base unit deinitialization arguments */
static VOID CAMERAUnitClose( in1UNIT_CAMERA *Unit, in1ANIM *Ani )
{
} 

/* CAMERA unit
 * response function
 * base unit response arguments */
static VOID CAMERAUnitResponse( in1UNIT_CAMERA *Unit, in1ANIM *Ani )
{
  MATRIXd R;
  VEC M;
  if(Ani->KeysClick['C'])
    Unit->MODE = (Unit->MODE + 1) % 2;
  if(Ani->KeysClick[VK_NUMPAD1])
    Unit->MODE = 1;  
  if(Ani->KeysClick[VK_NUMPAD0])
    Unit->MODE = 0;
  if(Ani->KeysClick[VK_NUMPAD9])
  {
    Ani->Eye.Dir = VecNormalize( VecNeg( Ani->Eye.Loc ) );
    Ani->Eye.Right = VecNormalize( VecCrossVec( Ani->Eye.Dir, VecSet( 0, 1, 0 ) ) );
    Ani->Eye.Up = VecCrossVec( Ani->Eye.Right, Ani->Eye.Dir );
  }
  switch(Unit->MODE)
  {
  case(0):
    R = MatrMulMatr( MatrMulMatr( MatrRotateVec( Ani->GlobalDeltaTime * Ani->JsR * 30, Ani->Eye.Dir.X,  Ani->Eye.Dir.Y,  Ani->Eye.Dir.Z ),
                                  MatrRotateVec( Ani->GlobalDeltaTime * Ani->JsY * 30, Ani->Eye.Right.X,  Ani->Eye.Right.Y,  Ani->Eye.Right.Z ) ), 
                     MatrRotateVec( Ani->GlobalDeltaTime * Ani->JsX * 30, Ani->Eye.Up.X,  Ani->Eye.Up.Y,  Ani->Eye.Up.Z ) );
    M = VecSet( 0, 0, 0 );
    if (Ani->JsPOV == 1 || Ani->JsPOV == 2 || Ani->JsPOV == 8)
      M = VecAddVec( M, VecMulNum( Ani->Eye.Dir,  Ani->GlobalDeltaTime * 3 ) );
    if (Ani->JsPOV == 4 || Ani->JsPOV == 5 || Ani->JsPOV == 6)
      M = VecAddVec( M, VecMulNum( Ani->Eye.Dir, -Ani->GlobalDeltaTime * 3 ) );
    if (Ani->JsPOV == 2 || Ani->JsPOV == 3 || Ani->JsPOV == 4)
      M = VecAddVec( M, VecMulNum( Ani->Eye.Right,  Ani->GlobalDeltaTime * 3 ) );  
    if (Ani->JsPOV == 6 || Ani->JsPOV == 7 || Ani->JsPOV == 8)
      M = VecAddVec( M, VecMulNum( Ani->Eye.Right, -Ani->GlobalDeltaTime * 3 ) );  
    Ani->Eye.Dir = VectorTransformer( Ani->Eye.Dir, R );
    Ani->Eye.Right = VectorTransformer( Ani->Eye.Right, R );
    Ani->Eye.Up = VectorTransformer( Ani->Eye.Up, R );
    Ani->Eye.Loc = VecAddVec( Ani->Eye.Loc, M );
    break;
  case(1):
    R = MatrMulMatr( MatrMulMatr( MatrRotateVec( Ani->GlobalDeltaTime * Ani->JsR * 30, Ani->Eye.Dir.X,  Ani->Eye.Dir.Y,  Ani->Eye.Dir.Z ),
                                  MatrRotateVec( Ani->GlobalDeltaTime * Ani->JsY * 30, Ani->Eye.Right.X,  Ani->Eye.Right.Y,  Ani->Eye.Right.Z ) ), 
                     MatrRotateVec( Ani->GlobalDeltaTime * Ani->JsX * 30, Ani->Eye.Up.X,  Ani->Eye.Up.Y,  Ani->Eye.Up.Z ) );
    M = VecSet( 0, 0, 0 );
    if ( VecLen2( Ani->Eye.Loc ) >= 0.03 && (Ani->JsPOV == 1 || Ani->JsPOV == 2 || Ani->JsPOV == 8) )
      M = VecAddVec( M, VecMulNum( VecNormalize( Ani->Eye.Loc ), -Ani->GlobalDeltaTime * 3 ) );
    if (Ani->JsPOV == 4 || Ani->JsPOV == 5 || Ani->JsPOV == 6)
      M = VecAddVec( M, VecMulNum( VecNormalize( Ani->Eye.Loc ),  Ani->GlobalDeltaTime * 3 ) );
    Ani->Eye.Dir = VectorTransformer( Ani->Eye.Dir, R );
    Ani->Eye.Right = VectorTransformer( Ani->Eye.Right, R );
    Ani->Eye.Up = VectorTransformer( Ani->Eye.Up, R );
    Ani->Eye.Loc = PointTransformer( VecAddVec( Ani->Eye.Loc, M ), R );
  }
} 

/* CAMERA unit
 * render function
 * base unit render arguments */
static VOID CAMERAUnitRender( in1UNIT_CAMERA *Unit, in1ANIM *Ani )
{
}

/* CAMERA unit
 * creation function */
in1UNIT * IN1_CAMERAUnitCreate( VOID )
{
  /* CAMERA unit cpeation pointer */
  in1UNIT_CAMERA *Unit;

  /* getting memory | defolte unit model for CAMERA unit */
  if ((Unit = (in1UNIT_CAMERA *)IN1_AnimUnitCreate(sizeof(in1UNIT_CAMERA))) == NULL)
  /* no awailabe memory | CAMERA unit model ERROR */
    return NULL;
  /* CAMERA unit buse function initialization */
  Unit->Init = (VOID *)CAMERAUnitInit;
  Unit->Close = (VOID *)CAMERAUnitClose;
  Unit->Response = (VOID *)CAMERAUnitResponse;
  Unit->Render = (VOID *)CAMERAUnitRender;
  /* CAMERA unit data initialization */
  Unit->MODE = 1;
  /* ready CAMERA unit pointer return */
  return (in1UNIT *)Unit;
} 
