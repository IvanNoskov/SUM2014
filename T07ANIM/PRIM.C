/* FILENAME: PRIM.C
 * PROGRAMMER: IN1
 * PURPOSE: Primtive handle functions.
 * LAST UPDATE: 16.06.2014
 */

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "anim.h"
#include "shader.h"

/* ������� */
#define power(A, B) ((A) < 0 ? -pow(-A, B) : pow(A, B))

/* ���� ��-��������� */
COLOR IN1_DefaultColor = {1, 1, 1, 1};

/* ������� �������� ���������.
 * ���������:
 *   - ����������� ��������:
 *       in1PRIM *P;
 *   - ��� ���������:
 *       in1PRIM_TYPE Type;
 *   - ��������� ����� (������, ������) ���
 *     Type == IN1_PRIM_GRID ��� ���������� ������
 *     � ��������:
 *       INT NumOfV_GW, NumOfI_GH;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL IN1_PrimCreate( in1PRIM *P, in1PRIM_TYPE Type,
       INT NumOfV_GW, INT NumOfI_GH )
{
  INT i, j, nv, ni, size;

  memset(P, 0, sizeof(in1PRIM));
  /* ����������� ��������� */
  switch (Type)
  {
  case IN1_PRIM_TRIMESH:
    nv = NumOfV_GW;
    ni = NumOfI_GH;
    break;
  case IN1_PRIM_GRID:
    nv = NumOfV_GW * NumOfI_GH;
    ni = NumOfV_GW * 2;
    break;
  }
  /* �������� ������ */
  size = sizeof(in1VERTEX) * nv + sizeof(INT) * ni;
  P->V = malloc(size);
  if (P->V == NULL)
    return FALSE;
  memset(P->V, 0, size);
  
  /* ��������� �������� */
  P->Size = size;
  P->Type = Type;
  P->NumOfV = nv;
  P->NumOfI = ni;
  P->GW = NumOfV_GW;
  P->GH = NumOfI_GH;
  P->I = (INT *)(P->V + nv);
  /* ��������� ��� ������� ��-��������� */
  for (i = 0; i < nv; i++)
  {
    P->V[i].C = IN1_DefaultColor;
    P->V[i].N = ShaderVecTransfrov(VecSet(0, 1, 0));
  }

  /* ��������� ������� � �������� ��� ���. ����� */
  if (Type == IN1_PRIM_GRID)
  {
    for (i = 0; i < NumOfI_GH; i++)
      for (j = 0; j < NumOfV_GW; j++)
        P->V[i * NumOfV_GW + j].T =
          IN1_UVSet(j / (NumOfV_GW - 1.0),
                    i / (NumOfI_GH - 1.0));
    for (i = 0; i < NumOfV_GW; i++)
    {
      P->I[2 * i + 0] = NumOfV_GW + i;
      P->I[2 * i + 1] = i;
    }
  }
  return TRUE;
} /* End of 'IN1_PrimCreate' function */

/* ������� �������� �����.
 * ���������:
 *   - ����������� ��������:
 *       in1PRIM *P;
 *   - ����� �����:
 *       VEC C;
 *   - ������ �����:
 *       FLT R;
 *   - ��������� ����� (������, ������):
 *       INT M, N;
 * ������������ ��������:
 *   (BOOL) TRUE ��� ������.
 */
BOOL IN1_PrimCreateSphere( in1PRIM *P, VEC C, FLT R, INT M, INT N )
{
  INT i, j;
  DBL t = clock() / 1000.0, t1;

  t1 = 2 + 2 * cos(t);
  t = 2 + 2 * sin(t * 1.01 + 2);

  if (!IN1_PrimCreate(P, IN1_PRIM_GRID, M, N))
    return FALSE;
  for (i = 0; i < N; i++)
    for (j = 0; j < M; j++)
    {
      in1VERTEX *V = P->V + i * M + j;
      DBL
        theta = i / (N - 1.0) * PI,
        phi = j / (M - 1.0) * 2 * PI;

      V->N = ShaderVecTransfrov(VecSet(power(sin(theta), t1) * power(sin(phi), t),
                                power(cos(theta), t1),
                                power(sin(theta), t1) * power(cos(phi), t)));
      V->P = ShaderVecTransfrov(VecAddVec(VecMulNum(VecRedTransfrov(V->N), R), C));
    }
  return TRUE;
} /* End of 'IN1_PrimCreateSphere' function */

