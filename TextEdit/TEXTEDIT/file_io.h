 
#include "framework.h"

using unique_handle = std::unique_ptr<std::remove_pointer<HANDLE>::type, std::integral_constant<decltype(&CloseHandle), &CloseHandle>>;

namespace file_io {
    enum class file_state{not_opened, opened};
    template < DWORD flags>
    struct open_text_file {
        OPENFILENAMEW file{ sizeof(OPENFILENAME) };
        open_text_file(const HWND parent, std::wstring& current) {
            file.hwndOwner = parent;
            file.lpstrFilter = L"Текстовые файлы(*.txt)\0*.txt\0";
            file.lpstrFile = current.data();
            file.nMaxFile = MAX_PATH;
            file.Flags = flags;
            file.lpstrDefExt = L"txt";
        }

        LPOPENFILENAMEW operator & () {
            return &file;
        }
    };

    using open_txt_to_edit = open_text_file<OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY>;
    using open_txt_to_save = open_text_file<OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT>;

    static void get_current_directory(std::wstring& name) {
        name.resize(MAX_PATH);
        name.resize(GetCurrentDirectoryW(MAX_PATH, name.data()));
       
    }


    static void show_file_contents(const HWND handle_window,  std::wstring& file) {
        HWND handle_parent = GetParent(handle_window);

        unique_handle hFile(CreateFileW(file.data(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, NULL, NULL));


        DWORD myltibyte_length = GetFileSize(hFile.get(), nullptr) + 1;
        std::unique_ptr<char[]> tempftext = std::make_unique< char[] >(myltibyte_length);


        if (ReadFile(hFile.get(), tempftext.get(), myltibyte_length, nullptr, nullptr)) {

            int wchar_length = MultiByteToWideChar(CP_UTF8, 0, tempftext.get(), -1, nullptr, 0);
            std::unique_ptr<wchar_t[]> converted = std::make_unique< wchar_t[] >(wchar_length);
            MultiByteToWideChar(CP_UTF8, 0, tempftext.get(), -1, converted.get(), wchar_length);


            SetWindowTextW(handle_window, converted.get());
            RedrawWindow(handle_parent, nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);

        }

        SetWindowTextW(handle_parent, file.c_str());
    }


    static void open_to_edit(const HWND handle_window, std::wstring& init_file, file_state& init_state) {
        HWND handle_parent = GetParent(handle_window);

        open_txt_to_edit file_to_open(handle_parent, init_file);

        if (GetOpenFileNameW(&file_to_open)) {
            show_file_contents(handle_window, init_file);
            init_state = file_state::opened;
           

        }


    }


    static void save_to_file(const HWND handle_window, const std::wstring init_file) {
     
        unique_handle handle_file(CreateFileW(init_file.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));

        int wchar_length = GetWindowTextLengthW(handle_window) + 1;

        std::unique_ptr<wchar_t[]> file_text = std::make_unique< wchar_t[] >(wchar_length);

            if (GetWindowTextW(handle_window, file_text.get(), wchar_length)) {
               
                int myltibyte_length = WideCharToMultiByte(CP_UTF8, 0, file_text.get(), wcslen(file_text.get()), nullptr, 0, nullptr, nullptr);

                std::unique_ptr<char[]> converted = std::make_unique< char[] >(myltibyte_length);

                WideCharToMultiByte(CP_UTF8, 0, file_text.get(), wcslen(file_text.get()), converted.get(), myltibyte_length, nullptr, nullptr);

                WriteFile(handle_file.get(), converted.get(), myltibyte_length * sizeof(char), nullptr, nullptr);
 
            }

    }


    static void open_file_to_save(const HWND handle_window, std::wstring& init_file, file_state& init_state) {
        HWND handle_parent = GetParent(handle_window);
        open_txt_to_save openfilename(handle_parent, init_file);

        if (GetSaveFileNameW(&openfilename)) {
            init_state = file_state::opened;      
            SetWindowTextW(handle_parent, init_file.c_str());
            
        }

        
    }

   

    

}
