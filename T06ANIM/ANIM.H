/* NI01, ANIM.H */


#ifndef __ANIM_H__
#define __ANIM_H__

#include "image.h"

/* MAX avalable animation units number */
#define IN1_MAX_UNITS 3000

/* UNIT tipe preliminary determination */
typedef struct tagin1UNIT in1UNIT;

/* main Animation sistem data structure */
typedef struct tagin1ANIM
{
  /*-- Window district --*/
  HWND hWnd;          /* output Window Handle */
  INT W, H;           /* output window Size */
  HBITMAP hBmFrame;   /* Frame Buffer */
  HDC hDC;            /* output Handle */

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
} in1ANIM;

/* defolte unit data initialization  */
#define IN1_UNIT_BASE_FIELDS \
  INT Size;                                        /* unit Data Tipe Size */    \
  VOID (*Init)( in1UNIT *Unit, in1ANIM *Ani );     /* pointer to Initialization function */    \
  VOID (*Close)( in1UNIT *Unit, in1ANIM *Ani );    /* pointer to Deinitialization function */    \
  VOID (*Response)( in1UNIT *Unit, in1ANIM *Ani ); /* pointer to Response function */    \
  VOID (*Render)( in1UNIT *Unit, in1ANIM *Ani )    /* pointer to Render function */

/* defolte( void ) animation unit data tipe */
struct tagin1UNIT
{
  IN1_UNIT_BASE_FIELDS; /* ������� ���� */
};

/*--- Animation control Functions ---*/

/* Animation Initialization function */
VOID IN1_AnimInit( HWND hWnd );

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

#endif /* __ANIM_H__ */
