/* NI01, IMAGE.C */


#include "image.h"

/* Image loading function
 * *Img - pointer to IMAGE struct
 * *Filename - input file name
 */
BOOL IN1_ImageLoad( in1IMAGE *Img, CHAR *FileName )
{
  HBITMAP hBmLoad;

  if (Img == NULL)
    return FALSE;
  Img->W = Img->H = 0;
  Img->hBm = NULL;
  Img->Bits = NULL;
  if ((hBmLoad = LoadImage(NULL, FileName,
         IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE)) != NULL)
  {
    BITMAP bm;
    BITMAPINFOHEADER bmi;
    HDC hDC, hMemDC1, hMemDC2;

    GetObject(hBmLoad, sizeof(bm), &bm);
    Img->W = bm.bmWidth;
    Img->H = bm.bmHeight;

    bmi.biSize = sizeof(BITMAPINFOHEADER);
    bmi.biWidth = Img->W;
    bmi.biHeight = -Img->H;
    bmi.biBitCount = 32;
    bmi.biPlanes = 1;
    bmi.biCompression = BI_RGB;
    bmi.biSizeImage = Img->W * Img->H * 4;
    bmi.biXPelsPerMeter = bmi.biYPelsPerMeter = 1;
    bmi.biClrUsed = 0;
    bmi.biClrImportant = 0;

    Img->hBm = CreateDIBSection(NULL, (BITMAPINFO *)&bmi, DIB_RGB_COLORS,
      (VOID **)&Img->Bits, NULL, 0);

    hDC = GetDC(NULL);
    hMemDC1 = CreateCompatibleDC(hDC);
    hMemDC2 = CreateCompatibleDC(hDC);

    SelectObject(hMemDC1, hBmLoad);
    SelectObject(hMemDC2, Img->hBm);
    BitBlt(hMemDC2, 0, 0, Img->W, Img->H, hMemDC1, 0, 0, SRCCOPY);

    DeleteDC(hMemDC1);
    DeleteDC(hMemDC2);
    ReleaseDC(NULL, hDC);
    DeleteObject(hBmLoad);
  }

  return Img->hBm != NULL;
}

/* in1IMAGE free function
 * *Img - pointer to in1IMAGE struct
 */
VOID IN1_ImageFree( in1IMAGE *Img )
{
  if (Img == NULL)
    return;
  if (Img->hBm != NULL)
    DeleteObject(Img->hBm);
  Img->W = Img->H = 0;
  Img->hBm = NULL;
  Img->Bits = NULL;
}

/* picsel color getting function.
 * *Img - pointer to in1IMAGE struct
 * X, Y - picsel coordinates
 */
DWORD IN1_ImageGetP( in1IMAGE *Img, INT X, INT Y )
{
  if (Img == NULL)
    return 0;
  if (Img->hBm != NULL &&
      X >= 0 && Y >= 0 && X < Img->W && Y < Img->H)
  {
    DWORD color;
    BYTE r, g, b;

    color = Img->Bits[Y * Img->W + X];
    b = color & 0xFF;
    g = (color >> 8) & 0xFF;
    r = (color >> 16) & 0xFF;
    /*return RBG color*/
    return RGB(r, g, b);
  }
  return 0;
} 

void IN1_ImagePaint ( HDC putPaintPlase, INT X, INT Y, in1IMAGE * Img, INT PaintMode)
{     
  HDC hDCLogo;

  if (Img == NULL)
    return;
  hDCLogo = CreateCompatibleDC( putPaintPlase );
  SelectObject ( hDCLogo, Img->hBm );
  BitBlt( putPaintPlase, X, Y, Img->W, Img->H, hDCLogo, 0, 0, PaintMode); 
  DeleteDC ( hDCLogo );
}
