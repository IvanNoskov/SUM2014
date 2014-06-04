/* NI01, 02.06.2014 */

#include <stdio.h>

#include <windows.h>

#define Nmax 7
#define defolteFileName "matric.txt"

double Matric[Nmax][Nmax], Det;
INT N;

BOOL ReadMatric(char *FileName)
{
  FILE *F;
  INT i, j;
  DOUBLE Tresh;
  if((F = fopen(FileName, "r")) == NULL)
    return FALSE;
  fscanf(F, "%i", &N);
  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++)
    {
      if (i < Nmax && j < Nmax)
        fscanf(F, "%lf", &(Matric[i][j]));
      else
        fscanf(F, "%lf", &Tresh);
    }
  fclose(F);
  return TRUE;
}

void PrintMas( INT *Mas, INT P )
{
  int i;
  double Temp = 1.0;
  for( i = 0; i < N; i++ )
    Temp *= Matric[i][Mas[i]];
  Det += P * Temp;
}

void SVAP_int(INT *A, INT *B)
{
  INT T;
  T = *A;
  *A = *B;
  *B = T;
}

void MakePermutation(INT K, INT *Mas, INT P)
{
  int i;
  if (K == N - 1)
  {
    PrintMas(Mas, P);
    return;
  }
  MakePermutation( K + 1, Mas, P);
  for(i = K + 1; i < N; i++)
  {
    SVAP_int(&(Mas[K]), &(Mas[i]));
    MakePermutation( K + 1, Mas, -P);
    SVAP_int(&(Mas[K]), &(Mas[i]));
  }
}


int main ( int argc, char *argv[] )
{
  INT Mass[Nmax], i;
  char Buf[50];
  Det = 0;
  for (i = 0; i < Nmax; i++)
    Mass[i] = i;
  if (!ReadMatric( defolteFileName ))
  {
    MessageBox(NULL, "Can't Open file\n", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  if (N <= 0)
  {
    MessageBox(NULL, "imposible matric size\n", "ERROR", MB_OK | MB_ICONERROR);
  }
  if (N > Nmax)
  {
    MessageBox(NULL, "Too big matric\nmaric will be cut to posible size", "ERROR", MB_OK | MB_ICONERROR);
  }
  MakePermutation(0, Mass, 1);
  sprintf(Buf, "Def(A) = %lf", Det);
  MessageBox(NULL, Buf, "Sucses", MB_OK | MB_ICONINFORMATION);
  return 0;
}