/* NI01 */

#include <stdlib.h>     
#include <stdio.h>
#include <windows.h>

#include "DEF.H"
#include "IMAGE.H"
#include "VCT.H"

#define WND_CLASS_NAME "my_fist's_window_class"

/*VOID FlipFullScreen( HWND hWnd )
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
  static INT W, H;
  static HDC hMemDC, hDCLogo;
  static HBITMAP hBm, hBmLogo;
  static BITMAP BmLogo;
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
} */

void PrintMatrix (CHAR *Buf, MATRIX M)
{
  sprintf( Buf, " / %6.3lf %6.3lf %6.3lf %6.3lf \\\n |  %6.3lf %6.3lf %6.3lf %6.3lf  |\n|  %6.3lf %6.3lf %6.3lf %6.3lf  |\n\\ %6.3lf %6.3lf %6.3lf %6.3lf /\n", M.A[0][0], M.A[0][1], M.A[0][2], M.A[0][3], M.A[1][0], M.A[1][1], M.A[1][2], M.A[1][3], M.A[2][0], M.A[2][1], M.A[2][2], M.A[2][3], M.A[3][0], M.A[3][1], M.A[3][2], M.A[3][3]);

}

INT WINAPI WinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstanse, CHAR *CmdLine, INT ShowCmd )
{
  MATRIX M1, M2, M3;
  VEC V1, V2;
  static char BUF[300];
  M1 = MatrIdenity( );
  PrintMatrix( BUF, M1 );
  MessageBox( NULL, BUF, "I matric ( M1 )", MB_OK | MB_ICONINFORMATION );
  M1 = MatrTranslate( 1, 3, 25 );
  PrintMatrix( BUF, M1 );
  MessageBox( NULL, BUF, "Transpose matric (x = 1, y = 3, z = 25) ( M1 )", MB_OK | MB_ICONINFORMATION );
  M2 = MatrScale(2, 4, 8);
  PrintMatrix( BUF, M2 );
  MessageBox( NULL, BUF, "Scale matric (x = 2, y = 4, z = 8) ( M2 )", MB_OK | MB_ICONINFORMATION );
  M3 = MatrMulMatr( M1, M2 );
  PrintMatrix( BUF, M3 );
  MessageBox( NULL, BUF, " M1 * M2 ( M3 )", MB_OK | MB_ICONINFORMATION );
  M3 = MatrMulMatr( M2, M1 );
  PrintMatrix( BUF, M3 );
  MessageBox( NULL, BUF, " M2 * M1 ( M3 )", MB_OK | MB_ICONINFORMATION );
  M1 = MatrTranspose( M3 );
  PrintMatrix( BUF, M1 );
  MessageBox( NULL, BUF, " (M3)t ( M1 )", MB_OK | MB_ICONINFORMATION );
  M2 = MatrInverse( M3 );
  PrintMatrix( BUF, M2 );
  MessageBox( NULL, BUF, " (M3)^-1 ( M2 )", MB_OK | MB_ICONINFORMATION );
  M1 = MatrMulMatr( M3, M2 );
  PrintMatrix( BUF, M1 );
  MessageBox( NULL, BUF, " M3 * M2 ( M1 )", MB_OK | MB_ICONINFORMATION );
  M1 = MatrMulMatr( M2, M3 );
  PrintMatrix( BUF, M1 );
  MessageBox( NULL, BUF, " M2 * M3 ( M1 )", MB_OK | MB_ICONINFORMATION );

}