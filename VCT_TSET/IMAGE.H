/* NI01 */

#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "def.h"

typedef struct tagIMAGE
{
  INT W, H;    
  HBITMAP hBm; 
  DWORD *Bits; 
} IMAGE;

BOOL ImageLoad( IMAGE *Img, CHAR *FileName );

VOID ImageFree( IMAGE *Img );

DWORD ImageGetP( IMAGE *Img, INT X, INT Y );

#endif /* __IMAGE_H__ */

