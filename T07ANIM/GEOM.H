
/***
 * ������ � ��������������� ���������
 ***/

/* ��� ������������� ���������� ��������� */
typedef struct tagin1UV
{
  FLT U, V; /* ���������� ���������� ��������� */
} in1UV;

/* ������� ������������� ���������� ��������� */
__inline in1UV IN1_UVSet( FLT U, FLT V )
{
  in1UV uv;

  uv.U = U;
  uv.V = V;
  return uv;
} /* End of 'IN1_UVSet' function */

/* ��� ������������� ������� */
typedef struct tagin1VERTEX
{
  VEC P;    /* ���������� */
  in1UV T;  /* ���������� ���������� */
  VEC N;    /* ������� */
  COLOR C;  /* ���� */
} in1VERTEX;


/* ��� ��������� - ����� ������������� ��� ���������� ����� ������ */
typedef enum tagin1PRIM_TYPE
{
  IN1_PRIM_TRIMESH, IN1_PRIM_GRID
} in1PRIM_TYPE;

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

/* ���� ��-��������� */
extern COLOR IN1_DefaultColor;

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

/* ������������ ����� ����� */
#define MAX_STR 300

/* ��� ������������� ��������� */
typedef struct tagin1MATERIAL
{
  VEC Ka, Kd, Ks;  /* ������������ ��������� */
  FLT Phong;       /* ����������� ����� */
  FLT Trans;       /* ����������� ������������ */
  CHAR
    Name[MAX_STR], /* ��� ��������� */
    MapD[MAX_STR]; /* ��� �������� */
  INT TexNo;       /* ������������� �������� � OpenGL */
} in1MATERIAL;

/* �������������� ������ - ����� ���������� */
typedef struct tagin1GEOM
{
  INT NumOfPrims, NumOfMtls; /* ������� �������� */
  in1MATERIAL *Mtls;         /* ������� ���������� */
  in1PRIM *Prims;            /* ������ ���������� */
} in1GEOM;

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

