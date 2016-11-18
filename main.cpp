/*
* hello_child_window.cpp
* Copyright (C) 2016.11, jinfeng <kavin.zhuang@gmail.com>
*/

/* Windows API */
#include <Windows.h>

/* ISO C99 */
#include <stdint.h>
#include <stdio.h>
#include <time.h>

#pragma warning(disable:4996)

#define TRACE(fmt, ...) printf("[%s:%d] " fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define PASSWORD_MAX 13

static TCHAR winclass[] = TEXT("mainwindow");
static TCHAR wintitle[] = TEXT("password");
static int win_left = 500;
static int win_top = 200;
static int win_width = 260;
static int win_height = 80;

static HWND hwndEdit = NULL;
static HWND hwndButton = NULL;

static char valid_chars[] = {
  '*', '#', '@', '=',
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'
};

static int valid_chars_size;

wchar_t password[PASSWORD_MAX];

static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  HDC         hdc;
  PAINTSTRUCT ps;

  int num;

  switch (uMsg)
  {
  case WM_CREATE:
    valid_chars_size = sizeof(valid_chars);
    srand((unsigned)time(NULL));
    hwndEdit = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 30, 10, 150, 20, hwnd, NULL, NULL, NULL);
    hwndButton = CreateWindow(L"button", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 190, 10, 20, 20, hwnd, NULL, NULL, NULL);
    return 0;
  case WM_COMMAND:
    if ((HWND)lParam == hwndButton) {
      if (BN_CLICKED == HIWORD(wParam)) {
        for (int i = 0; i < PASSWORD_MAX; i++) {
          num = rand() % valid_chars_size;
          password[i] = valid_chars[num];
        }
        password[PASSWORD_MAX-1] = '\0';
        wprintf(L"%s\n", password);
        SetWindowText(hwndEdit, password);
      }
    }
    return 0;
  case WM_PAINT:
    hdc = BeginPaint(hwnd, &ps);

    EndPaint(hwnd, &ps);
    return 0;
  case WM_DESTROY:
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static int register_window(HINSTANCE hInstance)
{
  WNDCLASSEX wce = { 0 };

  wce.cbSize = sizeof(wce);
  wce.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wce.hCursor = LoadCursor(NULL, IDC_ARROW);
  wce.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wce.hInstance = hInstance;
  wce.lpfnWndProc = WndProc;
  wce.lpszClassName = winclass;
  wce.style = CS_HREDRAW | CS_VREDRAW;
  if (!RegisterClassEx(&wce)) {
    return 1;
  }

  return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
  MSG msg;
  HWND hwnd;

  /* Console for DEBUG */
  //AllocConsole();
  //freopen("CONOUT$", "w", stdout);

  printf("Hello World\n");

  /* Windows Register */
  if (0 != register_window(hInstance)) {
    return 1;
  }

  /* Create Windows & Do Message Loop */
  hwnd = CreateWindowEx(0, winclass, wintitle, WS_OVERLAPPEDWINDOW,
    win_left, win_top, win_width, win_height,
    NULL, NULL, hInstance, NULL);
  if (hwnd == NULL) {
    return 1;
  }

  ShowWindow(hwnd, nShowCmd);
  UpdateWindow(hwnd);

  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return msg.wParam;
}
