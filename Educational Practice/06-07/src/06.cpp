// 06.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "06.h"
#include "MVC.h"

#define MAX_LOADSTRING 100
#define STR_SIZE 80

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int ifont_size = 2;
int font_size[] = { 14, 16, 18, 22, 24 };
COLORREF custom_colors[16];
COLORREF current_color = RGB(255, 255, 255);
HWND AboutDlg = NULL;

_Data Data(hInst);
_Controller Controller(Data);
_View View(Data);
_DiagramView DiagramView(Data);


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AboutDlgProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	ViewDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	AddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	ColorDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK	DiagramDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

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
    LoadStringW(hInstance, IDC_MY06, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY06));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY06));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY06);
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
      CW_USEDEFAULT, 0, 650, 500, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//Common Dialog boxes "OPEN", "SAVE AS"
	static OPENFILENAME ofn;
	static char szFile[MAX_PATH];

    switch (message)
    {
	case WM_CREATE:
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = "Text Files(*.txt)\0*.txt\0All files(*.*)\0*.*\0\0";
		ofn.Flags = OFN_CREATEPROMPT;

		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_FILE_OPEN:
				strcpy_s(szFile, sizeof(szFile), "");
				if (GetOpenFileName(&ofn))
				{
					Controller.File_Open(ofn, hWnd);
				}
				break;
			case ID_FILE_CLOSE:
				Controller.File_Close(ofn, hWnd);
				break;
			case ID_FILE_SAVE:
				Controller.File_Save();
				break;
			case ID_FILE_SAVEAS:
				strcpy_s(szFile, sizeof(szFile), "");
				if (GetSaveFileName(&ofn))
				{
					Controller.File_Save_as(ofn, hWnd);
				}
				break;
			
			case IDM_VIEW:
				View.Show(hWnd);
				break;
			case IDM_DIAGRAM:
				DiagramView.Show(hWnd);
				break;
			case IDM_COLOR:
				Controller.Choose_color(hWnd);
				break;
            case IDM_ABOUT:
				if (!IsWindow(AboutDlg))
					AboutDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_CONTEXTMENU:
	{
		HMENU hMenu = CreatePopupMenu();

		AppendMenu(hMenu, MFT_STRING, IDM_VIEW, "&View");
		AppendMenu(hMenu, MFT_STRING, IDM_DIAGRAM, "&Diagram");
		AppendMenu(hMenu, MFT_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MFT_STRING, ID_FILE_SAVE, "&Save");
		AppendMenu(hMenu, MFT_STRING, ID_FILE_CLOSE, "&Close");
		AppendMenu(hMenu, MFT_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MFT_STRING, IDM_EXIT, "&Exit");

		TrackPopupMenu(hMenu, TPM_RIGHTBUTTON | TPM_TOPALIGN | TPM_LEFTALIGN, LOWORD(lParam), HIWORD(lParam), 0, hWnd, NULL);
		DestroyMenu(hMenu);
	}
	break;

	case WM_PAINT:
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		int width = rect.right - rect.left,
			height = rect.bottom - rect.top;
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc, width / 10, height * 2 / 5, "Current open file: ", 20);
		TextOut(hdc, width / 10 + 150, height * 2 / 5, Controller.get_finName().c_str(), Controller.get_finName().size());
		TextOut(hdc, width / 10, height * 2 / 5 + 30, "Save directory: ", 17);
		TextOut(hdc, width / 10 + 150, height * 2 / 5 + 30, Controller.get_foutName().c_str(), Controller.get_foutName().size());
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

 INT_PTR CALLBACK ViewDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static LOGFONT lf;
	static HFONT hFont1;
	std::string temp;
	int i;
	
	int iCurListItem;

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:

		iCurListItem = 0;

		// Шрифт
		lf.lfHeight = font_size[ifont_size];
		lstrcpy((LPSTR)&lf.lfFaceName, "Courier");
		hFont1 = CreateFontIndirect(&lf);
		SendDlgItemMessage(hDlg, IDC_LIST, WM_SETFONT, (WPARAM)hFont1, NULL);
		

		// Инициализация комбо-бокса
		for (i = 0; i < sizeof(font_size) / sizeof(int); ++i)
		{
			temp = std::to_string(font_size[i]);
			SendDlgItemMessage(hDlg, IDC_COMBOFONT, CB_ADDSTRING, NULL, (LPARAM)temp.c_str());
		}
		SendDlgItemMessage(hDlg, IDC_COMBOFONT, CB_SETCURSEL, (WPARAM)ifont_size, NULL);

		// Данные из файла
		for (i = 0; i < Data.get_nItems(); ++i )
			SendDlgItemMessage(hDlg, IDC_LIST, LB_ADDSTRING, 0, (LPARAM)Data[i].line.c_str());

		ShowWindow(hDlg, SW_SHOW);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch LOWORD(wParam)
		{
		case IDOK:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		case IDC_ADD:
			Controller.AddItem(hDlg);
			break;
		case IDC_DELETE:
			Controller.DeleteItem(hDlg);
			break;
		case IDC_LIST:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
				Controller.ItemClick(hDlg);
				return TRUE;
				break;
			}
			break;
		case IDC_COMBOFONT:
			switch (HIWORD(wParam)) 
			{
			case CBN_SELCHANGE: 
				ifont_size = (int)SendDlgItemMessage(hDlg, IDC_COMBOFONT, CB_GETCURSEL, NULL, NULL);
				if (hFont1)
					DeleteObject(hFont1);
				lf.lfHeight = font_size[ifont_size];
				lstrcpy((LPSTR)&lf.lfFaceName, "Courier");
				hFont1 = CreateFontIndirect(&lf);
				SendDlgItemMessage(hDlg, IDC_LIST, WM_SETFONT, (WPARAM)hFont1, TRUE);
				return TRUE;
				break;
			}
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK AddDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{	
	static char temp[STR_SIZE];
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		ShowWindow(hDlg, SW_SHOW);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch LOWORD(wParam)
		{
		case IDOK: 
			Data.push_back();
			GetDlgItemText(hDlg, IDC_ADD_EDIT1, temp, STR_SIZE);
			Data[Data.get_nItems()].name = temp;
			GetDlgItemText(hDlg, IDC_ADD_EDIT2, temp, STR_SIZE);
			Data[Data.get_nItems()].code = temp;
			GetDlgItemText(hDlg, IDC_ADD_EDIT3, temp, STR_SIZE);
			Data[Data.get_nItems()].quantity = temp;
			GetDlgItemText(hDlg, IDC_ADD_EDIT4, temp, STR_SIZE);
			Data[Data.get_nItems()].unitPrice = temp;
			GetDlgItemText(hDlg, IDC_ADD_EDIT5, temp, STR_SIZE);
			Data[Data.get_nItems()].age = temp;
			Data[Data.get_nItems()].age += '-';
			GetDlgItemText(hDlg, IDC_ADD_EDIT6, temp, STR_SIZE);
			Data[Data.get_nItems()].age += temp;
			GetDlgItemText(hDlg, IDC_ADD_EDIT7, temp, STR_SIZE);
			Data[Data.get_nItems()].date = temp;
			Data[Data.get_nItems()].date += '.';
			GetDlgItemText(hDlg, IDC_ADD_EDIT8, temp, STR_SIZE);
			Data[Data.get_nItems()].date += temp;
			GetDlgItemText(hDlg, IDC_ADD_EDIT9, temp, STR_SIZE);
			Data[Data.get_nItems()].provider = temp;

			Data[Data.get_nItems()].doOneString();
			Data.Inc_nItems();

			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDCANCEL:
			EndDialog(hDlg, (INT_PTR)FALSE);
			return (INT_PTR)TRUE;
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DiagramDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		ShowWindow(hDlg, SW_SHOW);
		return (INT_PTR)TRUE;

	case WM_PAINT:
	{
		DiagramView.Paint(hDlg, current_color);
	}
	break;
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

INT_PTR CALLBACK ColorDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static CHOOSECOLOR cc;
	static COLORREF temp_color;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		SendDlgItemMessage(hDlg, IDC_RADIO1, BM_SETCHECK, BST_CHECKED, NULL);
		temp_color = RGB(255, 255, 255);
		cc.lStructSize = sizeof(cc);
		cc.lpCustColors = custom_colors;
		cc.Flags = CC_FULLOPEN;
		ShowWindow(hDlg, SW_SHOW);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch LOWORD(wParam)
		{
		case IDC_RADIO1:
			temp_color = RGB(255, 255, 255);
			break;
		case IDC_RADIO2:
			temp_color = RGB(100, 100, 100);
			break;
		case IDC_RADIO3:
			temp_color = RGB(190, 10, 10);
			break;
		case IDC_RADIO4:
			temp_color = RGB(30, 20, 250);
			break;
		case IDC_COLOR:
			if (ChooseColor(&cc))
				temp_color = cc.rgbResult;
			break;
		case IDOK:
			current_color = temp_color;				
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		case IDCANCEL:
			EndDialog(hDlg, (INT_PTR)FALSE);
			return (INT_PTR)TRUE;
			break;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

// Message handler for about box.
INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
		ShowWindow(hDlg, SW_SHOW);
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
			AboutDlg = NULL;
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
