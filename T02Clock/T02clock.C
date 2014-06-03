/* NI01, 03.06.2014 */

#include <stdlib.h>
#include <math.h>       

#include <windows.h>

#define WND_CLASS_NAME "my_fist's_window_class"
#define PI ((double)(3.1415926535897))

void DrawSec (HDC hDCP, double A, int X, int Y)
{
  static POINT T[4];
  T[0].x = X - 50 * sin( A );
  T[0].y = Y + 50 * cos( A );
  T[1].x = X - 40 * sin( A + 1 );
  T[1].y = Y + 40 * cos( A + 1 );
  T[2].x = X - 70 * sin( A + PI );
  T[2].y = Y + 70 * cos( A + PI );
  T[3].x = X - 40 * sin( A - 1 );
  T[3].y = Y + 40 * cos( A - 1 );

  SelectObject( hDCP, GetStockObject( DC_PEN ) );
  SelectObject( hDCP, GetStockObject( DC_BRUSH ) );
  SetDCPenColor( hDCP, RGB( 0, 0, 0 ) );
  SetDCBrushColor( hDCP, RGB( 255, 100, 100 ) );

  Polygon( hDCP, T, 4 );
}

void DrawMin (HDC hDCP, double A, int X, int Y)
{
  static POINT T[4];
  T[0].x = X - 70 * sin( A );
  T[0].y = Y + 70 * cos( A );
  T[1].x = X - 60 * sin( A + 1 );
  T[1].y = Y + 60 * cos( A + 1 );
  T[2].x = X - 90 * sin( A + PI );
  T[2].y = Y + 90 * cos( A + PI );
  T[3].x = X - 60 * sin( A - 1 );
  T[3].y = Y + 60 * cos( A - 1 );

  SelectObject( hDCP, GetStockObject( DC_PEN ) );
  SelectObject( hDCP, GetStockObject( DC_BRUSH ) );
  SetDCPenColor( hDCP, RGB( 0, 0, 0 ) );
  SetDCBrushColor( hDCP, RGB( 100, 255, 100 ) );

  Polygon( hDCP, T, 4 );
}

void DrawHour (HDC hDCP, double A, int X, int Y)
{
  static POINT T[4];
  T[0].x = X - 90 * sin( A );
  T[0].y = Y + 90 * cos( A );
  T[1].x = X - 80 * sin( A + 1 );
  T[1].y = Y + 80 * cos( A + 1 );
  T[2].x = X - 110 * sin( A + PI );
  T[2].y = Y + 110 * cos( A + PI );
  T[3].x = X - 80 * sin( A - 1 );
  T[3].y = Y + 80 * cos( A - 1 );

  SelectObject( hDCP, GetStockObject( DC_PEN ) );
  SelectObject( hDCP, GetStockObject( DC_BRUSH ) );
  SetDCPenColor( hDCP, RGB( 0, 0, 0 ) );
  SetDCBrushColor( hDCP, RGB( 100, 100, 255 ) );

  Polygon( hDCP, T, 4 );
}



LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  RECT rc;
  static INT W, H;
  static HDC hMemDC, hDCLogo;
  static HBITMAP hBm, hBmLogo;
  static BITMAP BmLogo;
  SYSTEMTIME Time;
  static double Hour, Min, Sec;
  switch(Msg)
  {
  case WM_CREATE:
    SetTimer( hWnd, 30, 100, NULL );
    hBmLogo = LoadImage( NULL, "clockface.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    GetObject(hBmLogo, sizeof(BITMAP), &BmLogo);
    return 0;
  case WM_SIZE:
    H = HIWORD(lParam);
    W = LOWORD(lParam);
    if (hBm != NULL)
      DeleteObject(hBm);
    hDC = GetDC( hWnd );
    hBm = CreateCompatibleBitmap( hDC, W, H );
    ReleaseDC( hWnd, hDC );
    SelectObject( hMemDC, hDC );
  case WM_TIMER:
    GetLocalTime( &Time );
    Sec = (double)2 * PI * Time.wSecond / 60;
    Min = (double)2 * PI * (Time.wMinute * 60 + Time.wSecond) / (60 * 60);
    Hour = (double)2 * PI * ((Time.wHour % 12) * 60 * 60 + Time.wMinute * 60 + Time.wSecond) / (12 * 60 * 60);
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
    SetDCBrushColor( hMemDC, RGB( 255, 255, 255 ) );
    Rectangle( hMemDC, 0, 0, W, H);
    BitBlt( hMemDC, (W - BmLogo.bmWidth) / 2, (H - BmLogo.bmHeight) / 2, BmLogo.bmWidth, BmLogo.bmHeight, hDCLogo, 0, 0, SRCCOPY);
    DrawHour(hMemDC, Hour, W / 2, H / 2);
    DrawMin(hMemDC, Min, W / 2, H / 2);
    DrawSec(hMemDC, Sec, W / 2, H / 2);
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