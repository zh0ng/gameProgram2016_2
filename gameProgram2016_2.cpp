// gameProgram2016_2.cpp : Defines the entry point for the application.
//
#include <stdlib.h>
#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

const int REFRESH_INTERVAL = 5;
const int WIN_WIDTH = 615;
const int WIN_HEIGHT = 365;
const int NUM_SNOW = 20;
const int SPEED_X = 1;
int SPEED_Y = 10;

HDC	hdc, hdcBuf, hdcBody;
HBITMAP backgroundBmp, snowBmp;
RECT	rect;

struct Point
{
	int x;
	int y;
};
Point snows[NUM_SNOW];

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void initSnows();
void print();
void snowDrop();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMEPROGRAM2016_2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GAMEPROGRAM2016_2);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_GAMEPROGRAM2016_2);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_GAMEPROGRAM2016_2;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		                CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

   
	MoveWindow(hWnd, 10, 10, WIN_WIDTH, WIN_HEIGHT, true);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
   
	hdc = GetDC(hWnd);
	hdcBuf = CreateCompatibleDC(hdc);
   
	hdcBody = CreateCompatibleDC(hdc);
	HBITMAP bmp = CreateCompatibleBitmap(hdc, WIN_WIDTH, WIN_HEIGHT);
   
	SelectObject(hdcBuf, bmp);
   
    backgroundBmp = (HBITMAP)LoadImage(NULL, "background.bmp", IMAGE_BITMAP, rect.right, rect.bottom, LR_LOADFROMFILE);
    snowBmp = (HBITMAP)LoadImage(NULL, "snow.bmp", IMAGE_BITMAP, 20, 20, LR_LOADFROMFILE);
      
	SetTimer(hWnd, 0, REFRESH_INTERVAL, NULL);

	initSnows();

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_TIMER: // 定时器消息
			print();
			snowDrop();
			break;
		case WM_KEYDOWN:
			if (wParam == VK_UP)
				SPEED_Y++;
			else if (wParam == VK_DOWN)
				SPEED_Y--;
			if (SPEED_Y < 0)
				SPEED_Y = 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

// 初始化雪花
void initSnows()
{
	srand((unsigned)(GetTickCount()));
	for (int i = 0; i < NUM_SNOW; i++)
	{
		snows[i].x = rand() % WIN_WIDTH;
		snows[i].y = rand() % WIN_HEIGHT;
	}
}

// 雪花飘落
void snowDrop()
{
	for (int i = 0; i < NUM_SNOW; i++)
	{
		snows[i].x += (rand() % 2 ? 1 : -1) * SPEED_X;
		snows[i].y += SPEED_Y;
		if (snows[i].y >= WIN_HEIGHT)
		{
			snows[i].x = rand() % WIN_WIDTH;
			snows[i].y = -20;
		}
	}
}

void print()
{
	SelectObject(hdcBody, backgroundBmp);
	BitBlt(hdcBuf, 0, 0, WIN_WIDTH, WIN_WIDTH, hdcBody, 0, 0, SRCCOPY);

	for (int i = 0; i < NUM_SNOW; i++)
	{	
		Sleep(1);
		
		SelectObject(hdcBody, snowBmp);
		BitBlt(hdcBuf, snows[i].x, snows[i].y, 20, 20, hdcBody, 0, 0, SRCPAINT);
	}
	
	
	//将mdc中的全部内容贴到hdc中
	BitBlt(hdc, 0, 0, WIN_WIDTH, WIN_WIDTH, hdcBuf, 0, 0, SRCCOPY);
}