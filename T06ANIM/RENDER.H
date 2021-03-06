#ifndef __RENDER_H__
#define __RENDER_H__

#include "VCT.H"

typedef struct tagin1CAMERA
{
  VEC Loc, Dir, Up, Right;
} in1CAMERA;

typedef struct tagvg4GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} vg4GOBJ;

void LookAt (VEC Loc, VEC At, VEC UpApprox);
void SetProj (DBL Wp, DBL Hp, DBL Pp, DBL Pf);
void CameraRefresh ( DBL DTime, DBL DirRotation, DBL UpRotation, DBL RightRotation, INT MOVE_V );
POINT IN1_WorldToScreen( VEC Point);
void ScreenOpt ( INT Hs, INT Ws);

/* ������� �������� ��������������� ������� */
BOOL VG4_RndGObjLoad( vg4GOBJ *GObj, CHAR *FileName );

/* ������� ������������ ��������������� ������� */
VOID VG4_RndGObjFree( vg4GOBJ *GObj );

/* ������� ��������� ��������������� ������� */
VOID VG4_RndGObjDraw( vg4GOBJ *GObj, HDC hDC );


#endif /* __RENDER_H__ */
