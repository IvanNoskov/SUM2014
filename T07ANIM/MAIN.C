/* NI01, STARTUP.C */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

#define WND_CLASS_NAME "Window Class Name"

/* Message processing function
 * hWnd - window descriptor
 * Msg - messege tipe
 * wParam - 'word' messege parameter
 * lParam - 'long' messege parameter
 * (LRESULT) - ansver to message.
 */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  MINMAXINFO *minmax;
  static BOOL pause;

  switch (Msg)
  {
  case WM_GETMINMAXINFO:
    minmax = (MINMAXINFO *)lParam;
    minmax->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK) +
      GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CYBORDER) * 2;
    return 0;
  case WM_CREATE:
    SetTimer(hWnd, 30, 1, NULL);
    if(!IN1_AnimInit( hWnd ))
    {
      SendMessage( hWnd, WM_DESTROY, 0, 0 );
      return -1;
    }
    return 0;
  case WM_SIZE:
    IN1_AnimResize(LOWORD(lParam), HIWORD(lParam));
  case WM_TIMER:
    IN1_AnimRender();
    IN1_AnimCopyFrame();
    return 0;
  case WM_ERASEBKGND:
    return 1;
  case WM_PAINT:
    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    IN1_AnimCopyFrame();
    return 0;
  case WM_DESTROY:
    IN1_AnimClose();
    PostQuitMessage(0);
    KillTimer(hWnd, 30);
    return 0;
  }
  return DefWindowProc(hWnd, Msg, wParam, lParam);
}
/* Starting program pount
 * hInstance - Handle to an instance.
 * hPrevInstance - Handle to an previos instance (don't use, must be NULL).
 * *CmdLine - Comand line
 * ShowCmd window chow flag
 */
INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;
  INT id = 0;

  /* class initialization */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0; /* additive class bites number */
  wc.cbWndExtra = 0; /* additive window bites number */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* background color */
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* class Registration */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  /* Window creation */
  hWnd = CreateWindow(WND_CLASS_NAME, "First Window Sample",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, /* Window position (Defolte) */
    CW_USEDEFAULT, CW_USEDEFAULT, /* Window size (Defolte) */
    NULL,                         /* parent window discriptor */
    NULL,                         /* menu discriptor */
    hInstance,                    /* program discriptor */
    NULL);                        /* pointer to advanset parametres */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /*--- adding Units to Animation system  ---*/
  IN1_AnimAddUnit( IN1_INFODisplayUnitCreate( id++ ) );
  IN1_AnimAddUnit( IN1_CAMERAUnitCreate( id++ ) ); 
  IN1_AnimAddUnit( IN1_KEYUnitCreate( id++, VecSet( 10, 0, 0 ) ) );
  IN1_AnimAddUnit( IN1_KEYUnitCreate( id++, VecSet( 10, 10, 0 ) ) );
  IN1_AnimAddUnit( IN1_KEYUnitCreate( id++, VecSet( -10, 0, 0 ) ) );
  IN1_AnimAddUnit( IN1_KEYUnitCreate( id++, VecSet( 0, 10, 0 ) ) );
  IN1_AnimAddUnit( IN1_KEYUnitCreate( id++, VecSet( 0, 0, 10 ) ) );
  IN1_AnimAddUnit( IN1_KEYUnitCreate( id++, VecSet( 0, -10, 0 ) ) );
  IN1_AnimAddUnit( IN1_GOSTUnitCreate( id++, 5 ) ); 
  IN1_AnimAddUnit( IN1_HOUSEUnitCreate( id++ ) ); 
  
/*  IN1_AnimAddUnit( IN1_CARUnitCreate( 0 ) );*/

  /* Message processing */
  while (TRUE)
  {
    if (PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
      if (msg.message == WM_QUIT)
        break;
      TranslateMessage(&msg);
      /* processing message in our Message processing function */
      DispatchMessage(&msg);
    }
    else
      SendMessage( hWnd, WM_TIMER, 0, 0 );
  }

  return msg.wParam;
}

