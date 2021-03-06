// 7_lab.cpp: Определяет точку входа для приложения.
//

#include "stdafx.h"
#include "7_lab.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void DrawBitmap(HWND hWnd, BITMAP bm, bool needToDrawImage);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY7LAB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY7LAB));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY7LAB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY7LAB);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, (LPCWSTR)"Лабораторная работа №7", WS_OVERLAPPED,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   static HWND hClearButton = CreateWindow((LPCSTR)"button", (LPCSTR)"Clear", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 600, 200, 30, hWnd, (HMENU)101, hInst, 0);
   static HWND hCloseButton = CreateWindow((LPCSTR)"button", (LPCSTR)"Close", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400, 600, 200, 30, hWnd, (HMENU)102, hInst, 0);
   static HWND hMoveButton = CreateWindow((LPCSTR)"button", (LPCSTR)"Move", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 700, 600, 200, 30, hWnd, (HMENU)103, hInst, 0);

   if (!hWnd || !hClearButton || !hCloseButton || !hMoveButton)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
HBITMAP hBitmap;
int imageXPos = 0;
int imageYPos = 0;
bool clickFlag = false;
bool movingFlag = false;
bool initPaint = true;
bool NeedToDraw = false;
int xMovement = 2;
int yMovement = 2;
RECT workingRect = { 100 + 1, 100 + 1, 900 - 1, 400 - 1 };
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	BITMAP bm;
	GetObject(hBitmap, sizeof(bm), &bm);
    switch (message)
    {
	case WM_CREATE:
		if ((hBitmap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1))) == NULL)
		{
			MessageBox(hWnd, "Нет картинки", "Ошибка", MB_OK);
			break;
		}
		InvalidateRect(hWnd, 0, TRUE);
		NeedToDraw = false;
		break;
	case WM_LBUTTONDOWN:
		if (clickFlag) break;

		imageXPos = LOWORD(lParam);
		imageXPos -= bm.bmWidth / 2;
		imageYPos = HIWORD(lParam);
		imageYPos -= bm.bmHeight / 2;

		if (imageXPos < 100 || imageXPos > 900 - bm.bmHeight || imageYPos < 100 || imageYPos > 400 - bm.bmHeight) break;
		clickFlag = true;
		InvalidateRect(hWnd, &workingRect, TRUE);
		NeedToDraw = true;
		break;
    case WM_PAINT:
		DrawBitmap(hWnd, bm, NeedToDraw);
        break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case 101:
				InvalidateRect(hWnd, &workingRect, TRUE);
				NeedToDraw = false;

				clickFlag = false;
				movingFlag = false;
				KillTimer(hWnd, 1);
				break;
			case 102:
				switch ((int)MessageBox(NULL, "Вы уверены, что хотите выйти?", "Предупреждение", MB_ICONQUESTION | MB_YESNO))
				{
					case IDYES:
						PostQuitMessage(NULL);
						DestroyWindow(hWnd);
						break;
					case IDNO: 
						break;
				}
				break;
			case 103:
				if (!movingFlag)
				{
					if (!clickFlag) {
						MessageBox(NULL, "Не нарисовано изображение!", "Ошибка", MB_OK); 
						break;
					}
					SetTimer(hWnd, 1, 16, NULL);
				}
				else
				{
					KillTimer(hWnd, 1);
				}
				movingFlag = !movingFlag;
				break;
		}
		break;
	case WM_TIMER:
		if (!movingFlag) break;
			InvalidateRect(hWnd, &workingRect, TRUE);
			NeedToDraw = true;

			imageXPos += xMovement;
			imageYPos += yMovement;
			if (imageXPos < 100 || imageXPos > 900 - bm.bmWidth) xMovement *= -1;
			if (imageYPos < 100 || imageYPos > 400 - bm.bmHeight) yMovement *= -1;
		break;
	case WM_CLOSE:
		PostQuitMessage(NULL);
		DestroyWindow(hWnd);
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void DrawBitmap(HWND hWnd, BITMAP bm, bool needToDrawImage)
{
	HDC hDC, hdcMem;
	HGDIOBJ hbmOld;
	PAINTSTRUCT ps;
	hDC = BeginPaint(hWnd, &ps);
	hdcMem = CreateCompatibleDC(hDC);
	hbmOld = SelectObject(hdcMem, hBitmap);
	if(needToDrawImage) BitBlt(hDC, imageXPos, imageYPos, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
	MoveToEx(hDC, 100, 100, NULL);
	LineTo(hDC, 900, 100);
	LineTo(hDC, 900, 400);
	LineTo(hDC, 100, 400);
	LineTo(hDC, 100, 100);
	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);
	EndPaint(hWnd, &ps);
}