/* ������� �������� ���������.
 * ���������:
 *   - ��������� ��������:
 *       in1PRIM *P;
 * ������������ ��������: ���.
 */
VOID IN1_PrimFree( in1PRIM *P )
{
  if (P->V != NULL)
    free(P->V);
  memset(P, 0, sizeof(in1PRIM));
} /* End of 'IN1_PrimFree' function */

/* ������� ��������� ���������.
 * ���������:
 *   - ��������:
 *       in1PRIM *P;
 * ������������ ��������: ���.
 */
VOID IN1_PrimDraw( in1PRIM *P )
{
  INT i;

  if (P->V == NULL)
    return;

  if (P->Buffers[0] == 0)
  {
    /* ������� � ��������� ������ ������ */
    glGenBuffers(2, P->Buffers);
    glGenVertexArrays(1, &P->VertexBuffer);

    /* ������� */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(in1VERTEX) * P->NumOfV,
      P->V, GL_STATIC_DRAW);

    /* ������� */
    glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(INT) * P->NumOfI,
      P->I, GL_STATIC_DRAW);
  }

  /* ���������� ������ */
  glBindVertexArray(P->VertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, P->Buffers[0]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, P->Buffers[1]);

  /* ������ ������� ������ */
  glVertexAttribPointer(0, 3, GL_FLOAT, FALSE, sizeof(in1VERTEX), (VOID *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, FALSE, sizeof(in1VERTEX), (VOID *)(sizeof(VECf)));
  glVertexAttribPointer(2, 3, GL_FLOAT, FALSE, sizeof(in1VERTEX), (VOID *)(sizeof(VECf) + sizeof(in1UV)));
  glVertexAttribPointer(3, 4, GL_FLOAT, FALSE, sizeof(in1VERTEX), (VOID *)(2 * sizeof(VECf) + sizeof(in1UV)));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);
  glEnableVertexAttribArray(3);

  glUseProgram(IN1_ShaderProg);
  if (P->Type == IN1_PRIM_TRIMESH)
  {
    /*
    glBegin(GL_TRIANGLES);
    for (i = 0; i < P->NumOfI; i++)
      glVertex3fv(&P->V[P->I[i]].P.X);
    glEnd();
    */
    glDrawElements(GL_TRIANGLES, P->NumOfI, GL_UNSIGNED_INT, (VOID *)0);
  }
  else
  {
    for (i = 0; i < P->GH - 1; i++)
    {
      /*
      INT start = i * P->GW, j;

      glBegin(GL_TRIANGLE_STRIP);
      for (j = 0; j < P->NumOfI; j++)
        glVertex3fv(&P->V[start + P->I[j]].P.X);
      glEnd();
      */
      glDrawElementsBaseVertex(GL_TRIANGLE_STRIP,
        P->NumOfI, GL_UNSIGNED_INT, (VOID *)0,
        i * P->GW);
    }
  }
  glUseProgram(0);
} 

VOID IN1_PrimAutoNormals( in1PRIM *P )
{
  INT i;
  VECf Zer = ShaderVecTransfrov( VecSet( 0, 0, 0 ) );

  if (P->Type == IN1_PRIM_TRIMESH)
  {
    /* �������� ��� ������� ������ */
    for (i = 0; i < P->NumOfV; i++)
      P->V[i].N = Zer;

    /* ��������� ������� ���� ������ � �������� �� � ���������� �� �������� */
    for (i = 0; i < P->NumOfI / 3; i++)
    {
      INT *n = P->I + i * 3;
      VEC norm;

      norm =
        VecNormalize( VecCrossVec( VecSubVec( VecRedTransfrov( P->V[n[1]].P ), VecRedTransfrov( P->V[n[0]].P) ),
                                                       VecSubVec( VecRedTransfrov( P->V[n[2]].P ), VecRedTransfrov( P->V[n[0]].P) ) ) );
      P->V[n[0]].N = ShaderVecTransfrov( VecAddVec( VecRedTransfrov( P->V[n[0]].N ), norm ) );
      P->V[n[1]].N = ShaderVecTransfrov( VecAddVec( VecRedTransfrov( P->V[n[1]].N ), norm ) );
      P->V[n[2]].N = ShaderVecTransfrov( VecAddVec( VecRedTransfrov( P->V[n[2]].N ), norm ) );
    }

    /* ��������� */
    for (i = 0; i < P->NumOfV; i++)
      P->V[i].N = ShaderVecTransfrov( VecNormalize( VecRedTransfrov( P->V[i].N ) ) );
  }
  else
  {
  }
}
