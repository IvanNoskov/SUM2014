/* NI01, 02.06.2014 */

#include <stdio.h>

#include <windows.h>

#define Nmax 10

void PrintMas( INT *Mas, FILE *F, INT P )
{
  int i;
  for( i = 0; i < Nmax; i++ )
    fprintf( F, "%i ", Mas[i]);
  fprintf(F, "-> number of inversions %i\n", P);
}

void MakePermutation (INT N, INT *Mas, INT P, BOOL *Used, FILE *F)
{
  int i, j;
  if (N == Nmax)
  {
    PrintMas(Mas, F, P);
    return;
  }
  for(i = 0, j = 0; i < Nmax; i++)
  {
    if (!Used[i])
    {
      Used[i] = TRUE;
      Mas[N] = i;
      MakePermutation(N + 1, Mas, P + j, Used, F);
      Used[i] = FALSE;
      j++;
    }
  }
}


int main ( int argc, char *argv[] )
{
  INT Mass[Nmax], i;
  BOOL Used[Nmax];
  FILE *F;
  for (i = 0; i < Nmax; i++)
    Used[i] = FALSE;
  if ((F = fopen("T03.log", "a")) == NULL)
  {
    MessageBox(NULL, "Can't Open file\n", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  fprintf( F, "/*--N = %i--/\n", Nmax);
  MakePermutation(0, Mass, 0, Used, F);
  fclose(F);
  MessageBox(NULL, "Permutation making finished sucsesful\n", "Sucses", MB_OK | MB_ICONINFORMATION);
  return 0;
}