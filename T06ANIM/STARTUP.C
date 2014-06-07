/* NI01, STARTUP.C */

#include <stdio.h>
#include <math.h>
#include <time.h>

#include "units.h"

#define WND_CLASS_NAME "Window Class Name"

/* MainWindowFunc preliminary determination */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam );

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
  INT i;

  /* Registration */
  wc.style = CS_HREDRAW | CS_VREDRAW;
  wc.cbClsExtra = 0; /* additive class bites number */
  wc.cbWndExtra = 0; /* additive window bites number */
  wc.hbrBackground = (HBRUSH)COLOR_WINDOW; /* Фоновый цвет - выбранный в системе */
  wc.hCursor = LoadCursor(NULL, IDC_HAND);
  wc.hIcon = LoadIcon(NULL, IDI_ERROR);
  wc.lpszMenuName = NULL;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = MainWindowFunc;
  wc.lpszClassName = WND_CLASS_NAME;

  /* Регистрируем класс */
  if (!RegisterClass(&wc))
  {
    MessageBox(NULL, "Error register window class", "Error", MB_ICONERROR | MB_OK);
    return 0;
  }

  /* Создание окна */
  hWnd = CreateWindow(WND_CLASS_NAME, "First Window Sample",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, /* Позиция окна (x, y) - по умолчанию */
    CW_USEDEFAULT, CW_USEDEFAULT, /* Размеры окна (w, h) - по умолчанию */
    NULL,                         /* Дескриптор родительского окна */
    NULL,                         /* Дескриптор загруженного меню */
    hInstance,                    /* Дескриптор приложения */
    NULL);                        /* Указатель на дополнительные параметры */

  ShowWindow(hWnd, ShowCmd);
  UpdateWindow(hWnd);

  /*** Добавление объектов ***/
  for (i = 0; i < 30 * 30; i++)
    IN1_AnimAddUnit(IN1_CowUnitCreate());
  IN1_AnimAddUnit(IN1_FPSDisplayUnitCreate());

  /* Запуск цикла обработки сообщений */
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    /* Передача сообщений в функцию окна */
    DispatchMessage(&msg);
  }

  return msg.wParam;
} /* End of 'WinMain' function */

/* Функция обработки сообщения окна.
 * АРГУМЕНТЫ:
 *   - дескриптор окна:
 *       HWND hWnd;
 *   - номер сообщения:
 *       UINT Msg;
 *   - параметр сообшения ('word parameter'):
 *       WPARAM wParam;
 *   - параметр сообшения ('long parameter'):
 *       LPARAM lParam;
 * ВОЗВРАЩАЕМОЕ ЗНАЧЕНИЕ:
 *   (LRESULT) - в зависимости от сообщения.
 */
LRESULT CALLBACK MainWindowFunc( HWND hWnd, UINT Msg,
                                 WPARAM wParam, LPARAM lParam )
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
    IN1_AnimInit(hWnd);
    return 0;
  case WM_SIZE:
    IN1_AnimResize(LOWORD(lParam), HIWORD(lParam));
  case WM_TIMER:
    IN1_AnimRender();
    IN1_AnimCopyFrame();
    return 0;
  case WM_CHAR:
    switch ((CHAR)wParam)
    {
    case 27:
      DestroyWindow(hWnd);
      return 0;
    case 'f':
      IN1_AnimFlipFullScreen();
      return 0;
    case 'p':
      IN1_AnimSetPause(pause = !pause);
      return 0;
    }
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
} /* End of 'MyWindowFunc' function */

/* END OF 'STARTUP.C' FILE */
