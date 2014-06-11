#include "RENDER.H"
#include "VCT.H"
#include "stdio.h"

in1CAMERA IN1_CAM;
static MATRIX MWorld, MViewer, MProjetion, MWorldToScreen;
DOUBLE Wproection, Hproection, Proj_plane,  Proj_Far;
INT Wscreen, Hscreen;

void LookAt (VEC Loc, VEC At, VEC UpApprox)
{
  IN1_CAM.Loc = Loc;
  IN1_CAM.Dir = VecNormalize( VecSubVec( At, Loc ) );
  IN1_CAM.Right = VecNormalize( VecCrossVec( IN1_CAM.Dir, UpApprox ) );
  IN1_CAM.Up = VecCrossVec( IN1_CAM.Right, IN1_CAM.Dir );
}

void SetProj (DBL Wp, DBL Hp, DBL Pp, DBL Pf)
{
  Wproection = Wp;
  Hproection = Hp;
  Proj_plane = Pp;
  Proj_Far = Pp;
  MProjetion = MatrProjection( -Wp / 2, Wp / 2, -Hp / 2, Hp / 2, Pp, Pf );
}

void ScreenOpt ( INT Hs, INT Ws)
{
  Wscreen = Ws;
  Hscreen = Hs;
}

void CameraRefresh ( DBL DTime, DBL DirRotation, DBL UpRotation, DBL RightRotation, INT MOVE_V )
{
  MATRIX R;
  VEC M;
  R = MatrMulMatr( MatrMulMatr( MatrRotateVec( DTime * DirRotation * 30, IN1_CAM.Dir.X,  IN1_CAM.Dir.Y,  IN1_CAM.Dir.Z ), MatrRotateVec( DTime * UpRotation * 30, IN1_CAM.Right.X,  IN1_CAM.Right.Y,  IN1_CAM.Right.Z ) ), MatrRotateVec( DTime * RightRotation * 30, IN1_CAM.Up.X,  IN1_CAM.Up.Y,  IN1_CAM.Up.Z ) );
  M = VecSet( 0, 0, 0 );
  if (MOVE_V == 1 || MOVE_V == 2 || MOVE_V == 8)
    M = VecAddVec( M, VecMulNum ( IN1_CAM.Dir, DTime ) );
  if (MOVE_V == 4 || MOVE_V == 5 || MOVE_V == 6)
    M = VecAddVec( M, VecMulNum ( IN1_CAM.Dir, -DTime ) );
  if (MOVE_V == 2 || MOVE_V == 3 || MOVE_V == 4)
    M = VecAddVec( M, VecMulNum ( IN1_CAM.Right, DTime ) );  
  if (MOVE_V == 6 || MOVE_V == 7 || MOVE_V == 8)
    M = VecAddVec( M, VecMulNum ( IN1_CAM.Right, -DTime ) );  
  IN1_CAM.Dir = VectorTransformer( IN1_CAM.Dir, R);
  IN1_CAM.Right = VectorTransformer( IN1_CAM.Right, R);
  IN1_CAM.Up = VectorTransformer( IN1_CAM.Up, R);
  IN1_CAM.Loc = VecAddVec( IN1_CAM.Loc, M );

  MViewer = MatrLookAt ( IN1_CAM.Loc, VecAddVec( IN1_CAM.Loc, IN1_CAM.Dir), IN1_CAM.Up );
  MWorld = MatrIdenity();
  /*MViewertoScreen.A[2][2] = 0;    */
  MWorldToScreen = MatrMulMatr( MatrMulMatr( MWorld, MViewer ), MProjetion );
}

POINT IN1_WorldToScreen( VEC Point )
{
  VEC T = PointTransformer( Point, MWorldToScreen );
  POINT A;
  A.x = ( T.X + 1 / 2.0) * (Wscreen - 1);
  A.y = (-T.Y + 1 / 2.0) * (Hscreen - 1);
  return A;
}

/* Функция загрузки геометрического объекта.
 * АРГУМЕНТЫ:
 *   - указатель на структуру для загружаемой геометрии:
 *       vg4GOBJ *GObj;
 *   - имя файла (*.OBJ):
 *       CHAR *FileName;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (BOOL) TRUE при успехе.
 */
BOOL VG4_RndGObjLoad( vg4GOBJ *GObj, CHAR *FileName )
{
  INT nv = 0, nf = 0;
  FILE *F;
  static CHAR Buf[1000];

  memset(GObj, 0, sizeof(vg4GOBJ));
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

VOID VG4_RndGObjFree( vg4GOBJ *GObj )
{
  if (GObj->V != NULL)
    free(GObj->V);
  memset(GObj, 0, sizeof(vg4GOBJ));
} 

VOID VG4_RndGObjDraw( vg4GOBJ *GObj, HDC hDC )
{
  INT i, j, n[3];
  POINT *pts;

  if ((pts = malloc( sizeof(POINT) * GObj->NumOfV )) == NULL)
    return;

  for (i = 0; i < GObj->NumOfV; i++)
  {
    pts[i] = IN1_WorldToScreen(GObj->V[i]);
  }
  for (i = 0; i < GObj->NumOfF; i++)
  {
    for (j = 0; j < 3; j++)
      n[j] = GObj->F[i][j];
    {
      MoveToEx(hDC, pts[n[0]].x, pts[n[0]].y, NULL);
      LineTo(hDC, pts[n[1]].x, pts[n[1]].y);
      LineTo(hDC, pts[n[2]].x, pts[n[2]].y);
      LineTo(hDC, pts[n[0]].x, pts[n[0]].y);
    }
  }
  free( pts );
}