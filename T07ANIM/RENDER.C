#include "RENDER.H"
#include "DEF.H"
#include "stdio.h"

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       in1GOBJ *GObj;
 *   - имя файла (*.OBJ):
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL IN1_RndGObjLoad( in1GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  memset(GObj, 0, sizeof(in1GOBJ));
  if ((F = fopen(FileName, "r")) == NULL)
    return 0;

  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    if (Buf[0] == 'v' && Buf[1] == ' ')
      nv++;
    else if (Buf[0] == 'f' && Buf[1] == ' ')
      nf++;
  }

  GObj->V = malloc(nv * sizeof(VEC) + nf * sizeof(INT [3]));
  GObj->F = (INT (*)[3])(GObj->V + nv);
  GObj->NumOfV = nv;
  GObj->NumOfF = nf;

  nv = 0;
  nf = 0;
  rewind(F);
  while (fgets(Buf, sizeof(Buf), F) != NULL)
  {
    DBL x, y, z;
    INT a, b, c;

    if (Buf[0] == 'v' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%lf%lf%lf", &x, &y, &z);
      GObj->V[nv++] = VecSet(x, y, z);
    }
    else if (Buf[0] == 'f' && Buf[1] == ' ')
    {
      sscanf(Buf + 2, "%i/%*i/%*i %i/%*i/%*i %i/%*i/%*i", &a, &b, &c) == 3 || 
      sscanf(Buf + 2, "%i//%*i %i//%*i %i//%*i", &a, &b, &c) == 3 || 
      sscanf(Buf + 2, "%i/%*i %i/%*i %i/%*i", &a, &b, &c) == 3 || 
      sscanf(Buf + 2, "%i %i %i", &a, &b, &c);
      GObj->F[nf][0] = a - 1;
      GObj->F[nf][1] = b - 1;
      GObj->F[nf][2] = c - 1;
      nf++;
    }
  }
  fclose(F);
  return TRUE;
} 

VOID IN1_RndGObjFree( in1GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(in1GOBJ));
} 

VOID IN1_RndGObjDraw( in1GOBJ *GObj, HDC hDC, MATRIXd TransformM )
{
  INT i, j, n[3];
  POINT *pts;

  if ((pts = malloc( sizeof(POINT) * GObj->NumOfV )) == NULL)
    return;

  glLoadMatrixd( &(TransformM.A[0][0]) );

  glPolygonMode( GL_BACK, GL_LINE );
  glPolygonMode( GL_FRONT, GL_FILL );
  glBegin( GL_TRIANGLES );
  for (i = 0; i < GObj->NumOfF; i++)
  {
    for (j = 0; j < 3; j++)
      n[j] = GObj->F[i][j];
    {
      glVertex3dv( &(GObj->V[n[0]].X) );
      glVertex3dv( &(GObj->V[n[1]].X) );
      glVertex3dv( &(GObj->V[n[2]].X) );
    }
  }
  glEnd( );
  free( pts );
}
