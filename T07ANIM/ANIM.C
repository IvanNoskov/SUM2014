/* NI01, ANIM.C */


#include <stdlib.h>
#include <time.h>

#include "anim.h"
#include "render.h"

#define IN1_JoyStik_discretization 128.0

#define IN1_AXIS_deviation_VALUE(A) ((INT)(IN1_JoyStik_discretization * (2.0 * (Contr1.dw ## A ## pos - Jcaps.w ## A ## min) / (Jcaps.w ## A ## max - Jcaps.w ## A ##min - 1) - 1) + 0.5) / IN1_JoyStik_discretization)
#define IN1_AXIS_setting_VALUE(A) ((INT)(IN1_JoyStik_discretization * (1.0 * (Contr1.dw ## A ## pos - Jcaps.w ## A ## min) / (Jcaps.w ## A ## max - Jcaps.w ## A ##min - 1))) / IN1_JoyStik_discretization)

/* Main system Animation system Data */
in1ANIM IN1_Anim;

BYTE
  Keys[256],
  KeysOld[256];

static INT
  IN1_MouseGlobalX, IN1_MouseGlobalY, /* ������� ���������� */
  IN1_MouseXOld, IN1_MouseYOld,       /* ����������� �� �������� ����� ���������� */
  IN1_MouseGlobalWheel;               /* ��������� ������ ���� */
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
BOOL IN1_AnimInit( HWND hWnd )
{
  PIXELFORMATDESCRIPTOR pfd = {0};
  LARGE_INTEGER li;      /* Temporal time syncronisation data */
  INT i;

  /* Animation system data initialization */
  IN1_Anim.hDC = GetDC(hWnd);
  IN1_Anim.hWnd = hWnd;
  IN1_Anim.W = 30;
  IN1_Anim.H = 30;
  IN1_Anim.NumOfUnits = 0;

  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_SUPPORT_GDI | PFD_DOUBLEBUFFER;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cDepthBits = 32;
  i = ChoosePixelFormat(IN1_Anim.hDC, &pfd);
  DescribePixelFormat(IN1_Anim.hDC, i, sizeof(pfd), &pfd);
  SetPixelFormat(IN1_Anim.hDC, i, &pfd);
  /* ������� �������� ���������� */
  IN1_Anim.hRC = wglCreateContext(IN1_Anim.hDC);
  /* ������ �������� ��������� */
  wglMakeCurrent(IN1_Anim.hDC, IN1_Anim.hRC);

  /* �������������� ���������� */
  if (glewInit() != GLEW_OK ||
      !(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader))
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(IN1_Anim.hRC);
    ReleaseDC(IN1_Anim.hWnd, IN1_Anim.hDC);
    memset(&IN1_Anim, 0, sizeof(in1ANIM));
    return FALSE;
  }

  /* Timer data initialization */
  QueryPerformanceFrequency(&li);
  TimeFreq = li.QuadPart;
  QueryPerformanceCounter(&li);
  TimeStart = TimeOld = TimeFPS = li.QuadPart;
  TimePause = 0;
  FrameCounter = 0;

  IN1_hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, IN1_MouseHook, GetModuleHandle(NULL), 0);
  
  glEnable( GL_DEPTH_TEST );
  glEnable( GL_ALPHA_TEST );

  IN1_Anim.PrjMWorld = MatrIdenity( );

  IN1_Anim.Eye.Dir = VecNormalize( VecSet( -1, -1, -1 ) );
  IN1_Anim.Eye.Loc = VecSet( 10, 10, 10 );
  IN1_Anim.Eye.Right = VecNormalize( VecCrossVec( IN1_Anim.Eye.Dir, VecSet( 0, 1, 0 ) ) );
  IN1_Anim.Eye.Up = VecCrossVec( IN1_Anim.Eye.Right, IN1_Anim.Eye.Dir );
  IN1_Anim.PrjMView = MatrLookAt( IN1_Anim.Eye.Loc, VecAddVec( IN1_Anim.Eye.Loc, IN1_Anim.Eye.Dir), IN1_Anim.Eye.Up );

  IN1_Anim.PrjH = 3;
  IN1_Anim.PrjW = 4;
  IN1_Anim.PrjDist = 1;
  IN1_Anim.PrjFar = 1000;
  IN1_Anim.PrjSize = 1;
  IN1_Anim.PrjMProjection = MatrProjection( -IN1_Anim.PrjW / 2, IN1_Anim.PrjW / 2, -IN1_Anim.PrjH / 2, IN1_Anim.PrjH / 2, IN1_Anim.PrjDist, IN1_Anim.PrjFar);
  IN1_AnimPrjMResponse();

  alutInit( NULL, 0 );
  alGetError();

  IN1_Anim.IsPause = TRUE;

  return TRUE;
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

  wglMakeCurrent(NULL, NULL);

  glDisable( GL_DEPTH_TEST );
  glDisable( GL_ALPHA_TEST );  

  wglDeleteContext(IN1_Anim.hRC);
  ReleaseDC(IN1_Anim.hWnd, IN1_Anim.hDC);
  /* Animation system memory Free */
  ReleaseDC( IN1_Anim.hWnd, IN1_Anim.hDC );
  memset(&IN1_Anim, 0, sizeof(in1ANIM));
} 

