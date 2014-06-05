/* NI01, 03.06.2014 */

#include <stdlib.h>
#include <math.h>       

#include <windows.h>

#define WND_CLASS_NAME "my_fist's_window_class"
<<<<<<< HEAD
#define PI ((double)(3.1415926535897))

void DrawMSec (HDC hDCP, double A, int X, int Y)
{
  static POINT T[5];
  SelectObject( hDCP, GetStockObject( DC_PEN ) );
  SelectObject( hDCP, GetStockObject( DC_BRUSH ) );
  SetDCPenColor( hDCP, RGB( 0, 0, 0 ) );
  SetDCBrushColor( hDCP, RGB( 255, 255, 255 ) );
  T[0].x = X - 0 * sin( A  + (PI) );
  T[0].y = Y + 0 * cos( A  + (PI) );
  T[1].x = X - 50 * sin( A + 1 );
  T[1].y = Y + 50 * cos( A + 1 );
  T[3].x = X - 50 * sin( A + 1 );
  T[3].y = Y + 50 * cos( A + 1 );
  T[2].x = X - 47 * sin( A + (PI + 0.1) );
  T[2].y = Y + 47 * cos( A + (PI + 0.1) );
  T[4].x = X - 45 * sin( A + (PI) );
  T[4].y = Y + 45 * cos( A + (PI) );
  Polygon( hDCP, T, 5 );
  T[0].x = X - 0 * sin( A - (PI) );
  T[0].y = Y + 0 * cos( A - (PI) );
  T[1].x = X - 50 * sin( A - 1 );
  T[1].y = Y + 50 * cos( A - 1 );
  T[3].x = X - 50 * sin( A - 1 );
  T[3].y = Y + 50 * cos( A - 1 );
  T[2].x = X - 47 * sin( A - (PI + 0.1) );
  T[2].y = Y + 47 * cos( A - (PI + 0.1) );
  T[4].x = X - 45 * sin( A - (PI) );
  T[4].y = Y + 45 * cos( A - (PI) );
  Polygon( hDCP, T, 5 );
}


void DrawSec (HDC hDCP, double A, int X, int Y)
{
  static POINT T[5];
  SelectObject( hDCP, GetStockObject( DC_PEN ) );
  SelectObject( hDCP, GetStockObject( DC_BRUSH ) );
  SetDCPenColor( hDCP, RGB( 0, 0, 0 ) );
  SetDCBrushColor( hDCP, RGB( 255, 100, 100 ) );
  T[0].x = X - 10 * sin( A );
  T[0].y = Y + 10 * cos( A );
  T[1].x = X - 30 * sin( A + 1 );
  T[1].y = Y + 30 * cos( A + 1 );
  T[3].x = X - 30 * sin( A - 1 );
  T[3].y = Y + 30 * cos( A - 1 );
  T[2].x = X - 70 * sin( A + (PI + 0.1) );
  T[2].y = Y + 70 * cos( A + (PI + 0.1) );
  T[4].x = X - 70 * sin( A - (PI + 0.1) );
  T[4].y = Y + 70 * cos( A - (PI + 0.1) );
  Polygon( hDCP, T, 5 );
  T[0].x = X - 10 * sin( A );
  T[0].y = Y + 10 * cos( A );
  T[1].x = X - 30 * sin( A - 1 );
  T[1].y = Y + 30 * cos( A - 1 );
  T[3].x = X - 30 * sin( A + 1 );
  T[3].y = Y + 30 * cos( A + 1 );
  T[2].x = X - 70 * sin( A - (PI + 0.1) );
  T[2].y = Y + 70 * cos( A - (PI + 0.1) );
  T[4].x = X - 70 * sin( A + (PI + 0.1) );
  T[4].y = Y + 70 * cos( A + (PI + 0.1) );
  Polygon( hDCP, T, 5 );
}

void DrawMin (HDC hDCP, double A, int X, int Y)
{
  static POINT T[5];
  SelectObject( hDCP, GetStockObject( DC_PEN ) );
  SelectObject( hDCP, GetStockObject( DC_BRUSH ) );
  SetDCPenColor( hDCP, RGB( 0, 0, 0 ) );
  SetDCBrushColor( hDCP, RGB( 100, 255, 100 ) );
  T[0].x = X - 25 * sin( A );
  T[0].y = Y + 25 * cos( A );
  T[1].x = X - 25 * sin( A + PI / 2 );
  T[1].y = Y + 25 * cos( A + PI / 2 );
  T[3].x = X - 40 * sin( A + PI * 2 / 3 );
  T[3].y = Y + 40 * cos( A + PI * 2 / 3 );
  T[2].x = X - 60 * sin( A - PI / 2 );
  T[2].y = Y + 60 * cos( A - PI / 2 );
  T[4].x = X - 90 * sin( A + PI );
  T[4].y = Y + 90 * cos( A + PI );
  Polygon( hDCP, T, 5 );
  T[0].x = X - 25 * sin( A );
  T[0].y = Y + 25 * cos( A );
  T[1].x = X - 25 * sin( A - PI / 2 );
  T[1].y = Y + 25 * cos( A - PI / 2 );
  T[3].x = X - 40 * sin( A - PI * 2 / 3 );
  T[3].y = Y + 40 * cos( A - PI * 2 / 3 );
  T[2].x = X - 60 * sin( A + PI / 2 );
  T[2].y = Y + 60 * cos( A + PI / 2 );
  T[4].x = X - 90 * sin( A - PI );
  T[4].y = Y + 90 * cos( A - PI );
  Polygon( hDCP, T, 5 );
}

