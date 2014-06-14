/* NI01 */

#ifndef __VCT_H__
#define __VCT_H__

#include <math.h>

#include <windows.h>

/* Common float point types */
typedef float FLT;
typedef double DBL;

#define PI ((double)(3.1415926535897))
#define Deg2Rad 0.01745329251994329576L
#define toleranse 0.000002

typedef struct tagVEC{
  DOUBLE X, Y, Z;
} VEC;

typedef struct tagMATRIXd
{
  DBL A[4][4];
} MATRIXd;

typedef struct tagMATRIXf
{
  FLT A[4][4];
} MATRIXf;

extern MATRIXd UnitMatrix;
extern long double MultiplierDegree2Radian;
extern INT iscl[4][3];

__inline VEC VecSet ( DOUBLE X, DOUBLE Y, DOUBLE Z )
{
  VEC A = {X, Y, Z};
  return A;
}

__inline VEC VecAddVec ( VEC A, VEC B )
{
  return VecSet( A.X + B.X, A.Y + B.Y, A.Z + B.Z );
}

__inline VEC VecNeg ( VEC A )
{
  return VecSet( - A.X, - A.Y, - A.Z );
}

__inline VEC VecSubVec ( VEC A, VEC B )
{
  return VecSet( A.X - B.X, A.Y - B.Y, A.Z - B.Z );
}

__inline VEC VecMulNum ( VEC A, DOUBLE L )
{
  return VecSet( A.X * L, A.Y * L, A.Z * L );
}

__inline VEC VecDivNum ( VEC A, DOUBLE L )
{
  return VecSet( A.X / L, A.Y / L, A.Z / L );
}

__inline DOUBLE VecDotVec ( VEC A, VEC B )
{
  return ( A.X * B.X + A.Y * B.Y + A.Z * B.Z );
}

__inline VEC VecCrossVec ( VEC A, VEC B )
{
  return VecSet( A.Y * B.Z - A.Z * B.Y,
                 A.Z * B.X - A.X * B.Z,
                 A.X * B.Y - A.Y * B.X);
}

__inline DOUBLE VecLen2 ( VEC A )
{
  return ( A.X * A.X + A.Y * A.Y + A.Z * A.Z );
}

__inline DOUBLE VecLen ( VEC A )
{
  DOUBLE T = ( A.X * A.X + A.Y * A.Y + A.Z * A.Z );
  if (T <= toleranse || ((T - 1) <= toleranse && (1 - T) <= toleranse))
    return T;
  return sqrt( T );
}

__inline VEC VecNormalize ( VEC A )
{
  return VecDivNum( A, VecLen( A ) );
}

__inline MATRIXd MatrSet (DOUBLE A11, DOUBLE A12, DOUBLE A13, DOUBLE A14,
                         DOUBLE A21, DOUBLE A22, DOUBLE A23, DOUBLE A24,
                         DOUBLE A31, DOUBLE A32, DOUBLE A33, DOUBLE A34,
                         DOUBLE A41, DOUBLE A42, DOUBLE A43, DOUBLE A44)
{
  MATRIXd M;
  M.A[0][0] = A11;
  M.A[0][1] = A12;
  M.A[0][2] = A13;
  M.A[0][3] = A14;
  M.A[1][0] = A21;
  M.A[1][1] = A22;
  M.A[1][2] = A23;
  M.A[1][3] = A24;
  M.A[2][0] = A31;
  M.A[2][1] = A32;
  M.A[2][2] = A33;
  M.A[2][3] = A34;
  M.A[3][0] = A41;
  M.A[3][1] = A42;
  M.A[3][2] = A43;
  M.A[3][3] = A44;
  return M;
}

__inline MATRIXd MatrIdenity()
{
  return UnitMatrix;
}

