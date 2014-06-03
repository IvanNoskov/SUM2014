/* NI01, 02.06.2014 */

#include <stdlib.h>
#include <math.h>       

#include <windows.h>

#define WND_CLASS_NAME "my_fist's_window_class"
#define ROTATION_SPEEED 2


int small_Brother (POINT cur, RECT rc, HWND hWnd)
{
  int x, y, x1, y1, x2, y2;
  HDC hDC;
  double L;

  x = cur.x;
  y = cur.y;

  hDC = GetDC( hWnd );
  SelectObject( hDC, GetStockObject( DC_PEN ) );
  SelectObject( hDC, GetStockObject( DC_BRUSH ) );
  SetDCPenColor( hDC, RGB( 0, 0, 0 ) );

  x1 = (int)(rc.right * (rand() % 300) / 300.0);
  y1 = (int)(rc.bottom * (rand() % 300) / 300.0);
  SetDCBrushColor( hDC, RGB( 255, 255, 255 ) );
  Ellipse( hDC, x1 - 30, y1 - 30, x1 + 30, y1 + 30);
  if ((L = sqrt( (x1 - x) * (x1 - x) + (y1 - y) * (y1 - y) )) <= 20)
  {
    x2 = x;
    y2 = y;
    SetDCBrushColor( hDC, RGB( 0, 0, 0 ) );
    Ellipse( hDC, x2 - 10, y2 - 10, x2 + 10, y2 + 10); 
  }
  else
  {
    x2 = (int)(20 * (x - x1) / L);
    y2 = (int)(20 * (y - y1) / L);
    SetDCBrushColor( hDC, RGB( 0, 0, 0 ) );
    Ellipse( hDC, x1 + x2 - 10, y1 + y2 - 10, x1 + x2 + 10, y1 + y2 + 10); 
  }

  ReleaseDC( hWnd, hDC );

  return 0;
}

int big_Brother (POINT Cur, RECT rc, HWND hWnd)
{
  HDC hDC;
  hDC = GetDC( hWnd );
  SelectObject( hDC, GetStockObject( DC_PEN ) );
  SelectObject( hDC, GetStockObject( DC_BRUSH ) );
  SetDCPenColor( hDC, RGB( 0, 0, 0 ) );
  SetDCBrushColor( hDC, RGB( 0, 0, 0 ) );
  Ellipse( hDC, Cur.x - (rc.right / 2), Cur.y - (rc.bottom / 2), Cur.x + (rc.right / 2), Cur.y + (rc.bottom / 2)); 
  ReleaseDC( hWnd, hDC );

  return 0;
}

LRESULT CALLBACK MyWindowFunc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam )
{
  HDC hDC;
  PAINTSTRUCT ps;
  INT i;
  static POINT Cur;
  POINT Cur_real;
  static int j = 30, h = 50;
  double L;
  int x, y, x1, y1, x2, y2;
  RECT rc;
  static BOOL Follow = TRUE;
  GetClientRect( hWnd, &rc );
  switch(Msg)
  {
  case WM_CREATE:
    GetCursorPos( &Cur );
    ScreenToClient( hWnd, &Cur );
    SetTimer( hWnd, 30, 9, NULL);
    return 0;
  case WM_CLOSE:
    if (IDNO == (i = MessageBox(hWnd, "Are you shure?\n(if you don't shure we will repaint our points)", "EXIT", MB_YESNOCANCEL | MB_ICONQUESTION)))
    {
      j = rand() % 256;
      srand(h);
      h = rand() * rand() - rand() + rand();
      InvalidateRect( hWnd, NULL, TRUE);
      return 0;
    }
    if (i == IDCANCEL)
      return 0;
    break;
  case WM_DESTROY:
    KillTimer( hWnd, 30 );
    PostQuitMessage(0);
    return 0;
  case WM_LBUTTONDOWN:
    Follow = FALSE;
    break;
  case WM_LBUTTONUP:
    Follow = TRUE;
    break;       
  case WM_PAINT:
 /*   Cur2.x = rc.right / 2;
    Cur2.y = rc.bottom / 2;
    big_Brother( Cur2, rc, hWnd);*/
    hDC = BeginPaint( hWnd, &ps );
    SetPixel( hDC, 15, 15, RGB( 0, 0, 0 ) );
    SelectObject( hDC, GetStockObject( DC_PEN ) );
    SelectObject( hDC, GetStockObject( DC_BRUSH ) );
    SetDCPenColor( hDC, RGB( 100, 100, 100 ) );
    SetDCBrushColor( hDC, RGB( 0, 0, 0 ) );
    srand(j);
    for (i = 0; i < 10; i++)
    {
      SetDCPenColor( hDC, RGB( (rand() * i) % 256, rand() % 256, rand() % 256 ) );
      SetDCBrushColor( hDC, RGB( rand() % 256, (rand() * i) % 256, rand() % 256 ) );
      Ellipse( hDC, (int)(rc.right * (rand() % 256) / 256.0),
                    (int)(rc.bottom * (rand() % 256) / 256.0), 
                    (int)(rc.right * (rand() % 256) / 256.0), 
                    (int)(rc.bottom * (rand() % 256) / 256.0) );
    }                 
    EndPaint( hWnd, &ps );
    break;
  case WM_TIMER:
    srand(h);
    GetCursorPos( &Cur_real );
    ScreenToClient( hWnd, &Cur_real );
    if(Follow)
    {
      if ((Cur.x - Cur_real.x) <= ROTATION_SPEEED && (Cur_real.x - Cur.x) <= ROTATION_SPEEED)
        Cur.x = Cur_real.x;
      else if (Cur.x < Cur_real.x)
        Cur.x += ROTATION_SPEEED;
      else
        Cur.x -= ROTATION_SPEEED;
      if ((Cur.y - Cur_real.y) <= ROTATION_SPEEED && (Cur_real.y - Cur.y) <= ROTATION_SPEEED)
        Cur.y = Cur_real.y;
      else if (Cur.y < Cur_real.y)
        Cur.y += ROTATION_SPEEED;
      else
        Cur.y -= ROTATION_SPEEED;
    }
    for (i = 0; i < 150; i++)
    {
      small_Brother(Cur, rc, hWnd); 
    }
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