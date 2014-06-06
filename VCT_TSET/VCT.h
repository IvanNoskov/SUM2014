/* NI01 */

#ifndef __VCT_H__
#define __VCT_H__

#include <math.h>

#include "DEF.H"

#define PI ((double)(3.1415926535897))
#define Deg2Rad 0.01745329251994329576L
#define toleranse 0.000002

typedef struct tagVEC{
  DOUBLE X, Y, Z;
} VEC;

typedef struct tagMATRIX
{
  DOUBLE A[4][4];
} MATRIX;

extern MATRIX UnitMatrix;
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

__inline MATRIX MatrSet (DOUBLE A11, DOUBLE A12, DOUBLE A13, DOUBLE A14,
                         DOUBLE A21, DOUBLE A22, DOUBLE A23, DOUBLE A24,
                         DOUBLE A31, DOUBLE A32, DOUBLE A33, DOUBLE A34,
                         DOUBLE A41, DOUBLE A42, DOUBLE A43, DOUBLE A44)
{
  MATRIX M;
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

__inline MATRIX MatrIdenity()
{
  return UnitMatrix;
}

__inline MATRIX MatrScale( DOUBLE sX, DOUBLE sY, DOUBLE sZ )
{
  MATRIX M = UnitMatrix;
  M.A[0][0] = sX;
  M.A[1][1] = sY;
  M.A[2][2] = sZ;
  return M;
}

__inline MATRIX MatrTranslate( DOUBLE dX, DOUBLE dY, DOUBLE dZ )
{
  MATRIX M = UnitMatrix;
  M.A[3][0] = dX;
  M.A[3][1] = dY;
  M.A[3][2] = dZ;
  return M;
}

__inline MATRIX MatrRotateX( DOUBLE AngleInDeg )
{
  MATRIX M = UnitMatrix;
  _asm {
    fld  AngleInDeg
    fmul MultiplierDegree2Radian
    fsincos
    fld  st(0)
    fstp M.A[1][1] 
    fstp M.A[2][2] 
    fld  st(0) 
    fstp M.A[1][2] 
    fldz       
    fsubr      
    fstp M.A[2][1] 
  }
  return M;
}

__inline MATRIX MatrRotateY( DOUBLE AngleInDeg )
{
  MATRIX M = UnitMatrix;
  _asm {
    fld  AngleInDeg
    fmul MultiplierDegree2Radian
    fsincos
    fld  st(0)
    fstp M.A[0][0] 
    fstp M.A[2][2] 
    fld  st(0) 
    fstp M.A[2][0] 
    fldz       
    fsubr      
    fstp M.A[0][1] 
  }
  return M;
}

__inline MATRIX MatrRotateZ( DOUBLE AngleInDeg )
{
  MATRIX M = UnitMatrix;
  _asm {
    fld  AngleInDeg
    fmul MultiplierDegree2Radian
    fsincos
    fld  st(0)
    fstp M.A[0][0] 
    fstp M.A[1][1] 
    fld  st(0) 
    fstp M.A[0][1] 
    fldz       
    fsubr      
    fstp M.A[1][0] 
  }
  return M;
}

__inline MATRIX MatrRotateVec( DOUBLE AngleInDeg, DOUBLE X, DOUBLE Y, DOUBLE Z )
{
  DBL a, sinA, cosA, len;
  MATRIX M;

  a = Deg2Rad * (AngleInDeg);
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

__inline MATRIX MatrMulMatr (MATRIX M1, MATRIX M2)
{
  MATRIX L;
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

__inline MATRIX MatrTranspose (MATRIX M)
{
  MATRIX L;
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

__inline DOUBLE MatrMathDop (MATRIX M, INT I, INT J)
{
  return (((I + J) % 2) == 0 ? -1 : 1) * (M.A[iscl[I][0]][iscl[J][0]] * M.A[iscl[I][1]][iscl[J][1]] * M.A[iscl[I][2]][iscl[J][2]] + 
                                          M.A[iscl[I][0]][iscl[J][1]] * M.A[iscl[I][1]][iscl[J][2]] * M.A[iscl[I][2]][iscl[J][0]] +
                                          M.A[iscl[I][0]][iscl[J][2]] * M.A[iscl[I][1]][iscl[J][0]] * M.A[iscl[I][2]][iscl[J][1]] -
                                          M.A[iscl[I][0]][iscl[J][2]] * M.A[iscl[I][1]][iscl[J][1]] * M.A[iscl[I][2]][iscl[J][0]] -
                                          M.A[iscl[I][0]][iscl[J][0]] * M.A[iscl[I][1]][iscl[J][2]] * M.A[iscl[I][2]][iscl[J][1]] -
                                          M.A[iscl[I][0]][iscl[J][1]] * M.A[iscl[I][1]][iscl[J][0]] * M.A[iscl[I][2]][iscl[J][2]]);
}

__inline MATRIX MatrInverse (MATRIX M)
{
  double Det = M.A[0][0] * MatrMathDop(M, 0, 0) - M.A[0][1] * MatrMathDop(M, 0, 1) + M.A[0][2] * MatrMathDop(M, 0, 2) - M.A[0][3] * MatrMathDop(M, 0, 3);
  MATRIX L;
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

__inline VEC PointTransformer (VEC A, MATRIX M)
{
  double W = M.A[0][3] * A.X + M.A[1][3] * A.Y + M.A[2][3] * A.Z + M.A[3][3];
  return VecSet( (M.A[0][0] * A.X + M.A[1][0] * A.Y + M.A[2][0] * A.Z + M.A[0][3]) / W,
                 (M.A[0][1] * A.X + M.A[1][1] * A.Y + M.A[2][1] * A.Z + M.A[1][3]) / W,
                 (M.A[0][2] * A.X + M.A[1][2] * A.Y + M.A[2][2] * A.Z + M.A[2][3]) / W);
}

__inline VEC VectorTransformer (VEC A, MATRIX M)
{
  double W = M.A[0][3] * A.X + M.A[1][3] * A.Y + M.A[2][3] * A.Z;
  return VecSet( (M.A[0][0] * A.X + M.A[1][0] * A.Y + M.A[2][0] * A.Z) / W,
                 (M.A[0][1] * A.X + M.A[1][1] * A.Y + M.A[2][1] * A.Z) / W,
                 (M.A[0][2] * A.X + M.A[1][2] * A.Y + M.A[2][2] * A.Z) / W);
}

#endif /* __VCT_H__ */