__inline MATRIXd MatrScale( DOUBLE sX, DOUBLE sY, DOUBLE sZ )
{
  MATRIXd M = UnitMatrix;
  M.A[0][0] = sX;
  M.A[1][1] = sY;
  M.A[2][2] = sZ;
  return M;
}

__inline MATRIXd MatrTranslate( DOUBLE dX, DOUBLE dY, DOUBLE dZ )
{
  MATRIXd M = UnitMatrix;
  M.A[3][0] = dX;
  M.A[3][1] = dY;
  M.A[3][2] = dZ;
  return M;
}

__inline MATRIXd MatrRotateX( DOUBLE AngleInDeg )
{
  MATRIXd M = UnitMatrix;
  _asm {
    fld  AngleInDeg
    fmul MultiplierDegree2Radian
    fsincos
    fld st(0)
    fstp qword ptr  M.A + 40 /* 8 * ((1 * 4) + (1)) */
    fstp qword ptr  M.A + 80 /* 8 * ((2 * 4) + (2)) */
    fld  st(0)                   
    fstp qword ptr  M.A + 48 /* 8 * ((1 * 4) + (2)) */
    fldz       
    fsubr      
    fstp qword ptr  M.A + 72 /* 8 * ((2 * 4) + (1)) */
  }
  return M;
}

__inline MATRIXd MatrRotateY( DOUBLE AngleInDeg )
{
  MATRIXd M = UnitMatrix;
  _asm {
    fld  AngleInDeg
    fmul MultiplierDegree2Radian
    fsincos
    fld st(0)
    fstp qword ptr  M.A /* 8 * ((0 * 4) + (0)) */
    fstp qword ptr  M.A + 80 /* 8 * ((2 * 4) + (2)) */
    fld  st(0)                   
    fstp qword ptr  M.A + 64 /* 8 * ((2 * 4) + (0)) */
    fldz       
    fsubr      
    fstp qword ptr  M.A + 16 /* 8 * ((0 * 4) + (2)) */
  }
  return M;
}

__inline MATRIXd MatrRotateZ( DOUBLE AngleInDeg )
{
  MATRIXd M = UnitMatrix;
  _asm {
    fld  AngleInDeg
    fmul MultiplierDegree2Radian
    fsincos
    fld  st(0)
    fstp qword ptr  M.A /* 8 * ((0 * 4) + (0)) */
    fstp qword ptr  M.A + 40 /* 8 * ((1 * 4) + (1)) */
    fld  st(0) 
    fstp qword ptr  M.A + 8 /* 8 * ((0 * 4) + (1)) */
    fldz       
    fsubr      
    fstp qword ptr  M.A + 32 /* 8 * ((1 * 4) + (0)) */
  }
  return M;
}

__inline MATRIXd MatrRotateVec( DOUBLE AngleInDeg, DOUBLE X, DOUBLE Y, DOUBLE Z )
{
  DBL a, sinA, cosA, len;
  MATRIXd M;

  a = Deg2Rad * (AngleInDeg) / -2.0;
  __asm {
    fld a
    fsincos
    fstp cosA
    fstp sinA
  }
  len = X * X + Y * Y + Z * Z;
  if (len != 0 && len != 1)
    len = sqrt(len), X /= len, Y /= len, Z /= len;
  X *= sinA;
  Y *= sinA;
  Z *= sinA;
  M.A[0][0] = 1 - 2 * (Y * Y + Z * Z);
  M.A[0][1] = 2 * X * Y - 2 * cosA * Z;
  M.A[0][2] = 2 * cosA * Y + 2 * X * Z;
  M.A[0][3] = 0;
  M.A[1][0] = 2 * X * Y + 2 * cosA * Z;
  M.A[1][1] = 1 - 2 * (X * X + Z * Z);
  M.A[1][2] = 2 * Y * Z - 2 * cosA * X;
  M.A[1][3] = 0;
  M.A[2][0] = 2 * X * Z - 2 * cosA * Y;
  M.A[2][1] = 2 * cosA * X + 2 * Y * Z;
  M.A[2][2] = 1 - 2 * (X * X + Y * Y);
  M.A[2][3] = 0;
  M.A[3][0] = 0;
  M.A[3][1] = 0;
  M.A[3][2] = 0;
  M.A[3][3] = 1;
  return M;
}

