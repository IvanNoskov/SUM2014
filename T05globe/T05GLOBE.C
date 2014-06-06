/* NI01, 03.06.2014 */

#include <stdlib.h>
#include <math.h>       

#include <windows.h>

#define WND_CLASS_NAME "my_fist's_window_class"
#define PI ((double)(3.1415926535897))
#define dStepF ((double)(0.15))
#define dStepT ((double)(0.03))
#define dStepS ((double)(0.10))


VOID PaintGlobe( HDC hDCP, POINT O, INT R, INT Seed, DOUBLE a, DOUBLE b, DOUBLE stable, INT Mode)
{
  double f, t;
  int R1;
  POINT pol[4];
  SelectObject( hDCP, GetStockObject( NULL_PEN ) );
  SelectObject( hDCP, GetStockObject( DC_BRUSH ) );
  switch(Mode)
  {
  case (1):
    srand(Seed);
    for(f = 0; f < 2 * PI; f += dStepF)
      for(t = 0; t < PI; t += dStepT)
         if(R * sin( t ) * cos( f + a ) >= 0)
         {
           if (f > stable)
           {
             SetPixel ( hDCP, O.x + R * sin( t ) * sin( f + a ), O.y + R * cos( t ),  RGB( 75 +  rand() % 150, 75 +  rand() % 150, 75 +  rand() % 150 ));
             rand(), rand();
           }  
           else
           {
             R1 = (rand() % 30) * sin(rand());
             SetPixel ( hDCP, O.x + (R + R1) * sin( t ) * sin( f + a ), O.y + (R + R1) * cos( t ),  RGB( 75 +  rand() % 150, 75 +  rand() % 150, 75 +  rand() % 150 ));
           }
        }
        else
        {
          rand(), rand(), rand(), rand(), rand();
        }

    break;
  case (2):
    srand(Seed);
    for(f = 0; f < 2 * PI; f += dStepF)
      for(t = 0; t < PI; t += dStepT)
        if(R * sin( t ) * cos( f + a ) >= 0)
        {
          if (f > stable)
          {
            R1 = 0;
            rand(), rand();
          }  
          else
          {
            R1 = (rand() % 30) * sin(rand());
          }
          pol[0].x = O.x + (R + R1) * sin( t ) * sin( f + a );
          pol[0].y = O.y + (R + R1) * cos( t );
          pol[1].x = O.x + (R + R1) * sin( t + dStepT ) * sin( f + a );
          pol[1].y = O.y + (R + R1) * cos( t + dStepT );
          pol[2].x = O.x + (R + R1) * sin( t + dStepT ) * sin( f + dStepF + a );
          pol[2].y = O.y + (R + R1) * cos( t + dStepT );
          pol[3].x = O.x + (R + R1) * sin( t ) * sin( f + dStepF + a );
          pol[3].y = O.y + (R + R1) * cos( t );
          SetDCBrushColor( hDCP, RGB( 75 +  rand() % 150, 75 +  rand() % 150, 75 +  rand() % 150 ) );
          Polygon( hDCP, pol, 4);
        }
        else
        {
          rand(), rand(), rand(), rand(), rand();
        }
    break;
  }
}

