#include <windows.h>
#include <string>
#include <cctype>
#include <vector>
#include "Calc.h"
#include <map>


 

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
 
using namespace std;

static WNDPROC InitEditProc;

BOOL CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

 
LRESULT CALLBACK FilterDlgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	
	case WM_KEYDOWN: {
		if (wParam == VK_ADD)
			return SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(KEYPLUS, BN_CLICKED), NULL);
		else if (wParam == VK_SUBTRACT)
			return SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(KEYMINUS, BN_CLICKED), NULL);
		else if (wParam == VK_DIVIDE) 
			return SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(KEYDIV, BN_CLICKED), NULL);
		else if (wParam == VK_ADD)
			return SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(KEYPLUS, BN_CLICKED), NULL);
		else if (wParam == VK_MULTIPLY)
			return SendMessage(GetParent(hWnd), WM_COMMAND, MAKEWPARAM(KEYMULT, BN_CLICKED), NULL);		
		break;
	} 
	case WM_CHAR: {
		if (iswgraph(wParam) && !iswdigit(wParam) && (wParam != '.'))
			return 0;
	}

	
	}
	return InitEditProc(hWnd, uMsg, wParam, lParam);
}



BOOL CALLBACK DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
 
	switch (uMsg) {

		
	case WM_COMMAND: {

		int COMMAND = LOWORD(wParam);

		if ((COMMAND >= KEY0) && (COMMAND <= KEY9)) {				
			HWND TEXTEDIT = GetDlgItem(hWnd, IDC_INPUT);
			SendMessage(TEXTEDIT, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(to_wstring(COMMAND - KEY0).c_str()));
			break;
		}
		if ((COMMAND >= KEYPLUS) && (COMMAND <= KEYDIV)) {
			HWND TEXTEDIT = GetDlgItem(hWnd, IDC_INPUT); 
			map<int, wstring> signs = { {KEYDIV, L" / "}, {KEYPLUS, L" + " }, {KEYMINUS, L" - "}, {KEYMULT, L" * "} };
			SendMessage(TEXTEDIT, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(signs[COMMAND].c_str()));
			break;
		}
		 
		switch (COMMAND) {

		case KEYEQ: {
			HWND TEXTEDIT = GetDlgItem(hWnd, IDC_INPUT); 

			int len = GetWindowTextLength(TEXTEDIT) + 1;

			SendMessage(TEXTEDIT, EM_SETSEL, len, len);
			wchar_t* buffer = new wchar_t[len];

			GetWindowText(TEXTEDIT, buffer, len);

			buffer[len - 1] = 0;
		

			SendMessage(TEXTEDIT, EM_REPLACESEL, TRUE, reinterpret_cast<LPARAM>(calculate(wstring(buffer)).c_str()));
			
			delete[] buffer;
			break;
		}
	

		case KEYCLR: {
			HWND TEXTEDIT = GetDlgItem(hWnd, IDC_INPUT);
			SendMessage(TEXTEDIT, WM_SETTEXT, NULL, reinterpret_cast<LPARAM>(L""));
			break;
		}
		
		case IDCANCEL:
			EndDialog(hWnd, TRUE);
			break;

		}
		break;
	}
	case WM_INITDIALOG: {
		HWND TEXTEDIT = GetDlgItem(hWnd, IDC_INPUT);
		HFONT hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, L"Roboto Medium");
		SendMessage(TEXTEDIT, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
		SetFocus(TEXTEDIT);

		 
		InitEditProc = reinterpret_cast<WNDPROC>(SetWindowLong(TEXTEDIT, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(FilterDlgProc)));

	}
		 
		

	default:
 
		return FALSE;
	}

 
	return TRUE;
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{


	DialogBox(hInstance, MAKEINTRESOURCE(IDD_CALCMAINDIAL), NULL, (DLGPROC)DialogProc);
	return 0;
}

