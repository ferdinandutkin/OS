#include <windows.h>
#include <iostream>
#include <string>
#include <io.h>
#include <tlhelp32.h>
#include <fcntl.h>
#include <memory>


using unique_handle = std::unique_ptr<std::remove_pointer<HANDLE>::type, std::integral_constant<decltype(&CloseHandle), &CloseHandle>>;


bool IsProcessRun(wchar_t* processName) { //я вынужден переписать ту что из методички
	unique_handle Snapshot(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	Process32First(Snapshot.get(), &pe);

	while (true) {
		if (!wcscmp(pe.szExeFile, processName)) return true;
		if (!Process32Next(Snapshot.get(), &pe)) return false;	
	}
}

std::wstring getErrMsg() {
	wchar_t buf[256];
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
	return(std::wstring(buf));
}




void init1() {
	std::wstring name_to_search;
	std::wcout << L"Введите имя файла: ";
	std::getline(std::wcin, name_to_search);

	HANDLE file_search;
	WIN32_FIND_DATA found_file_data;

	if (!name_to_search.ends_with(L".exe"))  name_to_search += L".exe";


	file_search = FindFirstFileW(name_to_search.c_str(), &found_file_data);


	if (file_search == INVALID_HANDLE_VALUE) {
		std::wcerr << L"Ошибка поиска файла. " + getErrMsg() << std::endl;
		exit(1);
	}

	std::wcout << L"Файл найден: " << found_file_data.cFileName << std::endl;
	FindClose(file_search);

	STARTUPINFO cif = { 0 };
	cif.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi = { 0 };

	
	if (!CreateProcessW(NULL, const_cast<wchar_t*>((name_to_search.substr(0, name_to_search.find_last_of(L"\\/") + 1) + std::wstring(found_file_data.cFileName)).c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &cif, &pi)) {
		std::wcerr << L"Ошибка создания процесса. " + getErrMsg() << std::endl;
		exit(1);
	}
	std::wcout << L"Процесс создан успешно" << std::endl;

	Sleep(2500); //эмм ну вдруг ее закроют а то проверка прям на месте не имеет смысла если процесс создан успешно

	DWORD code;
	GetExitCodeProcess(pi.hProcess, &code); //так ли нам нужен IsProcessRun в принципе?

	std::wcout << ((code == STILL_ACTIVE)? L"Все еще работает" : L"Уже не работает ") << std::endl;
	std::wcout << ((IsProcessRun(found_file_data.cFileName)) ? L"Совершенно точно работает" : L"Уже не работает ") << std::endl;

}



void init() {
		std::wstring name_to_search;
		std::wcout << L"Введите имя файла: ";
		std::getline(std::wcin, name_to_search);

		if (!name_to_search.ends_with(L".exe"))  name_to_search += L".exe";

		HANDLE file_search;
		WIN32_FIND_DATA found_file_data;

		file_search = FindFirstFile(name_to_search.c_str(), &found_file_data);

		if (file_search == INVALID_HANDLE_VALUE) {
			std::wcerr << L"Не удалось найти файл. " + getErrMsg() << std::endl;
			exit(1);
		}
		std::wcout << L"Файл найден: " << found_file_data.cFileName << std::endl;
		FindClose(file_search);

		STARTUPINFO cif = { 0 };
		cif.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION pi = { 0 };

		if (!CreateProcessW(NULL, const_cast<wchar_t*>((name_to_search.substr(0, name_to_search.find_last_of(L"\\/") + 1) + std::wstring(found_file_data.cFileName)).c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &cif, &pi)) {
			std::wcerr << L"Ошибка создания процесса. " + getErrMsg() << std::endl;
			exit(1);
		}
		std::wcout << L"Процесс создан успешно" << std::endl;


}


int main() {
	//ends_with из c++ 20 так что комилировать с latest 
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
	std::wcout << L"Задание 1" << std::endl << std::endl;
	init();
	std::wcout << std::endl << L"Задание 2" << std::endl << std::endl;
	init1();
	return 0;
}