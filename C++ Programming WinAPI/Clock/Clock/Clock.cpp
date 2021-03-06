// Clock.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <math.h>
#include "Clock.h"
#include <time.h>

#define MAX_LOADSTRING 100
#define PI 3.14159265

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
void TimerControl(const WPARAM &wParam, const HWND &hWnd);
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
    LoadStringW(hInstance, IDC_CLOCK, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLOCK));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLOCK));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLOCK);
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

void TimerControl(const WPARAM &wParam, const HWND &hWnd);

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   TimerControl(VK_RETURN, hWnd);

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


void Redraw(HWND hWnd)
{
	RedrawWindow(hWnd, NULL, NULL, RDW_INVALIDATE | RDW_ERASE);
}

const time_t current = time(NULL);
struct tm *tm_struct = localtime(&current);

double originX = 200;
double originY = 200;
double radius = 90;

double labelDistFromOrigin = radius * 0.9;

double secondHandLength = radius * 0.8;
double minuteHandLength = radius * 0.5;
double hourHandLength = radius * 0.3;

double secondHandAngle = (-PI/2) + (PI/30) * tm_struct->tm_sec;
double minuteHandAngle = (-PI / 2) + (PI / 30) * tm_struct->tm_min;
double hourHandAngle = (-PI / 2) + (PI / 6) * tm_struct->tm_hour;

bool isRunning = false;

void TimerControl(const WPARAM &wParam, const HWND &hWnd)
{
	if (wParam == VK_RETURN)
	{
		if (!isRunning)
		{
			SetTimer(hWnd, 1, 1000, NULL);
			SetWindowText(hWnd, TEXT("START"));
			isRunning = true;
		}
		else
		{
			KillTimer(hWnd, 1);
			SetWindowText(hWnd, TEXT("STOP"));
			isRunning = false;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
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
	case WM_CHAR:
		TimerControl(wParam, hWnd);
		break;
	case WM_TIMER:
		secondHandAngle += 2 * PI / 60;
		minuteHandAngle += 2 * PI / 3600;
		hourHandAngle += 2 * PI / 216000;

		Redraw(hWnd);
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			
			//Getting w/h of window
			RECT *rect = new RECT();
			GetClientRect(hWnd, rect);
			originX = (double)(rect->right)/2;
			originY = (double)(rect->bottom) / 2;


			//Resetting value if window is scaled
			radius = (rect->right > rect->bottom) ? (double)(rect->bottom) * 1/3 : (double)(rect->right) * 1/3;
			labelDistFromOrigin = radius * 0.9;
			secondHandLength = radius * 0.8;
			minuteHandLength = radius * 0.5;
			hourHandLength = radius * 0.3;


			Ellipse(hdc, (int)(originX - radius), (int)(originY - radius), (int)(originX + radius), (int)(originY + radius));


			//Drawing the 3 hands of the clock
			MoveToEx(hdc, originX, originY, NULL);
			LineTo(hdc, originX + secondHandLength * cos(secondHandAngle), originY + secondHandLength * sin(secondHandAngle));
			
			HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
			SelectObject(hdc, pen);
			MoveToEx(hdc, originX, originY, NULL);
			LineTo(hdc, originX + minuteHandLength * cos(minuteHandAngle), originY + minuteHandLength * sin(minuteHandAngle));
			
			pen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
			SelectObject(hdc, pen);
			MoveToEx(hdc, originX, originY, NULL);
			LineTo(hdc, originX + hourHandLength * cos(hourHandAngle), originY + hourHandLength * sin(hourHandAngle));


			//Drawing the labels
			pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
			SelectObject(hdc, pen);

			double tempAngle = -PI / 2;
			TCHAR *wcs = new TCHAR[5];
			PSIZE pSize = new SIZE;
			for (int i = 1; i <= 12; i++)
			{
				tempAngle += (PI / 30) * 5;
				if (i % 3 == 0)
				{
					wsprintf(wcs, TEXT("%d"), i);
					GetTextExtentPoint32(hdc, wcs, _tcslen(wcs), pSize);

					if (i == 3) TextOut(hdc, originX + labelDistFromOrigin * cos(tempAngle), originY + labelDistFromOrigin * sin(tempAngle) - pSize->cy/2, wcs, _tcslen(wcs));
					else if (i == 6) TextOut(hdc, originX + labelDistFromOrigin * cos(tempAngle) - pSize->cx / 2, originY + labelDistFromOrigin * sin(tempAngle), wcs, _tcslen(wcs));
					else if (i == 9) TextOut(hdc, originX + labelDistFromOrigin * cos(tempAngle), originY + labelDistFromOrigin * sin(tempAngle) - pSize->cy / 2, wcs, _tcslen(wcs));
					else if (i == 12) TextOut(hdc, originX + labelDistFromOrigin * cos(tempAngle) - pSize->cx / 2, originY + labelDistFromOrigin * sin(tempAngle), wcs, _tcslen(wcs));
				}
				else
				{
					MoveToEx(hdc, originX + labelDistFromOrigin * cos(tempAngle), originY + labelDistFromOrigin * sin(tempAngle), NULL);
					LineTo(hdc, originX + (labelDistFromOrigin + 10) * cos(tempAngle), originY + (labelDistFromOrigin + 10) * sin(tempAngle));
				}
			}

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
