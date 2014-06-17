/* NI01, ANIM.H */


#ifndef __ANIM_H__
#define __ANIM_H__

#include "image.h"
#include "render.h"

/* MAX avalable animation units number */
#define IN1_MAX_UNITS 3000

/* Units Tipe_ID */
typedef enum tagin1UNIT_TYPE
{
  IN1_NOT_SPECIFIED_UNIT = 0, IN1_UNIT_INFO, IN1_UNIT_WARDEN, IN1_UNIT_GOST, IN1_UNIT_EYE, IN1_UNIT_KEY
} in1UNIT_TYPE;

/* UNIT tipe preliminary determination */
typedef struct tagin1UNIT in1UNIT;

/* main Animation sistem data structure */
typedef struct tagin1ANIM
{
  /*-- Window district --*/
  HWND hWnd;          /* output Window Handle */
  INT W, H;           /* output window Size */
  HDC hDC;            /* Wndow Handle */
  HGLRC hRC;          /* Rendering Handle */

  /*-- Animation units district --*/
  in1UNIT *Units[IN1_MAX_UNITS]; /* animation Unts Massive */
  INT NumOfUnits;                /* Num of of the existing animation Units*/

  /*-- Time | synchronization district --*/
  DBL
    Time,            /* Time from animation Start whis Pause including in Second */
    GlobalTime,      /* Time from animation Start whisout Pause including in Second */
    DeltaTime,       /* Time from Previos frame whis Pause including in Second */
    GlobalDeltaTime, /* Time from Previos frame whisout Pause including in Second */
    FPS;             /* Frames Per Second */
  BOOL
    IsPause;         /* Pause flag */

  /*-- Input district --*/
  /*- keyboard -*/
  BYTE Keys[256];          /* Keys | Mouse state */
  BYTE KeysOld[256];       /* Keys | Mouse state */  
  BYTE KeysClick[256];     /* Keys | Mouse state */  
  /*- mouse -*/
  DBL
    MsWeel,
    MsDeltaX,
    MsDeltaY,
    MsAbsX,
    MsAbsY;
  /*- joustic -*/
  DBL
    JsX,
    JsY,
    JsZ,
    JsR,
    JsU;
  BYTE
    JsBut[32],
    JsButOld[32],
    JsButClick[32],
    JsPOV;
  /*-- Projection creation --*/
  DBL
    PrjH,
    PrjW,
    PrjDist,
    PrjFar,
    PrjSize;
  MATRIXd
    PrjMWorld,
    PrjMView,
    PrjMProjection,
    PrjM_WV,
    PrjM_VP,
    PrjM_WVP;
  in1CAMERA
    Eye;
} in1ANIM;

/* defolte unit data initialization  */
#define IN1_UNIT_BASE_FIELDS \
  INT Size,                                        /* unit Data Tipe Size */                     \
      ID;                                          /* curent Unit Identificator */               \
  in1UNIT_TYPE TipeID;                             /* curent unit Tipe Identificator*/           \
  VOID (*Init)( in1UNIT *Unit, in1ANIM *Ani );     /* pointer to Initialization function */      \
  VOID (*Close)( in1UNIT *Unit, in1ANIM *Ani );    /* pointer to Deinitialization function */    \
  VOID (*Response)( in1UNIT *Unit, in1ANIM *Ani ); /* pointer to Response function */            \
  VOID (*Render)( in1UNIT *Unit, in1ANIM *Ani );   /* pointer to Render function */              \
  VOID (*Collision)( in1UNIT *Unit, in1ANIM *Ani ) /* pointer to Collision function */          

/* defolte( void ) animation unit data tipe */
struct tagin1UNIT
{
  IN1_UNIT_BASE_FIELDS; /* Базовые поля */
};

/*--- Animation control Functions ---*/

VOID IN1_AnimPrjMResponse();

/* Animation Initialization function */
BOOL IN1_AnimInit( HWND hWnd );

/* Animation Deinitialization function  */
VOID IN1_AnimClose( VOID );

/* Animation Resize processing function */
VOID IN1_AnimResize( INT W, INT H );

/* Animation frame Rendering function */
VOID IN1_AnimRender( VOID );

/* Animation frame Outpute function */
VOID IN1_AnimCopyFrame( VOID );

/* Animation MultiFulscrin processing function */
VOID IN1_AnimFlipFullScreen( VOID );

/* Animation Pause function */
VOID IN1_AnimSetPause( BOOL NewPauseFlag );

/*--- Units Animate Functions ---*/

/* Adding Unit to Sistem function */
VOID IN1_AnimAddUnit( in1UNIT *Unit );

/* Creating Unit function */
in1UNIT * IN1_AnimUnitCreate( INT Size );

static LRESULT CALLBACK IN1_MouseHook( INT Code, WPARAM wParam, LPARAM lParam );

extern in1ANIM IN1_Anim;

#endif /* __ANIM_H__ */
