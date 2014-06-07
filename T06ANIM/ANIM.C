/* NI01, ANIM.C */


#include <stdlib.h>
#include <time.h>

#include "anim.h"

/* Main system Animation system Data */
static in1ANIM IN1_Anim;

/* Time Sincronisation data */
static INT64
  TimeFreq,  /* tic per second */
  TimeStart, /* Animation Start time */
  TimeOld,   /* Previos Frame time */
  TimePause, /* Pause time */
  TimeFPS;   /* FPS marcer time */
static INT
  FrameCounter; /* frame counter */

/* Animation Initialization function
 * hWnd - Working Window Handle
 */
VOID IN1_AnimInit( HWND hWnd )
{
  HDC hDC = GetDC(hWnd); /* Temporal device context data*/
  LARGE_INTEGER li;      /* Temporal time syncronisation data */

  /* Animation system data initialization */
  IN1_Anim.hWnd = hWnd;
  IN1_Anim.hDC = CreateCompatibleDC(hDC);
  IN1_Anim.hBmFrame = CreateCompatibleBitmap(hDC, 30, 30);
  IN1_Anim.W = 30;
  IN1_Anim.H = 30;
  IN1_Anim.NumOfUnits = 0;
  ReleaseDC(hWnd, hDC);

  /* Timer data initialization */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;
}

/* Animation Deinitialization function
 */
VOID IN1_AnimClose( VOID )
{
  INT i; /* Counter */

  /* Units memory Free */
  for (i = 0; i < IN1_Anim.NumOfUnits; i++)
  {
    IN1_Anim.Units[i]->Close(IN1_Anim.Units[i], &IN1_Anim);
    free(IN1_Anim.Units[i]);
  }
  /* Animation system memory Free */
  DeleteObject(IN1_Anim.hBmFrame);
  DeleteDC(IN1_Anim.hDC);
  memset(&IN1_Anim, 0, sizeof(in1ANIM));
} 

/* Animation Resize processing function 
 * W, H - working window SIZE
 */
VOID IN1_AnimResize( INT W, INT H )
{
  HDC hDC = GetDC(IN1_Anim.hWnd);

  /* regeting Frame Buffer */
  DeleteObject(IN1_Anim.hBmFrame);
  IN1_Anim.hBmFrame = CreateCompatibleBitmap(hDC, W, H);
  SelectObject(IN1_Anim.hDC, IN1_Anim.hBmFrame);

  /* Сохранение размера */
  IN1_Anim.W = W;
  IN1_Anim.H = H;

  ReleaseDC(IN1_Anim.hWnd, hDC);
} 

/* animation Farme Rendering function
 */
VOID IN1_AnimRender( VOID )
{
  INT i;            /* Counter */
  LARGE_INTEGER li; /* Temporal time syncronisation data */

  /* Inpur regtting */
  GetKeyboardState(IN1_Anim.Keys);
  for (i = 0; i < 256; i++)
    IN1_Anim.Keys[i] >>= 7;

  /* Timer reggeting */
  QueryPerformanceCounter(&li);

  /* Global time */
  IN1_Anim.GlobalTime = (DBL)(li.QuadPart - TimeStart) / TimeFreq;
  IN1_Anim.GlobalDeltaTime = (DBL)(li.QuadPart - TimeOld) / TimeFreq;

  /* local time */
  if (IN1_Anim.IsPause)
  {
    TimePause += li.QuadPart - TimeOld;
    IN1_Anim.DeltaTime = 0;
  }
  else
    IN1_Anim.DeltaTime = IN1_Anim.GlobalDeltaTime;

  IN1_Anim.Time = (DBL)(li.QuadPart - TimeStart - TimePause) / TimeFreq;

  /* FPS counting */
  if (li.QuadPart - TimeFPS > TimeFreq)
  {
    IN1_Anim.FPS = FrameCounter / ((DBL)(li.QuadPart - TimeFPS) / TimeFreq);
    TimeFPS = li.QuadPart;
    FrameCounter = 0;
  }

  /* previos time seve */
  TimeOld = li.QuadPart;

  /* Fone cleaning */
  SelectObject(IN1_Anim.hDC, GetStockObject(DC_BRUSH));
  SelectObject(IN1_Anim.hDC, GetStockObject(NULL_PEN));
  SetDCBrushColor(IN1_Anim.hDC, RGB(0, 110, 0));
  Rectangle(IN1_Anim.hDC, 0, 0, IN1_Anim.W, IN1_Anim.H);

  /* animation units response */
  for (i = 0; i < IN1_Anim.NumOfUnits; i++)
    IN1_Anim.Units[i]->Response(IN1_Anim.Units[i], &IN1_Anim);

  /* animation units rendering */
  for (i = 0; i < IN1_Anim.NumOfUnits; i++)
  {
    SelectObject(IN1_Anim.hDC, GetStockObject(DC_BRUSH));
    SelectObject(IN1_Anim.hDC, GetStockObject(DC_PEN));
    SetDCBrushColor(IN1_Anim.hDC, RGB(0, 0, 0));
    SetDCPenColor(IN1_Anim.hDC, RGB(55, 155, 255));
    IN1_Anim.Units[i]->Render(IN1_Anim.Units[i], &IN1_Anim);
  }

  /* Frame counting */
  FrameCounter++;
} 

/* Animation frame Outpute function
 */
VOID IN1_AnimCopyFrame( VOID )
{
  HDC hDC;

  /* frame Outpute */
  hDC = GetDC(IN1_Anim.hWnd);
  BitBlt(hDC, 0, 0, IN1_Anim.W, IN1_Anim.H, IN1_Anim.hDC, 0, 0, SRCCOPY);
  ReleaseDC(IN1_Anim.hWnd, hDC);
}

/* Animation MultiFulscrin processing function
 */
VOID IN1_AnimAddUnit( in1UNIT *Unit )
{
  if (IN1_Anim.NumOfUnits < IN1_MAX_UNITS)
  {
    IN1_Anim.Units[IN1_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &IN1_Anim);
  }
} 
/* Animation Pause function
 */
VOID IN1_AnimFlipFullScreen( VOID )
{
  static BOOL IsFullScreen = FALSE; /* mode */
  static RECT SaveRC;               /* swaed siZe */

  if (!IsFullScreen)
  {
    RECT rc;
    HMONITOR hmon;
    MONITORINFOEX moninfo;

    /* size sawing */
    GetWindowRect(IN1_Anim.hWnd, &SaveRC);

    /* monitor tacing */
    hmon = MonitorFromWindow(IN1_Anim.hWnd, MONITOR_DEFAULTTONEAREST);

    /* monitor info getting */
    moninfo.cbSize = sizeof(moninfo);
    GetMonitorInfo(hmon, (MONITORINFO *)&moninfo);

    /* -> fulscree mode */
    rc = moninfo.rcMonitor;

    AdjustWindowRect(&rc, GetWindowLong(IN1_Anim.hWnd, GWL_STYLE), FALSE);

    SetWindowPos(IN1_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    /* Load window size */
    SetWindowPos(IN1_Anim.hWnd, HWND_TOPMOST,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
}

/* Animation Pause function
 * NewPauseFlag - new Pause flag
 */
VOID IN1_AnimSetPause( BOOL NewPauseFlag )
{
  IN1_Anim.IsPause = NewPauseFlag;
} 
