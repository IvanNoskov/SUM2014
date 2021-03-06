#ifndef __RENDER_H__
#define __RENDER_H__

#include "VCT.H"

#define MAX_STR 300

typedef struct tagin1CAMERA
{
  VEC Loc, Dir, Up, Right;
} in1CAMERA;

typedef struct tagin1GOBJ
{
  VEC *V;      /* ������ ������ ������� */
  INT NumOfV;  /* ���������� ������ */
  INT (*F)[3]; /* ������ ������ ������� */
  INT NumOfF;  /* ���������� ������ */
} in1GOBJ;

typedef struct tagin1UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} in1UV;

typedef struct tagCOLOR
{
  FLT R, G, B, A; /* ���������� ���������� ��������� */
} COLOR;

/* ��� ������������� ������� */
typedef struct tagin1VERTEX
{
  VECf P;    /* ���������� */
  in1UV T;  /* ���������� ���������� */
  VECf N;    /* ������� */
  COLOR C;  /* ���� */
} in1VERTEX;

/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagin1PRIM_TYPE
{
  IN1_PRIM_TRIMESH, IN1_PRIM_GRID
} in1PRIM_TYPE;
/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagin1MATERIAL
{
  VECf Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
  UINT ShaderID;
} in1MATERIAL;

/* ��� �������� ��������� */
typedef struct tagin1PRIM
{
  in1PRIM_TYPE Type; /* ��� ��������� */
  INT
    NumOfV,     /* ���������� ������ */
    NumOfI,     /* ���������� �������� */
    GW, GH,     /* ������ ����� */
    Mtl,        /* ����� ��������� */
    Size;       /* ������ ���������� ������ � ������ */
  in1VERTEX *V; /* ������ ������ */
  INT *I;       /* ������ �������� */

  /* ������ ��� ������ � OpenGL */
  INT
    Buffers[2],    /* ����� ������ � �������� */
    VertexBuffer;  /* ����� �������� ������ */
} in1PRIM;

/* �������������� ������ - ����� ���������� */
typedef struct tagin1GEOM
{
  INT NumOfPrims, NumOfMtls; /* ������� �������� */
  in1MATERIAL *Mtls;         /* ������� ���������� */
  in1PRIM *Prims;            /* ������ ���������� */
} in1GEOM;

/* ������� ������������� ���������� ��������� */
__inline in1UV IN1_UVSet( FLT U, FLT V )
{
  in1UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'IN1_UVSet' function */

__inline COLOR IN1_ColorSet( FLT R, FLT B, FLT G )
{
  COLOR C;

  C.A = 1;
  C.B = B;
  C.G = G;
  C.R = R;

  return C;
}

__inline COLOR IN1_ColorSetA( FLT R, FLT B, FLT G, FLT A )
{
  COLOR C;

  C.A = A;
  C.B = B;
  C.G = G;
  C.R = R;

  return C;
}

extern COLOR IN1_DefaultColor;

/* ������� ���������� ��������� � ������� */
INT IN1_GeomAddMaterial( in1GEOM *G,
                         in1MATERIAL *Mtl );

/* ������� ���������� ��������� � ������� */
INT IN1_GeomAddPrim( in1GEOM *G, in1PRIM *Prim );

/* ������� ������������ ��������������� ������� */
VOID IN1_GeomFree( in1GEOM *G );

/* ������� ��������� */
VOID IN1_GeomDraw( in1GEOM *G );

/* ������� �������� ��������������� ������� */
BOOL IN1_GeomLoad( in1GEOM *G, CHAR *FileName );

/* ������� �������� ��������� */
BOOL IN1_PrimCreate( in1PRIM *P, in1PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH );

/* ������� �������� ����� */
BOOL IN1_PrimCreateSphere( in1PRIM *P,
       VEC C, FLT R, INT M, INT N );

/* ������� �������� ��������� */
VOID IN1_PrimFree( in1PRIM *P );

/* ������� ��������� ��������� */
VOID IN1_PrimDraw( in1PRIM *P );

VOID IN1_PrimAutoNormals( in1PRIM *P );



#endif /* __RENDER_H__ */
