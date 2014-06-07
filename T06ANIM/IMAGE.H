/* NI01, IMAGE.H */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "def.h"

/* Image data tipe */
typedef struct tagIMAGE
{
  INT W, H;    /* Size of image */
  HBITMAP hBm; /* image Handle */
  DWORD *Bits; /* inage Data (Bif format) */
} IMAGE;

/* Image Load function */
BOOL ImageLoad( IMAGE *Img, CHAR *FileName );

/* Image Freee function */
VOID ImageFree( IMAGE *Img );

/* Picsel Color getting function */
DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

#endif /* __IMAGE_H__ */