__inline MATRIXd MatrMulMatr (MATRIXd M1, MATRIXd M2)
{
  MATRIXd L;
  L.A[0][0] = M1.A[0][0] * M2.A[0][0] + M1.A[0][1] * M2.A[1][0] + M1.A[0][2] * M2.A[2][0] + M1.A[0][3] * M2.A[3][0];
  L.A[0][1] = M1.A[0][0] * M2.A[0][1] + M1.A[0][1] * M2.A[1][1] + M1.A[0][2] * M2.A[2][1] + M1.A[0][3] * M2.A[3][1];
  L.A[0][2] = M1.A[0][0] * M2.A[0][2] + M1.A[0][1] * M2.A[1][2] + M1.A[0][2] * M2.A[2][2] + M1.A[0][3] * M2.A[3][2];
  L.A[0][3] = M1.A[0][0] * M2.A[0][3] + M1.A[0][1] * M2.A[1][3] + M1.A[0][2] * M2.A[2][3] + M1.A[0][3] * M2.A[3][3];
  L.A[1][0] = M1.A[1][0] * M2.A[0][0] + M1.A[1][1] * M2.A[1][0] + M1.A[1][2] * M2.A[2][0] + M1.A[1][3] * M2.A[3][0];
  L.A[1][1] = M1.A[1][0] * M2.A[0][1] + M1.A[1][1] * M2.A[1][1] + M1.A[1][2] * M2.A[2][1] + M1.A[1][3] * M2.A[3][1];
  L.A[1][2] = M1.A[1][0] * M2.A[0][2] + M1.A[1][1] * M2.A[1][2] + M1.A[1][2] * M2.A[2][2] + M1.A[1][3] * M2.A[3][2];
  L.A[1][3] = M1.A[1][0] * M2.A[0][3] + M1.A[1][1] * M2.A[1][3] + M1.A[1][2] * M2.A[2][3] + M1.A[1][3] * M2.A[3][3];
  L.A[2][0] = M1.A[2][0] * M2.A[0][0] + M1.A[2][1] * M2.A[1][0] + M1.A[2][2] * M2.A[2][0] + M1.A[2][3] * M2.A[3][0];
  L.A[2][1] = M1.A[2][0] * M2.A[0][1] + M1.A[2][1] * M2.A[1][1] + M1.A[2][2] * M2.A[2][1] + M1.A[2][3] * M2.A[3][1];
  L.A[2][2] = M1.A[2][0] * M2.A[0][2] + M1.A[2][1] * M2.A[1][2] + M1.A[2][2] * M2.A[2][2] + M1.A[2][3] * M2.A[3][2];
  L.A[2][3] = M1.A[2][0] * M2.A[0][3] + M1.A[2][1] * M2.A[1][3] + M1.A[2][2] * M2.A[2][3] + M1.A[2][3] * M2.A[3][3];
  L.A[3][0] = M1.A[3][0] * M2.A[0][0] + M1.A[3][1] * M2.A[1][0] + M1.A[3][2] * M2.A[2][0] + M1.A[3][3] * M2.A[3][0];
  L.A[3][1] = M1.A[3][0] * M2.A[0][1] + M1.A[3][1] * M2.A[1][1] + M1.A[3][2] * M2.A[2][1] + M1.A[3][3] * M2.A[3][1];
  L.A[3][2] = M1.A[3][0] * M2.A[0][2] + M1.A[3][1] * M2.A[1][2] + M1.A[3][2] * M2.A[2][2] + M1.A[3][3] * M2.A[3][2];
  L.A[3][3] = M1.A[3][0] * M2.A[0][3] + M1.A[3][1] * M2.A[1][3] + M1.A[3][2] * M2.A[2][3] + M1.A[3][3] * M2.A[3][3];
  return L;
}

