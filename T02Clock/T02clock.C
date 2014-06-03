/* NI01, 03.06.2014 */

#include <stdlib.h>
#include <math.h>       

#include <windows.h>

#define WND_CLASS_NAME "my_fist's_window_class"
#define PI (3,1415926535897)

void DrawHour (HDC hDCP, double A, int X, int Y)
{
  static POINT T[4];
  T[0].x = 50 * cos( A );
  T[0].y = -50 * sin( A );
  T[1].x = 40 * cos( A + 1 );
  T[1].y = -40 * sin( A + 1 );
  T[2].x = 70 * cos( A + PI );
  T[2].y = -70 * sin( A + PI );
  T[3].x = 40 * cos( A - 1 );
  T[3].y = -40 * sin( A - 1 );

  SelectObject( hDCP, GetStockObject( DC_PEN ) );
  SelectObject( hDCP, GetStockObject( DC_BRUSH ) );
  SetDCPenColor( hDCP, RGB( 0, 0, 0 ) );
  SetDCBrushColor( hDCP, RGB( 255, 100, 100 ) );

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
  SYSTEMTIME Time;
  static double Hour, Min, Sec;
  switch(Msg)
  {
  case WM_CREATE:
    hDC = GetDC( hWnd );
    hMemDC = CreateCompatibleBitmap( hDC, W, H);
    ReleaseDC ( hWnd, hDC );
    SetTimer( hWnd, 30, 100, NULL );
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
    GetSystemTime( &Time );
    Sec = 60.0 * 2 * PI / Time.wSecond;
    Min = 60 * 60.0 * 2 * PI / (60 * Time.wMinute + Time.wSecond);
    Hour = 60 * 60 * 60.0 * 2 * PI / (3600 * Time.wHour + 60 * Time.wMinute + Time.wSecond);
    InvalidateRect( hWnd, NULL, FALSE);
  case WM_CLOSE:
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  case WM_PAINT:
    hDC = BeginPaint( hWnd, &ps );
    DrawHour(hMemDC, Hour, W / 2, H / 2);
    BitBlt(hDC, 0, 0, W, H, hMemDC, 0, 0, SRCCOPY);
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