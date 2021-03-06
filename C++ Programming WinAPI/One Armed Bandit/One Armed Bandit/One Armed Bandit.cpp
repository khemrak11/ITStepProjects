// One Armed Bandit.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "One Armed Bandit.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void StartStopTimer(const HWND &hWnd);
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
    LoadStringW(hInstance, IDC_ONEARMEDBANDIT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ONEARMEDBANDIT));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ONEARMEDBANDIT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ONEARMEDBANDIT);
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

   CreateWindow(TEXT("BUTTON"), TEXT("START"), WS_VISIBLE | WS_CHILD, 350, 150, 70, 30, hWnd, (HMENU)1, hInst, NULL);

   CreateWindow(TEXT("STATIC"), TEXT("IDB_APPLE \t130 \nIDB_BANANA \t131 \nIDB_CHERRY \t132 \nIDB_GRAPE \t133 \nIDB_ORANGE \t134 \nIDB_STRAWBERRY \t135 \nIDB_WATERMELON \t136"), WS_VISIBLE | WS_CHILD, 390, 200, 200, 115, hWnd, 0, hInst, NULL);

   SetWindowPos(hWnd, NULL, 0, 0, 700, 800, SWP_SHOWWINDOW);

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
const int row = 7;
const int column = 3;

int pos[column][row];
int acceleration = 50;
float timeRan = 0;
//speed = (e ^ (- 1 * timeRan) (while speed > 0) pos += speed

int amountPics = IDB_WATERMELON - IDB_APPLE + 1;
int *ID = new int[amountPics];
CircularQueue IDPos(column * row);

int interval = 100;

bool isRunning = false;

void DrawImage(HDC hdc, int IDB, int x, int y)
{
	HBITMAP hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB));
	if (hBitMap != NULL)
	{
		HDC hdcBitmap = CreateCompatibleDC(hdc);
		BITMAP bitmap;
		GetObject(hBitMap, sizeof(bitmap), &bitmap);

		SelectObject(hdcBitmap, hBitMap);
		BitBlt(hdc, x, y, bitmap.bmWidth, bitmap.bmHeight, hdcBitmap, 0, 0, SRCCOPY);

		DeleteDC(hdcBitmap);
	}
	DeleteObject(hBitMap);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		srand(time(NULL));
		for (int i = IDB_APPLE; i <= IDB_WATERMELON; i++)
		{
			ID[i - IDB_APPLE] = i;
		}
		for (int i = 0; i < column; i++)
		{
			for (int j = 0; j < row; j++)
			{
				pos[i][j] = j * 100;
				IDPos.Push(ID[rand() % amountPics]);
			}
		}
	}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case 1:
				StartStopTimer(hWnd);
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
	case WM_TIMER:
	{
		timeRan += 1 /((float)interval);
		
		float *speed = new float[column];

		for (int i = 0; i < column; i++) speed[i] = acceleration * (column - i) * (exp((-1) * (timeRan)));

		if (speed[0] <= 1 && speed[1] <= 1 && speed[2] <= 1)
		{
			StartStopTimer(hWnd);
		}

		for (int i = 0; i < column; i++)
		{
			for (int j = 0; j < row; j++)
			{
				if (speed[i] <= 1) break;

				pos[i][j] += speed[i];
				if (pos[i][j] >= 700)
				{
					pos[i][j] %= 700;
					//IDPos.Shift();
				}
			}
		}
		InvalidateRect(hWnd, NULL, true);
		
		delete[] speed;
	}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			for (int i = 0; i < column; i++)
			{
				for (int j = 0; j < row; j++)
				{
					if (150 <= pos[i][j] && pos[i][j] <= 450)
					{
						DrawImage(hdc, IDPos.GetValue(j * column + i), 10 + (i * 100) , pos[i][j]);
					}
				}
			}

			MoveToEx(hdc, 330, 350, NULL);
			LineTo(hdc, 360, 350);
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

void StartStopTimer(const HWND &hWnd)
{
	if (!isRunning)
	{
		SetTimer(hWnd, 1, interval, NULL);
		isRunning = true;
	}
	else
	{
		KillTimer(hWnd, 1);
		isRunning = false;

		int *type = new int[column];

		int *position = new int[column];
		int closest = 200;
		for (int i = 0; i < column; i++)
		{
			closest = 250;
			for (int j = 0; j < row; j++)
			{
				if (closest <= pos[i][j] && pos[i][j] < 350)
				{
					closest = pos[i][j];
					position[i] = j;
				}
			}
		}

		for (int i = 0; i < column; i++)
		{
			type[i] = IDPos.GetValue(position[i] * column + i);
		}

		TCHAR *wcs = new TCHAR[30];
		wsprintf(wcs, TEXT("%d - %d - %d"), type[0], type[1], type[2]);
		SetWindowText(hWnd, wcs);

		delete[] wcs;
		delete[] type;
		delete[] position;
	}
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