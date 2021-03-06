// main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "CompareCircles.h"
#include "Circle.h"
#include <stdio.h>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hEdit1, hEdit2;
HWND hButtonAdd1, hButtonAdd2;
HWND hButttonMinus1, hButttonMinus2;
HWND hButtonDraw, hButtonCompare;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COMPARECIRCLES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COMPARECIRCLES));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COMPARECIRCLES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_COMPARECIRCLES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   hEdit1 = CreateWindowEx(0, TEXT("EDIT"), TEXT("5"), WS_CHILD | WS_VISIBLE | WS_BORDER, 20, 20, 60, 30, hWnd, NULL, hInst, NULL);
   hButtonAdd1 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("+"), WS_CHILD | WS_VISIBLE | WS_BORDER, 80, 20, 15, 15, hWnd, (HMENU)01, hInst, NULL);
   hButttonMinus1 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("-"), WS_CHILD | WS_VISIBLE | WS_BORDER, 80, 35, 15, 15, hWnd, (HMENU)02, hInst, NULL);

   hEdit2 = CreateWindowEx(0, TEXT("EDIT"), TEXT("5"), WS_CHILD | WS_VISIBLE | WS_BORDER, 175, 20, 60, 30, hWnd, NULL, hInst, NULL);
   hButtonAdd2 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("+"), WS_CHILD | WS_VISIBLE | WS_BORDER, 235, 20, 15, 15, hWnd, (HMENU)03, hInst, NULL);
   hButttonMinus2 = CreateWindowEx(0, TEXT("BUTTON"), TEXT("-"), WS_CHILD | WS_VISIBLE | WS_BORDER, 235, 35, 15, 15, hWnd, (HMENU)04, hInst, NULL);


   CreateWindowEx(0, TEXT("BUTTON"), TEXT("DRAW"), WS_CHILD | WS_VISIBLE | WS_BORDER, 75, 250, 60, 30, hWnd, (HMENU)5, hInst, NULL);
   CreateWindowEx(0, TEXT("BUTTON"), TEXT("COMPARE"), WS_CHILD | WS_VISIBLE | WS_BORDER, 75, 300, 90, 30, hWnd, (HMENU)6, hInst, NULL);
   CreateWindowEx(0, TEXT("BUTTON"), TEXT("RESET"), WS_CHILD | WS_VISIBLE | WS_BORDER, 75, 350, 70, 30, hWnd, (HMENU)7, hInst, NULL);
   
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
int x1 = 50, y1 = 150, x2 = 200, y2 = 150;
int thicknessPen1 = 1, thicknessPen2 = 1;

Circle c1(x1, y1, 0);
Circle c2(x2, y2, 0);

void Redraw(HWND hWnd)
{
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
}

HWND temp;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int length = 100;
	TCHAR *wcs = new TCHAR[length];
	temp = hWnd;
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case 1:
			case 2:
			{
				(wmId == 1) ? c1++ : c1--;
				wsprintf(wcs, TEXT("%d"), c1.GetR());
				SetWindowText(hEdit1, wcs);
				Redraw(hWnd);
			}
				break;
			case 3:
			case 4:
			{
				(wmId == 3) ? c2++ : c2--;
				wsprintf(wcs, TEXT("%d"), c2.GetR());
				SetWindowText(hEdit2, wcs);
				Redraw(hWnd);
			}
				break;
			case 5:
			{
				Redraw(hWnd);
			}
				break;
			case 6:
			{
				if (c1 == c2)
				{
					thicknessPen1 = 2;
					thicknessPen2 = 2;
				}
				else if (c1 > c2)
				{
					thicknessPen1 = 2;
					thicknessPen2 = 1;
				}
				else
				{
					thicknessPen1 = 1;
					thicknessPen2 = 2;
				}
				Redraw(hWnd);
			}
				break;
			case 7:
			{
				thicknessPen1 = 1;
				thicknessPen2 = 1;
				
				c1.SetR(5);
				c2.SetR(5);

				Redraw(hWnd);
			}
				break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_PAINT:
        {
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			HPEN pen = CreatePen(PS_SOLID, thicknessPen1, RGB(0, 0, 0));
			SelectObject(hdc, pen);

			GetWindowText(hEdit1, wcs, length);
			c1.SetR(_wtoi(wcs));
			
			Draw(c1, hdc);

			pen = CreatePen(PS_SOLID, thicknessPen2, RGB(0, 0, 0));
			SelectObject(hdc, pen);

			GetWindowText(hEdit2, wcs, length);
			c2.SetR(_wtoi(wcs));

			Draw(c2, hdc);

			UpdateWindow(hWnd);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK subEditProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_RETURN) Redraw(temp);
	default:
		return CallWindowProc(WndProc, hWnd, msg, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