VOID FlipFullScreen( HWND hWnd )
{
  static BOOL IsFullScreen = FALSE;
  static RECT SaveRC;              

  if (!IsFullScreen)
  {
    RECT rc;
    GetWindowRect(hWnd, &SaveRC);
    rc.left = 0;
    rc.top = 0;
    rc.right = GetSystemMetrics(SM_CXSCREEN);
    rc.bottom = GetSystemMetrics(SM_CYSCREEN);
    AdjustWindowRect(&rc, GetWindowLong(hWnd, GWL_STYLE), FALSE);
    SetWindowPos(hWnd, HWND_TOPMOST,
      rc.left, rc.top,
      rc.right - rc.left, rc.bottom - rc.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = TRUE;
  }
  else
  {
    SetWindowPos(hWnd, HWND_TOP,
      SaveRC.left, SaveRC.top,
      SaveRC.right - SaveRC.left, SaveRC.bottom - SaveRC.top,
      SWP_NOOWNERZORDER);
    IsFullScreen = FALSE;
  }
}


LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  RECT rc;
  CHAR ch;
  static INT W, H, SM, Mode = 2;
  static HDC hMemDC, hDCLogo;
  static HBITMAP hBm, hBmLogo;
  static BITMAP BmLogo;
  SYSTEMTIME Time;
  static double Spin, Stable = 0;
  static POINT O;
  static BOOL NRND = TRUE, PAUSE = FALSE;
  switch(Msg)
  {
  case WM_CREATE:
    SetTimer( hWnd, 30, 1, NULL );
    hBmLogo = LoadImage( NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(BITMAP), &BmLogo);
    return 0;
  case WM_SIZE:
    H = HIWORD(lParam);
    W = LOWORD(lParam);
    if (H < W)
      SM = H;
    else
      SM = W;
    O.x = W / 2;
    O.y = H / 2;
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC( hWnd );
    hBm = CreateCompatibleBitmap( hDC, W, H );
    ReleaseDC( hWnd, hDC );
    SelectObject( hMemDC, hDC );
    break;
  case WM_CHAR:
    ch = (CHAR)wParam;
    if (ch == 27)
      SendMessage( hWnd, WM_DESTROY, 0, 0);
    else if (ch == 'f')
      FlipFullScreen( hWnd );
    else if (ch == 'r')
      NRND = !NRND;
    else if (ch == 'p')
      PAUSE = !PAUSE;
    else if (ch == '1')
      Mode = 1;
    else if (ch == '2')
      Mode = 2;
    break;
  case WM_TIMER:
    GetLocalTime( &Time );
    if (!PAUSE)
      Spin = (double)2 * PI * (Time.wMilliseconds + Time.wSecond * 1000) / 10000.0; 
    if (NRND && Stable > -0.001)
      Stable -= dStepS;
    if (!NRND && Stable < 2 * PI)
      Stable += dStepS;
    InvalidateRect( hWnd, NULL, FALSE);
    break;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    DeleteObject( hBm );
    DeleteObject ( hBmLogo );
    PostQuitMessage(0);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint( hWnd, &ps );
    hMemDC = CreateCompatibleDC( hDC );
    hDCLogo = CreateCompatibleDC( hDC );
    SelectObject( hMemDC, hBm );
    SelectObject( hDCLogo, hBmLogo );
    SelectObject( hMemDC, GetStockObject( DC_PEN ) );
    SelectObject( hMemDC, GetStockObject( DC_BRUSH ) );
    SetDCPenColor( hMemDC, RGB( 0, 0, 0 ) );
    SetDCBrushColor( hMemDC, RGB( 0, 0, 0 ) );
    Rectangle( hMemDC, 0, 0, W, H);
    PaintGlobe( hMemDC, O, SM / 2 - 30, 30, Spin, 0.5, Stable, Mode);
    BitBlt( hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    DeleteDC( hMemDC );
    DeleteDC( hDCLogo );
    EndPaint( hWnd, &ps );
    break;
  }
  return DefWindowProc( hWnd, Msg, wParam, lParam);
}

INT WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstanse, CHAR *CmdLine, INT ShowCmd )
{
  WNDCLASS wc;
  HWND hWnd;
  MSG msg;

  wc.style = CS_VREDRAW | CS_HREDRAW;
  wc.cbWndExtra = 0;
  wc.cbClsExtra = 0;
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
  wc.hCursor = LoadCursor(NULL, IDC_CROSS);
  wc.hIcon = LoadIcon(NULL, IDI_SHIELD);
  wc.lpszMenuName = NULL;
  wc.hInstance =hInstance;
  wc.lpfnWndProc = MyWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  if( !RegisterClass( &wc ) )
  {
    MessageBox( NULL, "ERRoR", "ERRoR", MB_OK | MB_ICONWARNING );
    return 0;
  }

  hWnd = CreateWindow( WND_CLASS_NAME, "some points", WS_OVERLAPPEDWINDOW, 
                       CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
                       NULL, NULL, hInstance, NULL );
  ShowWindow( hWnd, ShowCmd );
  UpdateWindow( hWnd );

  while (GetMessage( &msg, NULL, 0, 0 ))
  {
    TranslateMessage( &msg );
    DispatchMessage( &msg );
  }

  return msg.wParam;
}