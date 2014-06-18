/* FILENAME: GEOMHAND.C
 * PROGRAMMER: IN1
 * PURPOSE: Geometry object handle functions.
 * LAST UPDATE: 14.06.2014
 */

#include <stdlib.h>

#include "anim.h"
#include "shader.h"

/* Функция добавления материала к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       in1GEOM *G;
 *   - добавляемый материал:
 *       in1MATERIAL *Mtl;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного материала или -1
 *         при ошибке.
 */
INT IN1_GeomAddMaterial( in1GEOM *G, in1MATERIAL *Mtl )
{
  INT i;
  in1MATERIAL *M;

  /* ищем материал в библиотеке */
  for (i = 0; i < G->NumOfMtls; i++)
    if (strcmp(G->Mtls[i].Name, Mtl->Name)== 0)
    {
      G->Mtls[i] = *Mtl;
      return i;
    }

  if ((M = malloc(sizeof(in1MATERIAL) * (G->NumOfMtls + 1))) == NULL)
    return -1;
  if (G->Mtls != NULL)
  {
    /* копируем старые данные */
    memcpy(M, G->Mtls, sizeof(in1MATERIAL) * G->NumOfMtls);
    /* освобождаем память */
    free(G->Mtls);
  }
  /* добавляем */
  G->Mtls = M;
  M[G->NumOfMtls] = *Mtl;
  return G->NumOfMtls++;
} /* End of 'IN1_GeomAddMaterial' function */

/* Функция добавления примитива к объекту.
 * АРГУМЕНТЫ:
 *   - геометрический объект:
 *       in1GEOM *G;
 *   - добавляемый примитив:
 *       in1PRIM *Prim;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (INT) номер добавленного примитива или -1
 *         при ошибке.
 */
INT IN1_GeomAddPrim( in1GEOM *G, in1PRIM *Prim )
{
  in1PRIM *P;

  if ((P = malloc(sizeof(in1PRIM) * (G->NumOfPrims + 1))) == NULL)
    return -1;
  if (G->Prims != NULL)
  {
    /* копируем старые данные */
    memcpy(P, G->Prims, sizeof(in1PRIM) * G->NumOfPrims);
    /* освобождаем память */
    free(G->Prims);
  }
  /* добавляем */
  G->Prims = P;
  P[G->NumOfPrims] = *Prim;
  return G->NumOfPrims++;
} /* End of 'IN1_GeomAddPrim' function */

/* Функция освобождения */
VOID IN1_GeomFree( in1GEOM *G )
{
  INT i;

  for (i = 0; i < G->NumOfPrims; i++)
    IN1_PrimFree(G->Prims + i);
  free(G->Prims);
  for (i = 0; i < G->NumOfMtls; i++)
    glDeleteTextures(1, &G->Mtls[i].TexNo);
  free(G->Mtls);
  memset(G, 0, sizeof(in1GEOM));
} /* End of 'IN1_GeomFree' function */

/* Функция рисования */
VOID IN1_GeomDraw( in1GEOM *G )
{
  INT i, loc;
  MATRIXd WVP, MatrWorldInvTrans;
  VECf V;

  /* вычислили матрицы преобразования */
  WVP = MatrMulMatr(IN1_Anim.PrjMWorld,
    MatrMulMatr(IN1_Anim.PrjMView, IN1_Anim.PrjMProjection));
  MatrWorldInvTrans = MatrTranspose(MatrInverse(IN1_Anim.PrjMWorld));

  /* выбор программы шейдеров вывода примитивов */
  glUseProgram(IN1_ShaderProg);
  loc = glGetUniformLocation(IN1_ShaderProg, "MatrWVP");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, ShaderMatrTransfom(WVP).A[0]);

  loc = glGetUniformLocation(IN1_ShaderProg, "MatrWorldInverseTranspose");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, ShaderMatrTransfom(MatrWorldInvTrans).A[0]);

  loc = glGetUniformLocation(IN1_ShaderProg, "MatrWorld");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, ShaderMatrTransfom(IN1_Anim.PrjMWorld).A[0]);

  loc = glGetUniformLocation(IN1_ShaderProg, "MatrView");
  if (loc != -1)
    glUniformMatrix4fv(loc, 1, FALSE, ShaderMatrTransfom(IN1_Anim.PrjMView).A[0]);

  loc = glGetUniformLocation(IN1_ShaderProg, "Time");
  if (loc != -1)
    glUniform1f(loc, IN1_Anim.Time);

  V = ShaderVecTransfrov( IN1_Anim.Eye.Loc );
  loc = glGetUniformLocation(IN1_ShaderProg, "Eye");
  if (loc != -1)
    glUniform3fv(loc, 1, &V.X);

  V = ShaderVecTransfrov( VecAddVec( IN1_Anim.Eye.Loc, VecMulNum( IN1_Anim.Eye.Up, 0.3 ) ) );
  loc = glGetUniformLocation(IN1_ShaderProg, "Lantern");
  if (loc != -1)
    glUniform3fv(loc, 1, &V.X);

  V = ShaderVecTransfrov( IN1_Anim.Eye.Dir );
  loc = glGetUniformLocation(IN1_ShaderProg, "ViewDir");
  if (loc != -1)
    glUniform3fv(loc, 1, &V.X);


  for (i = 0; i < G->NumOfPrims; i++)
  {
    INT mtl = G->Prims[i].Mtl;

    /* подготавливаем материал */
    if (G->Mtls != NULL)
    {
      INT loc;

      if (G->Mtls[mtl].TexNo == 0 && G->Mtls[mtl].MapD[0] != 0)
      {
        INT j;
        in1IMAGE Img;
                       
        IN1_ImageLoad(&Img, G->Mtls[mtl].MapD);

        /* получаем свободный номер текстуры */
        glGenTextures(1, &G->Mtls[mtl].TexNo);
        /* делаем ее активной */
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        for (j = 0; j < Img.W * Img.H; j++)
          Img.Bits[j] |= 0xFF000000;
        /* отправляем картинку в видеопамять */
        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, Img.W, Img.H,
          GL_BGRA_EXT, GL_UNSIGNED_BYTE, Img.Bits);

        /* Параметры вывода */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);
      }
      /* передаем параметры */
      if (G->Mtls[mtl].TexNo != 0)
      {
        loc = glGetUniformLocation(IN1_ShaderProg, "DrawTexture");
        if (loc != -1)
          glUniform1i(loc, 0);
        glEnable(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);
        /*glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, G->Mtls[mtl].TexNo);*/
      }
      loc = glGetUniformLocation(IN1_ShaderProg, "Ka");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ka.X);
      loc = glGetUniformLocation(IN1_ShaderProg, "Kd");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Kd.X);
      loc = glGetUniformLocation(IN1_ShaderProg, "Ks");
      if (loc != -1)
        glUniform3fv(loc, 1, &G->Mtls[mtl].Ks.X);
      loc = glGetUniformLocation(IN1_ShaderProg, "Phong");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Phong);
      loc = glGetUniformLocation(IN1_ShaderProg, "Trans");
      if (loc != -1)
        glUniform1f(loc, G->Mtls[mtl].Trans);
    }
    IN1_PrimDraw(G->Prims + i);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
} /* End of 'IN1_GeomDraw' function */

/* END OF 'GEOMHAND.C' FILE */
