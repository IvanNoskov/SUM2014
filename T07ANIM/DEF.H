/* NI01, UNITS.H */

#ifndef __DEF_H__
#define __DEF_H__

#pragma warning(disable: 4244 4305)

#include <windows.h>

#include <glew.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include <mmsystem.h>

#include <al.h>                                                                                    
#include <alc.h>
#include <alut.h>

#include "VCT.H"

#pragma comment(lib, "opengl32")
#pragma comment(lib, "glu32")
#pragma comment(lib, "glew32")
#pragma comment(lib, "winmm")
#pragma comment(lib, "alut")
#pragma comment(lib, "openal32")

/* Large integer data types */
typedef __int64 INT64;
typedef unsigned __int64 UINT64;

#endif /* __DEF_H__ */

