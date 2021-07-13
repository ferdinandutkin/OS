// TEXTEDIT.cpp : Определяет точку входа для приложения.
//

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#include "TEXTEDIT.h"
#include "history.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна


HWND g_hwnd;



// Отправить объявления функций, включенных в этот модуль кода:

bool                init_instance(HINSTANCE, int);
LRESULT CALLBACK    window_procedure(HWND, UINT, WPARAM, LPARAM);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,  _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine,  _In_ int nCmdShow){
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    LoadStringW(hInstance, IDC_TEXTEDIT, szWindowClass, MAX_LOADSTRING);

    WNDCLASSEXW wcex{ sizeof(WNDCLASSEX) };

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = window_procedure;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEXTEDIT));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TEXTEDIT);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    // Выполнить инициализацию приложения:
    if (!init_instance(hInstance, nCmdShow)) {
        return false;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TEXTEDIT));

    MSG msg;

 
    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0)) {
        if (!TranslateAccelerator(g_hwnd, hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return  static_cast<int>(msg.wParam);
}









 
bool init_instance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

    HWND hWnd = CreateWindowW(szWindowClass, nullptr, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
    g_hwnd = hWnd;
    if (!hWnd) {
        return false;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return true;
}



//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//



HWND create_toolbar(const HWND handle_window) {

    const int button_count = 9;
    const int bitmap_size = 16;



    HWND handle_toolbar = CreateWindowExW(0, TOOLBARCLASSNAME, NULL, WS_CHILD | TBSTYLE_FLAT, 0, 0, 0, 0, handle_window, NULL, hInst, NULL);


    HIMAGELIST image_list = ImageList_Create(bitmap_size, bitmap_size, ILC_COLOR16 | ILC_MASK, button_count, 0);

    SendMessageW(handle_toolbar, TB_SETIMAGELIST, 0, reinterpret_cast<LPARAM>(image_list));


    SendMessageW(handle_toolbar, TB_LOADIMAGES, static_cast<WPARAM>(IDB_STD_SMALL_COLOR), reinterpret_cast<LPARAM>(HINST_COMMCTRL));


    TBBUTTON buttons[button_count] = {
        { STD_FILENEW, IDM_NEW,  TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, reinterpret_cast<INT_PTR>(L"Новый") },
        { STD_FILEOPEN, IDM_OPEN, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, reinterpret_cast<INT_PTR>(L"Открыть")},
        { STD_FILESAVE, IDM_SAVE, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, reinterpret_cast<INT_PTR>(L"Сохранить")},
        { STD_FILESAVE, IDM_SAVE_AS, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, reinterpret_cast<INT_PTR>(L"Сохранить как...")},
        { 0, 0, TBSTATE_ENABLED,  BTNS_SEP, {0}, 0, 0},
        { STD_CUT, IDM_CUT, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0,  reinterpret_cast <INT_PTR>(L"Вырезать")},
        { STD_COPY, IDM_COPY, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0,  reinterpret_cast <INT_PTR>(L"Копировать")},
        { STD_PASTE, IDM_PASTE, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, reinterpret_cast<INT_PTR>(L"Вставить")},
        { STD_DELETE, IDM_DELETE, TBSTATE_ENABLED, BTNS_AUTOSIZE, {0}, 0, reinterpret_cast<INT_PTR>(L"Удалить")},
    };

    SendMessageW(handle_toolbar, TB_BUTTONSTRUCTSIZE, static_cast <WPARAM>(sizeof(TBBUTTON)), 0);
    SendMessageW(handle_toolbar, TB_ADDBUTTONS, static_cast<WPARAM>(button_count), reinterpret_cast<LPARAM>(buttons));


    SendMessageW(handle_toolbar, TB_AUTOSIZE, 0, 0);
    ShowWindow(handle_toolbar, true);

    return handle_toolbar;
}



LRESULT CALLBACK window_procedure(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    bool (*confirm)() = [] { return MessageBoxW(NULL, L"Вы уверены, что хотите потерять несохраненные данные?", L"Подтверждение", MB_ICONEXCLAMATION | MB_YESNO) == IDYES; };
    const wchar_t* nf_name = L"Новый файл.txt";

    static file_io::file_state current_state;
    static std::wstring current_file;

    std::wstring init_current_file = current_file;

    static HWND handle_edit;
    static HWND handle_toolbar;

    static int toolbar_height;

    static COLORREF edit_color;
    static HFONT edit_font;

   

    static history::history_menu menu(hWnd, 1200, 3);




    switch (message) {



    case WM_CREATE: {
 
        menu.show();

        file_io::get_current_directory(current_file);
       
        current_file += L"\\";
        current_file += nf_name;

        init_current_file = current_file;

        current_state = file_io::file_state::not_opened;


        SetWindowTextW(hWnd, nf_name);
        RECT parent_rect, toolbar_rect;

        handle_toolbar = create_toolbar(hWnd);

        GetClientRect(handle_toolbar, &toolbar_rect);
        GetClientRect(hWnd, &parent_rect);

        toolbar_height = toolbar_rect.bottom - toolbar_rect.top;


        handle_edit = CreateWindowExW(0L, L"Edit", nullptr, WS_VISIBLE | WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL | WS_CLIPCHILDREN | ES_NOHIDESEL | ES_AUTOVSCROLL | ES_MULTILINE,
            0, toolbar_height, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top - toolbar_height,
            hWnd, reinterpret_cast<HMENU>(ID_TEXT_EDIT), hInst, NULL);


        edit_font = CreateFontW(32, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_SWISS, L"Calibri");


        SendMessageW(handle_edit, WM_SETFONT, reinterpret_cast<WPARAM>(edit_font), true);

        SetFocus(handle_edit);

        break;
    }

    case WM_SIZE: {
        RECT parent_rect;
        GetClientRect(hWnd, &parent_rect);
        SendMessageW(handle_toolbar, TB_AUTOSIZE, 0, 0);
        MoveWindow(handle_edit, 0, toolbar_height, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top - toolbar_height, true);

        break;

    }

    case WM_CTLCOLOREDIT:
        if (reinterpret_cast<HWND>(lParam) == handle_edit) {
            SetTextColor(reinterpret_cast<HDC>(wParam), edit_color);
            return reinterpret_cast<bool>((reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH))));
        }
        return false;

    case WM_CLOSE:
        if (confirm())
            DestroyWindow(hWnd);
        else
            SendMessageW(hWnd, WM_COMMAND, MAKEWPARAM(IDM_SAVE, BN_CLICKED), 0);
        break;
    


    case WM_COMMAND: {

        const int command = LOWORD(wParam);

        std::unordered_map<int, int> idm_wm{ {IDM_CUT, WM_CUT}, {IDM_COPY, WM_COPY}, {IDM_PASTE, WM_PASTE}, {IDM_DELETE, WM_CLEAR} };

        std::unordered_map<int, formatting::font_function> font_stuff{ {IDM_FONT, formatting::set_font},
          {IDM_ITALIC, formatting::toggle_italic}, {IDM_BOLD, formatting::toggle_bold},
          {IDM_UNDERLINE, formatting::toggle_underline}, {IDM_STRIKE_OUT, formatting::toggle_strike_out} };



        if ((command >= IDM_CUT) && (command <= IDM_DELETE)) {
            SendMessageW(handle_edit, idm_wm[command], 0, 0);
            RedrawWindow(hWnd, nullptr, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
            break;
        }
        if (command == IDM_SEL_ALL) {
            SendMessageW(handle_edit, EM_SETSEL, 0, -1);
            break;
        }
            
        if ((command >= IDM_FONT) && (command <= IDM_STRIKE_OUT)) {
            font_stuff[command](handle_edit, edit_font);
            break;
        }

        if (menu.in_range(command) && (current_file != menu.get(command))) {
            current_file = menu.get(command);
            file_io::show_file_contents(handle_edit, current_file);
            break;
            
        }


            switch (command) {

            case IDM_NEW:
                if (confirm()) {
                    SetWindowTextW(handle_edit, nullptr);
                    SetWindowTextW(hWnd, nf_name);
                    current_state = file_io::file_state::not_opened;
                }
                else SendMessageW(hWnd, WM_COMMAND, MAKEWPARAM(IDM_SAVE, BN_CLICKED), 0);
                break;


            case IDM_OPEN:
                file_io::open_to_edit(handle_edit, current_file, current_state);
                break;
         

            case IDM_SAVE:
                if (current_state == file_io::file_state::opened)
                    file_io::save_to_file(handle_edit, current_file);
                else {
                    file_io::open_file_to_save(handle_edit, current_file, current_state);

                    if (current_state == file_io::file_state::opened)
                        file_io::save_to_file(handle_edit, current_file);
                }
                break;

            case IDM_SAVE_AS:
                file_io::open_file_to_save(handle_edit, current_file, current_state);
                if (current_state == file_io::file_state::opened)
                    file_io::save_to_file(handle_edit, current_file);
                break;

            case IDM_COLOR:
                formatting::set_color(handle_edit, edit_color);
                break;

            case IDM_EXIT:
                 SendMessageW(hWnd, WM_CLOSE, 0, 0);
                break;
            default:
                return DefWindowProcW(hWnd, message, wParam, lParam);
            }
    }
                   break;

    case WM_DESTROY:
        DeleteObject(edit_font);

        PostQuitMessage(0);
                   break;
    default:
        return DefWindowProcW(hWnd, message, wParam, lParam);
    }

    if (current_file != init_current_file) {
        menu.append(current_file);

    }
     
    return 0;
}

 