__inline MATRIXd MatrTranspose (MATRIXd M)
{
  MATRIXd L;
  L.A[0][0] = M.A[0][0];
  L.A[0][1] = M.A[1][0];
  L.A[0][2] = M.A[2][0];
  L.A[0][3] = M.A[3][0];
  L.A[1][0] = M.A[0][1];
  L.A[1][1] = M.A[1][1];
  L.A[1][2] = M.A[2][1];
  L.A[1][3] = M.A[3][1];
  L.A[2][0] = M.A[0][2];
  L.A[2][1] = M.A[1][2];
  L.A[2][2] = M.A[2][2];
  L.A[2][3] = M.A[3][2];
  L.A[3][0] = M.A[0][3];
  L.A[3][1] = M.A[1][3];
  L.A[3][2] = M.A[2][3];
  L.A[3][3] = M.A[3][3];
  return L;
}

__inline DOUBLE MatrMathDop (MATRIXd M, INT I, INT J)
{
  return (((I + J) % 2) == 0 ? -1 : 1) * (M.A[iscl[I][0]][iscl[J][0]] * M.A[iscl[I][1]][iscl[J][1]] * M.A[iscl[I][2]][iscl[J][2]] + 
                                          M.A[iscl[I][0]][iscl[J][1]] * M.A[iscl[I][1]][iscl[J][2]] * M.A[iscl[I][2]][iscl[J][0]] +
                                          M.A[iscl[I][0]][iscl[J][2]] * M.A[iscl[I][1]][iscl[J][0]] * M.A[iscl[I][2]][iscl[J][1]] -
                                          M.A[iscl[I][0]][iscl[J][2]] * M.A[iscl[I][1]][iscl[J][1]] * M.A[iscl[I][2]][iscl[J][0]] -
                                          M.A[iscl[I][0]][iscl[J][0]] * M.A[iscl[I][1]][iscl[J][2]] * M.A[iscl[I][2]][iscl[J][1]] -
                                          M.A[iscl[I][0]][iscl[J][1]] * M.A[iscl[I][1]][iscl[J][0]] * M.A[iscl[I][2]][iscl[J][2]]);
}

__inline MATRIXd MatrInverse (MATRIXd M)
{
  double Det = M.A[0][0] * MatrMathDop(M, 0, 0) - M.A[0][1] * MatrMathDop(M, 0, 1) + M.A[0][2] * MatrMathDop(M, 0, 2) - M.A[0][3] * MatrMathDop(M, 0, 3);
  MATRIXd L;
  L.A[0][0] = MatrMathDop(M, 0, 0) / Det;
  L.A[0][1] = MatrMathDop(M, 1, 0) / Det;
  L.A[0][2] = MatrMathDop(M, 2, 0) / Det;
  L.A[0][3] = MatrMathDop(M, 3, 0) / Det;
  L.A[1][0] = MatrMathDop(M, 0, 1) / Det;
  L.A[1][1] = MatrMathDop(M, 1, 1) / Det;
  L.A[1][2] = MatrMathDop(M, 2, 1) / Det;
  L.A[1][3] = MatrMathDop(M, 3, 1) / Det;
  L.A[2][0] = MatrMathDop(M, 0, 2) / Det;
  L.A[2][1] = MatrMathDop(M, 1, 2) / Det;
  L.A[2][2] = MatrMathDop(M, 2, 2) / Det;
  L.A[2][3] = MatrMathDop(M, 3, 2) / Det;
  L.A[3][0] = MatrMathDop(M, 0, 3) / Det;
  L.A[3][1] = MatrMathDop(M, 1, 3) / Det;
  L.A[3][2] = MatrMathDop(M, 2, 3) / Det;
  L.A[3][3] = MatrMathDop(M, 3, 3) / Det;
  return L;
}

