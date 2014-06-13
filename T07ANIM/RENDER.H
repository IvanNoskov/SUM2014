#ifndef __RENDER_H__
#define __RENDER_H__

#include "VCT.H"

typedef struct tagin1CAMERA
{
  VEC Loc, Dir, Up, Right;
} in1CAMERA;

typedef struct tagin1GOBJ
{
  VEC *V;      /* массив вершин объекта */
  INT NumOfV;  /* количество вершин */
  INT (*F)[3]; /* массив граней объекта */
  INT NumOfF;  /* количество граней */
} in1GOBJ;


/* Функция загрузки геометрического объекта */
BOOL IN1_RndGObjLoad( in1GOBJ *GObj, CHAR *FileName );

/* Функция освобождения геометрического объекта */
VOID IN1_RndGObjFree( in1GOBJ *GObj );

/* Функция отрисовки геометрического объекта */
VOID IN1_RndGObjDraw( in1GOBJ *GObj, HDC hDC, MATRIX TransformM );


#endif /* __RENDER_H__ */
