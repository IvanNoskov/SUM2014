/* NI01, ANIM.C */


#include <stdlib.h>
#include <time.h>

#include "anim.h"
#include "render.h"

#include <mmsystem.h>

#pragma comment(lib, "winmm")

#define IN1_JoyStik_discretization 128.0

#define IN1_AXIS_deviation_VALUE(A) ((INT)(IN1_JoyStik_discretization * (2.0 * (Contr1.dw ## A ## pos - Jcaps.w ## A ## min) / (Jcaps.w ## A ## max - Jcaps.w ## A ##min - 1) - 1) + 0.5) / IN1_JoyStik_discretization)
#define IN1_AXIS_setting_VALUE(A) ((INT)(IN1_JoyStik_discretization * (1.0 * (Contr1.dw ## A ## pos - Jcaps.w ## A ## min) / (Jcaps.w ## A ## max - Jcaps.w ## A ##min - 1))) / IN1_JoyStik_discretization)

/* Main system Animation system Data */
static in1ANIM IN1_Anim;

BYTE
  Keys[256],
  KeysOld[256];

static INT
  IN1_MouseGlobalX, IN1_MouseGlobalY, /* Текущие координаты */
  IN1_MouseXOld, IN1_MouseYOld,       /* Сохраненные от прошлого кадра координаты */
  IN1_MouseGlobalWheel;               /* Состояние колеса мыши */
static HHOOK
  IN1_hMouseHook;

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

  IN1_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, IN1_MouseHook, GetModuleHandle(NULL), 0);

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
  LookAt( VecSet( 0, 0, 0),  VecSet( 1, 0, 0),  VecSet( 0, 1, 0) );
  SetProj( 6, 6, 0);
}

/* Animation Deinitialization function
 */
VOID IN1_AnimClose( VOID )
{
  INT i; /* Counter */

  UnhookWindowsHookEx( IN1_hMouseHook );

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
  ScreenOpt( H, W );

  ReleaseDC(IN1_Anim.hWnd, hDC);
} 

/* animation Farme Rendering function
 */
VOID IN1_AnimRender( VOID )
{
  INT i;            /* Counter */
  LARGE_INTEGER li; /* Temporal time syncronisation data */
  POINT MsPoint;

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
  SetDCBrushColor(IN1_Anim.hDC, RGB(0, 0, 0));
  Rectangle(IN1_Anim.hDC, 0, 0, IN1_Anim.W, IN1_Anim.H);

  /* Frame counting */
  FrameCounter++;

  /*-- Keyboard processing --*/
  GetKeyboardState( Keys );
  for (i = 0; i < 256; i++)
  {
    Keys[i] >>= 7;
    IN1_Anim.KeysClick[i] = Keys[i] && !KeysOld[i];
  }
  memcpy( IN1_Anim.Keys, Keys, sizeof(Keys) );
  memcpy( IN1_Anim.KeysOld, KeysOld, sizeof(KeysOld) );
  memcpy( KeysOld, Keys, sizeof(Keys) );
  /*-- Mouse processing --*/
  /*  wheel */
  IN1_Anim.MsWeel = IN1_MouseGlobalWheel;
  IN1_MouseGlobalWheel = 0;
  /* Main position */
  MsPoint.x = IN1_MouseGlobalX;
  MsPoint.y = IN1_MouseGlobalY;
  ScreenToClient(IN1_Anim.hWnd, &MsPoint);
  IN1_Anim.MsAbsX = MsPoint.x;
  IN1_Anim.MsAbsY = MsPoint.y;
  /* move */
  IN1_Anim.MsDeltaX = IN1_MouseGlobalX - IN1_MouseXOld;
  IN1_Anim.MsDeltaY = IN1_MouseGlobalY - IN1_MouseYOld;
  IN1_MouseXOld = IN1_MouseGlobalX;
  IN1_MouseYOld = IN1_MouseGlobalY;


  /*-- Joystic processing --*/
  if ((i = joyGetNumDevs()) > 0)
  {
    JOYCAPS Jcaps;

    /* getting information about joystic */
    if (joyGetDevCaps(JOYSTICKID1, &Jcaps, sizeof(Jcaps)) == JOYERR_NOERROR)
    {
      JOYINFOEX Contr1;

      /* getting Joystic state */
      Contr1.dwSize = sizeof(JOYCAPS);
      Contr1.dwFlags = JOY_RETURNALL;
      if (joyGetPosEx(JOYSTICKID1, &Contr1) == JOYERR_NOERROR)
      {
        /* Buttons */
        memcpy(IN1_Anim.JsButOld, IN1_Anim.JsBut, sizeof(IN1_Anim.JsBut));
        for (i = 0; i < 32; i++)
          IN1_Anim.JsBut[i] = (Contr1.dwButtons >> i) & 1;
        for (i = 0; i < 32; i++)
          IN1_Anim.JsButClick[i] = IN1_Anim.JsBut[i] && !IN1_Anim.JsButOld[i];

        /* axes */
        IN1_Anim.JsX = IN1_AXIS_deviation_VALUE(X);
        IN1_Anim.JsY = IN1_AXIS_deviation_VALUE(Y);
        if (Jcaps.wCaps & JOYCAPS_HASZ)
          IN1_Anim.JsZ = IN1_AXIS_setting_VALUE(Z);
        if (Jcaps.wCaps & JOYCAPS_HASR)
          IN1_Anim.JsR = IN1_AXIS_deviation_VALUE(R);
        if (Jcaps.wCaps & JOYCAPS_HASU)
          IN1_Anim.JsU = IN1_AXIS_deviation_VALUE(U);

        /* Point-Of-View */
        if (Jcaps.wCaps & JOYCAPS_HASPOV)
        {
          if (Contr1.dwPOV == 0xFFFF)
            IN1_Anim.JsPOV = 0;
          else
            IN1_Anim.JsPOV = Contr1.dwPOV / 4500 + 1;
        }
      }
    }
  }
  CameraRefresh( );
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
  static BOOL IsFullScreen = FALSE, IsAlreadyswitch = FALSE; /* mode */
  static RECT SaveRC;               /* swaed siZe */

  if ( IsAlreadyswitch )
    return;
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

    IsAlreadyswitch = !IsAlreadyswitch;
    IsFullScreen = TRUE;
    SetWindowPos(IN1_Anim.hWnd, HWND_TOP,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top + 201,
      SWP_NOOWNERZORDER);
    IsAlreadyswitch = !IsAlreadyswitch;
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

/* Функция обработки захваченных мышинных сообщений.
 * АРГУМЕНТЫ:
 *   - код захвата:
 *       INT Code;
 *   - параметр сообшения ('word parameter') -
 *     код сообщения от мыши:
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter') -
 *     (MSLLHOOKSTRUCT *) для мышинных сообщений;
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
static LRESULT CALLBACK IN1_MouseHook( INT Code, WPARAM wParam, LPARAM lParam )
{
  MSLLHOOKSTRUCT *hs = (MSLLHOOKSTRUCT *)lParam;

  switch (wParam)
  {
  case WM_MOUSEMOVE:
    IN1_MouseGlobalX = hs->pt.x;
    IN1_MouseGlobalY = hs->pt.y;
    break;
  case WM_MOUSEWHEEL:
    IN1_MouseGlobalWheel = (SHORT)HIWORD(hs->mouseData);
    break;
  }
  return 0;
} /* End of 'IN1_MouseHook' function */