/* Animation Resize processing function 
 * W, H - working window SIZE
 */
VOID IN1_AnimResize( INT W, INT H )
{
  DBL size = 1, ratio_W = 1, ratio_H = 1;
  /* Size saving */
  IN1_Anim.W = W;
  IN1_Anim.H = H;
  glViewport(0, 0, W, H);
  if (W > H)
    ratio_W = (DBL)W / H;
  else
    ratio_H = (DBL)H / W;
  IN1_Anim.PrjH = IN1_Anim.PrjSize * ratio_H;
  IN1_Anim.PrjW = IN1_Anim.PrjSize * ratio_W;
  IN1_Anim.PrjMProjection = MatrProjection( -IN1_Anim.PrjW / 2, IN1_Anim.PrjW / 2, -IN1_Anim.PrjH / 2, IN1_Anim.PrjH / 2, IN1_Anim.PrjDist, IN1_Anim.PrjFar);
  IN1_AnimPrjMResponse();
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

  /* Camera proccessing */
  IN1_Anim.PrjMView = MatrLookAt( IN1_Anim.Eye.Loc, VecAddVec( IN1_Anim.Eye.Loc, IN1_Anim.Eye.Dir), IN1_Anim.Eye.Up );
  IN1_AnimPrjMResponse();


  /* Background cleaning */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

/*  glColor3d( 1, 1, 1);

  glRectd(0, 0, 1, 1);
  glRectd(0, 0, -1, -1);

  glColor3d( 1, 0.3, sin( IN1_Anim.GlobalTime ) / 2 + 0.5);

  glRectd (-1, 1, -0.55 + cos( IN1_Anim.Time * 1.25 ) / 3, 0.55 - cos( IN1_Anim.Time  * 1.75) / 3);*/


  /* animation units response */
  for (i = 0; i < IN1_Anim.NumOfUnits; i++)
    IN1_Anim.Units[i]->Response(IN1_Anim.Units[i], &IN1_Anim);

  for (i = 0; i < IN1_Anim.NumOfUnits; i++)
    IN1_Anim.Units[i]->Collision(IN1_Anim.Units[i], &IN1_Anim);


  /* animation units rendering */
  for (i = 0; i < IN1_Anim.NumOfUnits; i++)
  {
    glDepthMask(1);
    IN1_Anim.Units[i]->Render(IN1_Anim.Units[i], &IN1_Anim);
  }
  
  /* Frame counting */
  FrameCounter++;
} 

/* Animation frame Outpute function
 */
VOID IN1_AnimCopyFrame( VOID )
{
  /* frame Outpute */
  glFinish();
  SwapBuffers( IN1_Anim.hDC );
}

VOID IN1_AnimPrjMResponse()
{
  IN1_Anim.PrjM_VP = MatrMulMatr( IN1_Anim.PrjMView,  IN1_Anim.PrjMProjection );
  IN1_Anim.PrjM_WV = MatrMulMatr( IN1_Anim.PrjMWorld, IN1_Anim.PrjMView );
  IN1_Anim.PrjM_WVP = MatrMulMatr( MatrMulMatr( IN1_Anim.PrjMWorld, IN1_Anim.PrjMView ), IN1_Anim.PrjMProjection );
}

/* Adding Unit to Animation Sistem
 */
VOID IN1_AnimAddUnit( in1UNIT *Unit )
{
  if (IN1_Anim.NumOfUnits < IN1_MAX_UNITS)
  {
    IN1_Anim.Units[IN1_Anim.NumOfUnits++] = Unit;
    Unit->Init(Unit, &IN1_Anim);
  }
} 
/* Animation MultiFulscrin processing function
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
      rc.right - rc.left, rc.bottom - rc.top,
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

/* ������� ��������� ����������� �������� ���������.
 * ���������:
 *   - ��� �������:
 *       INT Code;
 *   - �������� ��������� ('word parameter') -
 *     ��� ��������� �� ����:
 *       WPARAM wParam;
 *   - �������� ��������� ('long parameter') -
 *     (MSLLHOOKSTRUCT *) ��� �������� ���������;
 *       LPARAM lParam;
 * ������������ ��������:
 *   (LRESULT) - � ����������� �� ���������.
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
}