void DrawHour (HDC hDCP, double A, int X, int Y)
{
  static POINT T[5];
  SelectObject( hDCP, GetStockObject( DC_PEN ) );
  SelectObject( hDCP, GetStockObject( DC_BRUSH ) );
  T[0].x = X - 25 * sin( A );
  T[0].y = Y + 25 * cos( A );
  T[1].x = X - 20 * sin( A + 0.5 );
  T[1].y = Y + 20 * cos( A + 0.5 );
  T[2].x = X - 120 * sin( A + PI );
  T[2].y = Y + 120 * cos( A + PI );
  T[3].x = X - 20 * sin( A - 0.5 );
  T[3].y = Y + 20 * cos( A - 0.5 );
  SetDCPenColor( hDCP, RGB( 0, 0, 0 ) );
  SetDCBrushColor( hDCP, RGB( 10, 10, 155 ) );
  Polygon( hDCP, T, 4 );
  SetDCBrushColor( hDCP, RGB( 150, 150, 255 ) );
  T[0].x = X - 60 * sin( A + 0.3 );
  T[0].y = Y + 60 * cos( A + 0.3 );
  T[1].x = X - 50 * sin( A + 0.5 );
  T[1].y = Y + 50 * cos( A + 0.5 );
  T[4].x = X - 120 * sin( A + PI );
  T[4].y = Y + 120 * cos( A + PI );
  T[2].x = X - 70 * sin( A + 0.6 );
  T[2].y = Y + 70 * cos( A + 0.6 );
  T[3].x = X - 35 * sin( A + PI );
  T[3].y = Y + 35 * cos( A + PI );
  Polygon( hDCP, T, 5 );
  T[0].x = X - 60 * sin( A - 0.3 );
  T[0].y = Y + 60 * cos( A - 0.3 );
  T[1].x = X - 50 * sin( A - 0.5 );
  T[1].y = Y + 50 * cos( A - 0.5 );
  T[4].x = X - 120 * sin( A - PI );
  T[4].y = Y + 120 * cos( A - PI );
  T[2].x = X - 70 * sin( A - 0.6 );
  T[2].y = Y + 70 * cos( A - 0.6 );
  T[3].x = X - 35 * sin( A - PI );
  T[3].y = Y + 35 * cos( A - PI );
  Polygon( hDCP, T, 5 );
}
=======
#define PI (3,1415926535897)
>>>>>>> parent of 92d552e... T02CLOCK V1.0

