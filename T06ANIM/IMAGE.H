/* NI01, IMAGE.H */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "def.h"

/* Image data tipe */
typedef struct tagin1IMAGE
{
  INT W, H;    /* Size of image */
  HBITMAP hBm; /* image Handle */
  DWORD *Bits; /* inage Data (Bif format) */
} in1IMAGE;

/* Image Load function */
BOOL IN1_ImageLoad( in1IMAGE *Img, CHAR *FileName );

/* Image Freee function */
VOID IN1_ImageFree( in1IMAGE *Img );

/* Picsel Color getting function */
DWORD IN1_ImageGetP( in1IMAGE *Img, INT X, INT Y );

void IN1_ImagePaint ( HDC putPaintPlase, INT X, INT Y, in1IMAGE * Img, INT PaintMode);

#endif /* __IMAGE_H__ */

