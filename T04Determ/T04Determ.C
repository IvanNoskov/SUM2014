/* NI01, 05.06.2014 */

#include <stdio.h>

#include <windows.h>

#define Nmax 7
#define defolteFileName "matric.txt"
#define toleranse (double)0.0000001

/* Matric[Nmax][Nmax] - matrix memory
 * Det - matrix Determinator
 * N - matrix size
 * Sign - sign of modified matrix Determinator < {-1, 1}
 */
double Matric[Nmax][Nmax], Det;
INT N, Sign;

/*Read Matrix fom "Filemame" file*/
BOOL ReadMatric(char *FileName)
{
/* *F - input file
 * i, j - counters
 * Tresh - Temp sourse for Matrc cutting
 */
  FILE *F;
  INT i, j;
  DOUBLE Tresh;
  /* open file*/
  if((F = fopen(FileName, "r")) == NULL)
    /* file didn't open*/
    return FALSE;
  /* read matrix size*/
  fscanf(F, "%i", &N);
  /* read matrix*/
  for(i = 0; i < N; i++)
    for(j = 0; j < N; j++)
    {
      /* read matrix element whis allowable indexs*/
      if (i < Nmax && j < Nmax)
        fscanf(F, "%lf", &(Matric[i][j]));
      /* cut matrix element whis unallowable indexs*/
      else
        fscanf(F, "%lf", &Tresh);
    }
  /* close file*/
  fclose(F);
  /* sucsess indicator*/
  return TRUE;
}


/* Count ABS of X */
DOUBLE ModX (DOUBLE X)
{
  if (X < 0)
    return -X;
  return X;
}

/* Swap Matric[a][?] and Matric[b][?]*/
void Switc_Str (INT a, INT b)
{
/* i - counter
 * Temp - Temp sourse for Swapping
 */
  INT i;
  DOUBLE Temp;
  for(i = 0; i < N; i++)
  {
    Temp = Matric[a][i];
    Matric[a][i] = Matric[b][i];
    Matric[b][i] = Temp;
  }
}

/* Swap Matric[?][a] and Matric[?][b]*/
void Switc_Sto (INT a, INT b)
{
/* i - counter
 * Temp - Temp sourse for Swapping
 */
  INT i;
  DOUBLE Temp;
  for(i = 0; i < N; i++)
  {
    Temp = Matric[i][a];
    Matric[i][a] = Matric[i][b];
    Matric[i][b] = Temp;
  }
}

/* Trunsform Matric to upper tringalar form vihtout chandje Deteminator ABC*/
BOOL Vidilenie ( void )
{
/* i, j, h - counters
 * j1, h1 - Temp sourse for MAX( Matric[j][h] ) search
 * B - Temp sourse for Matric[j][i] / Matric[i][i] ratio
 */
  INT i, j, h, h1, j1;
  double B;
  for(i = 0; i < N; i++)
  {
    /* search MAX( Matric[j][h] ); i <= j < N, i <= h < N, result in Matric[j1][h1]*/
    j1 = h1 = i;
    for (j = i; j < N; j++)
    {
      for (h = i; h < N; h++)
      {
        if (ModX( Matric[j][h] ) > ModX( Matric[j1][h1] ))
        {
          j1 = j, h1 = h;
        }
      }
    }
    /* if Matric[j1][h1] == 0 indicate this and fail out of function*/
    if (ModX( Matric[j1][h1] ) <= toleranse)
      return FALSE;
    /* Swap Matric[j1][h1] and Matric[i][i] whis counting matrix Determination sign*/
    if (j1 != i)
    {
      Switc_Str( i, j1 );
      Sign = -Sign;
    }
    if (h1 != i)
    {
      Switc_Sto( i, h1 );
      Sign = -Sign;
    }
    /* set Matric[j][i] to 0; i < j < N, without chanje Determinator*/
    for (j = i + 1; j < N; j++)
    {
      B = Matric[j][i] / Matric[i][i];
      for (h = i; h < N; h++)
      {
         Matric[j][h] -= B * Matric[i][h]; 
      }
    }
  }
  /* sucsess indicator*/
  return TRUE;
}

/* counting matric Determinator*/
void DetCount ( void )
{
  INT i;
  Det = Sign;
  for (i = 0; i < N; i++)
    Det *= Matric[i][i];
}

int main ( int argc, char *argv[] )
{
/* Buf[50] - String Buffer for filename and result messege
 */
  char Buf[50];
/* defult Det and Sign*/
  Det = 0;
  Sign = 1;
/* Scan filename*/
  printf("Input file Name\n");
  scanf("%s", Buf);
/* Scan matrix*/
  if (!ReadMatric( Buf ))
  {
    /* File didn't open*/
    MessageBox(NULL, "Can't Open file\n", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  if (N <= 0)
  {
    /* input matrix size is not positive*/
    MessageBox(NULL, "imposible matrix size\n", "ERROR", MB_OK | MB_ICONERROR);
    return 0;
  }
  if (N > Nmax)
  {
    /* input matrix size is bigget then maximum allowed value*/
    MessageBox(NULL, "Too big matrix\nmatrix will be cut to posible size", "ERROR", MB_OK | MB_ICONERROR);
  }
  /* matrix transformate*/
  if (Vidilenie())
    /* counting nonzero Determinator*/
    DetCount();
  /* output Determinator*/
  sprintf(Buf, "Def(A) = %lf", Det);
  MessageBox(NULL, Buf, "Sucses", MB_OK | MB_ICONINFORMATION);
  return 0;
}