void DrawDayOfWeek (HDC hDCP, double A, int X, int Y)
{
  static POINT T[4];
<<<<<<< HEAD
  SetTextColor( hDCP, RGB( 0, 0, 0 ) );
  SetBkMode( hDCP, TRANSPARENT );
  TextOut( hDCP, X + 160 * sin(PI * 2 / 7 * 0) - 8, Y - 160 * cos(PI * 2 / 7 * 0) - 8, "SU", 2);
  TextOut( hDCP, X + 160 * sin(PI * 2 / 7 * 1) - 8, Y - 160 * cos(PI * 2 / 7 * 1) - 8, "MO", 2);
  TextOut( hDCP, X + 160 * sin(PI * 2 / 7 * 2) - 8, Y - 160 * cos(PI * 2 / 7 * 2) - 8, "TU", 2);
  TextOut( hDCP, X + 160 * sin(PI * 2 / 7 * 3) - 8, Y - 160 * cos(PI * 2 / 7 * 3) - 8, "WE", 2);
  TextOut( hDCP, X + 160 * sin(PI * 2 / 7 * 4) - 8, Y - 160 * cos(PI * 2 / 7 * 4) - 8, "TH", 2);
  TextOut( hDCP, X + 160 * sin(PI * 2 / 7 * 5) - 8, Y - 160 * cos(PI * 2 / 7 * 5) - 8, "FR", 2);
  TextOut( hDCP, X + 160 * sin(PI * 2 / 7 * 6) - 8, Y - 160 * cos(PI * 2 / 7 * 6) - 8, "SA", 2);
=======
  T[0].x = X + 50 * cos( A );
  T[0].y = Y - 50 * sin( A );
  T[1].x = X + 40 * cos( A + 1 );
  T[1].y = Y - 40 * sin( A + 1 );
  T[2].x = X + 70 * cos( A + PI );
  T[2].y = Y - 70 * sin( A + PI );
  T[3].x = X + 40 * cos( A - 1 );
  T[3].y = Y - 40 * sin( A - 1 );

>>>>>>> parent of 92d552e... T02CLOCK V1.0
  SelectObject( hDCP, GetStockObject( DC_PEN ) );
  SelectObject( hDCP, GetStockObject( DC_BRUSH ) );
  T[0].x = X - 30 * sin( A );
  T[0].y = Y + 30 * cos( A );
  T[1].x = X - 10 * sin( A + 1 );
  T[1].y = Y + 10 * cos( A + 1 );
  T[2].x = X - 150 * sin( A + PI );
  T[2].y = Y + 150 * cos( A + PI );
  T[3].x = X - 10 * sin( A - 1 );
  T[3].y = Y + 10 * cos( A - 1 );
  SetDCPenColor( hDCP, RGB( 0, 0, 0 ) );
<<<<<<< HEAD
  SetDCBrushColor( hDCP, RGB( 10, 10, 10 ) );
=======
  SetDCBrushColor( hDCP, RGB( 255, 100, 100 ) );

>>>>>>> parent of 92d552e... T02CLOCK V1.0
  Polygon( hDCP, T, 4 );
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
  static INT W, H, SM;
  static HDC hMemDC, hDCLogo;
  static HBITMAP hBm, hBmLogo;
  SYSTEMTIME Time;
<<<<<<< HEAD
  static double Hour, Min, Sec, DayOfWeek, MSec;
  switch(Msg)
  {
  case WM_CREATE:
    SetTimer( hWnd, 30, 1, NULL );
    hBmLogo = LoadImage( NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(BITMAP), &BmLogo);
=======
  static double Hour, Min, Sec;
  static POINT Trangle[3] = {{10, 10}, {100, 15}, {15, 100}};
  switch(Msg)
  {
  case WM_CREATE:
    hDC = GetDC( hWnd );
    hMemDC = CreateCompatibleBitmap( hDC, W, H);
    ReleaseDC ( hWnd, hDC );
    SetTimer( hWnd, 30, 100, NULL );
>>>>>>> parent of 92d552e... T02CLOCK V1.0
    return 0;
  case WM_SIZE:
    H = HIWORD(lParam);
    W = LOWORD(lParam);
    if (H < W)
      SM = H;
    else
      SM = W;
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
    break;
  case WM_TIMER:
<<<<<<< HEAD
    GetLocalTime( &Time );
    MSec = (double)2 * PI * (Time.wMilliseconds / 1000.0);                                                                                       
    Sec = (double)2 * PI * (Time.wSecond / 60.0 + Time.wMilliseconds / 60000.0);
    Min = (double)2 * PI * (Time.wMinute * 60 + Time.wSecond) / (60 * 60);
    Hour = (double)2 * PI * ((Time.wHour % 12) * 60 * 60 + Time.wMinute * 60 + Time.wSecond) / (12 * 60 * 60);
    DayOfWeek = (double)2 * PI * ((Time.wDayOfWeek + Time.wHour / 24.0) / 7.0);
=======
    GetSystemTime( &Time );
    Sec = 60.0 * 2 * PI / Time.wSecond;
    Min = 60 * 60.0 * 2 * PI / (60 * Time.wMinute + Time.wSecond);
    Hour = 12 * 60 * 60.0 * 2 * PI / (120 * (Time.wHour % 12) + 60 * Time.wMinute + Time.wSecond);
>>>>>>> parent of 92d552e... T02CLOCK V1.0
    InvalidateRect( hWnd, NULL, FALSE);
    break;
  case WM_DESTROY:
    KillTimer(hWnd, 30);
    DeleteObject( hBm );
    DeleteDC( hMemDC );
    PostQuitMessage(0);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint( hWnd, &ps );
    SelectObject( hDC, GetStockObject( DC_PEN ) );
    SelectObject( hDC, GetStockObject( DC_BRUSH ) );
    SetDCPenColor( hDC, RGB( 0, 0, 0 ) );
    SetDCBrushColor( hDC, RGB( 255, 0, 255 ) );
    Rectangle( hMemDC, 0, 0, W, H);
<<<<<<< HEAD
    BitBlt( hMemDC, (W - BmLogo.bmWidth) / 2, (H - BmLogo.bmHeight) / 2, BmLogo.bmWidth, BmLogo.bmHeight, hDCLogo, 0, 0, SRCCOPY);
    DrawDayOfWeek( hMemDC, DayOfWeek, W / 2, H / 2 );
    DrawHour( hMemDC, Hour, W / 2, H / 2 );
    DrawMin( hMemDC, Min, W / 2, H / 2 );
    DrawSec( hMemDC, Sec, W / 2, H / 2 );
    DrawMSec( hMemDC, MSec, W / 2, H / 2 );
    BitBlt( hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
    /*  StretchBlt( hDC, (W - SM) / 2, (H - SM) / 2, SM, SM, hMemDC, (W - BmLogo.bmWidth) / 2, (H - BmLogo.bmHeight) / 2, (W + BmLogo.bmWidth) / 2, (H + BmLogo.bmHeight) / 2, SRCCOPY);*/
    DeleteDC( hMemDC );
    DeleteDC( hDCLogo );
=======
    DrawHour(hMemDC, Hour, W / 2, H / 2);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
>>>>>>> parent of 92d552e... T02CLOCK V1.0
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