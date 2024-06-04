// CPP_Lab6_Toolbar.cpp : Defines the entry point for the application.
//

#include <vector>
#include <string>
#include <windows.h>
#include <commctrl.h>
#include "framework.h"
#include "CPP_Lab6_Toolbar.h"
#include "Shapes.h"
#include "resource.h"

constexpr auto MAX_LOADSTRING = 100;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

const int N = 123;
const int studentNumber = 23;

ShapeType currentShapeType = ShapeType::SHAPE_POINT;

Shape shapes[N];
int shapeCount = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void AddToolbar(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CPPLAB5VECTORSHAPEEDITOR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CPPLAB5VECTORSHAPEEDITOR));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CPPLAB5VECTORSHAPEEDITOR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CPPLAB5VECTORSHAPEEDITOR);
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

   ShowWindow(hWnd, nCmdShow);
   SetWindowTextA(hWnd, "Графічний редактор");
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

void AddToolbar(HWND hWnd) {
    TBBUTTON tbb[4];
    ZeroMemory(tbb, sizeof(tbb));

    tbb[0].iBitmap = 0;
    tbb[0].fsState = TBSTATE_ENABLED;
    tbb[0].fsStyle = TBSTYLE_BUTTON;
    tbb[0].idCommand = ID_OBJECTS_POINT;

    tbb[1].iBitmap = 1;
    tbb[1].fsState = TBSTATE_ENABLED;
    tbb[1].fsStyle = TBSTYLE_BUTTON;
    tbb[1].idCommand = ID_OBJECTS_LINE;

    tbb[2].iBitmap = 2;
    tbb[2].fsState = TBSTATE_ENABLED;
    tbb[2].fsStyle = TBSTYLE_BUTTON;
    tbb[2].idCommand = ID_OBJECTS_RECTANGLE;

    tbb[3].iBitmap = 3;
    tbb[3].fsState = TBSTATE_ENABLED;
    tbb[3].fsStyle = TBSTYLE_BUTTON;
    tbb[3].idCommand = ID_OBJECTS_ELLIPSE;

    HWND hToolbar = CreateToolbarEx(hWnd,
        WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS | CCS_TOP,
        ID_TOOLBAR,
        4,
        hInst,
        IDB_BITMAP1,
        tbb,
        4,
        24, 24, 24, 24,
        sizeof(TBBUTTON));
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static POINT ptStart;
    static POINT ptEnd;
    static bool isDrawing = false;
    static HPEN hPen;
    static std::wstring currentShapeName;
    switch (message)
    {
    case WM_CREATE: {
        AddToolbar(hWnd);
    }
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case ID_OBJECTS_POINT:
                currentShapeType = ShapeType::SHAPE_POINT;
                break;
            case ID_OBJECTS_LINE:
                currentShapeType = ShapeType::SHAPE_LINE;
                break;
            case ID_OBJECTS_RECTANGLE:
                currentShapeType = ShapeType::SHAPE_RECTANGLE;
                break;
            case ID_OBJECTS_ELLIPSE:
                currentShapeType = ShapeType::SHAPE_ELLIPSE;
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
            switch (currentShapeType) {
            case ShapeType::SHAPE_POINT: currentShapeName = L" (Крапка)"; break;
            case ShapeType::SHAPE_LINE: currentShapeName = L" (Лінія)"; break;
            case ShapeType::SHAPE_RECTANGLE: currentShapeName = L" (Прямокутник)"; break;
            case ShapeType::SHAPE_ELLIPSE: currentShapeName = L" (Еліпс)"; break;
            default: currentShapeName = L""; break;
            }
            SetWindowText(hWnd, (L"Графічний редактор" + currentShapeName).c_str());
            break;
        }
    case WM_LBUTTONDOWN: {
        ptStart.x = LOWORD(lParam);
        ptStart.y = HIWORD(lParam);
        isDrawing = true;
        break;
    }
    case WM_MOUSEMOVE: {
        if (isDrawing) {
            HDC hdc = GetDC(hWnd);
            SetROP2(hdc, R2_NOTXORPEN);
            hPen = CreatePen(PS_DOT, 1, 0);
            SelectObject(hdc, hPen);

            if (currentShapeType == ShapeType::SHAPE_RECTANGLE ||
                currentShapeType == ShapeType::SHAPE_ELLIPSE) {
                Rectangle(hdc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y); // Erase previous
            }
            else if (currentShapeType == ShapeType::SHAPE_LINE) {
                MoveToEx(hdc, ptStart.x, ptStart.y, nullptr);
                LineTo(hdc, ptEnd.x, ptEnd.y); // Erase previous
            }

            ptEnd.x = LOWORD(lParam);
            ptEnd.y = HIWORD(lParam);

            if (currentShapeType == ShapeType::SHAPE_RECTANGLE ||
                currentShapeType == ShapeType::SHAPE_ELLIPSE) {
                Rectangle(hdc, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y); // Draw new
            }
            else if (currentShapeType == ShapeType::SHAPE_LINE) {
                MoveToEx(hdc, ptStart.x, ptStart.y, NULL);
                LineTo(hdc, ptEnd.x, ptEnd.y); // Draw new
            }


            ReleaseDC(hWnd, hdc);
        }
        break;
    }
    case WM_LBUTTONUP: {
        if (isDrawing) {
            ptEnd.x = LOWORD(lParam);
            ptEnd.y = HIWORD(lParam);
            isDrawing = false;

            if (shapeCount < N) {
                shapes[shapeCount++] = { currentShapeType, ptStart, ptEnd, GetShapeColor(studentNumber) };
            }

            InvalidateRect(hWnd, nullptr, TRUE);
        }
        break;
    }
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        DrawShapes(hdc, shapes, shapeCount);

        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY: {
        DeleteObject(hPen);
        PostQuitMessage(0);
        break;
    }

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