__inline VEC PointTransformer (VEC A, MATRIXd M)
{
  double W = M.A[0][3] * A.X + M.A[1][3] * A.Y + M.A[2][3] * A.Z + M.A[3][3];
  return VecSet( (M.A[0][0] * A.X + M.A[1][0] * A.Y + M.A[2][0] * A.Z + M.A[3][0]) / W,
                 (M.A[0][1] * A.X + M.A[1][1] * A.Y + M.A[2][1] * A.Z + M.A[3][1]) / W,
                 (M.A[0][2] * A.X + M.A[1][2] * A.Y + M.A[2][2] * A.Z + M.A[3][2]) / W);
}

__inline VEC VectorTransformer (VEC A, MATRIXd M)
{
  return VecSet( (M.A[0][0] * A.X + M.A[1][0] * A.Y + M.A[2][0] * A.Z),
                 (M.A[0][1] * A.X + M.A[1][1] * A.Y + M.A[2][1] * A.Z),
                 (M.A[0][2] * A.X + M.A[1][2] * A.Y + M.A[2][2] * A.Z));
}

__inline MATRIXd MatrLookAt (VEC Loc, VEC At, VEC UpApprox)
{
  VEC Dir, Up, Right;
  MATRIXd R;
  Dir = VecNormalize( VecSubVec( At, Loc ) );
  Right = VecNormalize( VecCrossVec( Dir, UpApprox ) );
  Up = VecCrossVec( Right, Dir );
  R.A[0][0] = Right.X;
  R.A[0][1] = Up.X;
  R.A[0][2] = -Dir.X;
  R.A[0][3] = 0;
  R.A[1][0] = Right.Y;
  R.A[1][1] = Up.Y;
  R.A[1][2] = -Dir.Y;
  R.A[1][3] = 0;
  R.A[2][0] = Right.Z;
  R.A[2][1] = Up.Z;
  R.A[2][2] = -Dir.Z;
  R.A[2][3] = 0;
  R.A[3][0] = -VecDotVec( Loc, Right );
  R.A[3][1] = -VecDotVec( Loc, Up );
  R.A[3][2] = VecDotVec( Loc, Dir );
  R.A[3][3] = 1;
  return R;
}

__inline MATRIXd MatrProjection( DBL Left, DBL Right,
                                DBL Bottom, DBL Top,
                                DBL Near, DBL Far )
{
  MATRIXd m =
  {
    {
      {      2 * Near / (Right - Left),                               0,                              0,  0},
      {                              0,       2 * Near / (Top - Bottom),                              0,  0},
      {(Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom),   -(Far + Near) / (Far - Near), -1},
      {                              0,                               0, -2 * Near * Far / (Far - Near),  0}
    }
  };
  return m;
} 

__inline MATRIXf ShaderMatrTransfom( MATRIXd Mi )
{
  MATRIXf Mo;
  Mo.A[0][0] = Mi.A[0][0];
  Mo.A[0][1] = Mi.A[0][1];
  Mo.A[0][2] = Mi.A[0][2];
  Mo.A[0][3] = Mi.A[0][3];
  Mo.A[1][0] = Mi.A[1][0];
  Mo.A[1][1] = Mi.A[1][1];
  Mo.A[1][2] = Mi.A[1][2];
  Mo.A[1][3] = Mi.A[1][3];
  Mo.A[2][0] = Mi.A[2][0];
  Mo.A[2][1] = Mi.A[2][1];
  Mo.A[2][2] = Mi.A[2][2];
  Mo.A[2][3] = Mi.A[2][3];
  Mo.A[3][0] = Mi.A[3][0];
  Mo.A[3][1] = Mi.A[3][1];
  Mo.A[3][2] = Mi.A[3][2];
  Mo.A[3][3] = Mi.A[3][3];
  return Mo;
}

#endif /* __VCT_H__ */
