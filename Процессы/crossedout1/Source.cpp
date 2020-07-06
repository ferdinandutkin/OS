#include <windows.h>
#include <iostream>
#include <string>
#include <io.h>
#include <tlhelp32.h>
#include <fcntl.h>
#include <memory>


using unique_handle = std::unique_ptr<std::remove_pointer<HANDLE>::type, std::integral_constant<decltype(&CloseHandle), &CloseHandle>>;


bool IsProcessRun(wchar_t* processName) { //� �������� ���������� �� ��� �� ���������
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
	std::wcout << L"������� ��� �����: ";
	std::getline(std::wcin, name_to_search);

	HANDLE file_search;
	WIN32_FIND_DATA found_file_data;

	if (!name_to_search.ends_with(L".exe"))  name_to_search += L".exe";


	file_search = FindFirstFileW(name_to_search.c_str(), &found_file_data);


	if (file_search == INVALID_HANDLE_VALUE) {
		std::wcerr << L"������ ������ �����. " + getErrMsg() << std::endl;
		exit(1);
	}

	std::wcout << L"���� ������: " << found_file_data.cFileName << std::endl;
	FindClose(file_search);

	STARTUPINFO cif = { 0 };
	cif.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi = { 0 };

	
	if (!CreateProcessW(NULL, const_cast<wchar_t*>((name_to_search.substr(0, name_to_search.find_last_of(L"\\/") + 1) + std::wstring(found_file_data.cFileName)).c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &cif, &pi)) {
		std::wcerr << L"������ �������� ��������. " + getErrMsg() << std::endl;
		exit(1);
	}
	std::wcout << L"������� ������ �������" << std::endl;

	Sleep(2500); //��� �� ����� �� ������� � �� �������� ���� �� ����� �� ����� ������ ���� ������� ������ �������

	DWORD code;
	GetExitCodeProcess(pi.hProcess, &code); //��� �� ��� ����� IsProcessRun � ��������?

	std::wcout << ((code == STILL_ACTIVE)? L"��� ��� ��������" : L"��� �� �������� ") << std::endl;
	std::wcout << ((IsProcessRun(found_file_data.cFileName)) ? L"���������� ����� ��������" : L"��� �� �������� ") << std::endl;

}



void init() {
		std::wstring name_to_search;
		std::wcout << L"������� ��� �����: ";
		std::getline(std::wcin, name_to_search);

		if (!name_to_search.ends_with(L".exe"))  name_to_search += L".exe";

		HANDLE file_search;
		WIN32_FIND_DATA found_file_data;

		file_search = FindFirstFile(name_to_search.c_str(), &found_file_data);

		if (file_search == INVALID_HANDLE_VALUE) {
			std::wcerr << L"�� ������� ����� ����. " + getErrMsg() << std::endl;
			exit(1);
		}
		std::wcout << L"���� ������: " << found_file_data.cFileName << std::endl;
		FindClose(file_search);

		STARTUPINFO cif = { 0 };
		cif.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION pi = { 0 };

		if (!CreateProcessW(NULL, const_cast<wchar_t*>((name_to_search.substr(0, name_to_search.find_last_of(L"\\/") + 1) + std::wstring(found_file_data.cFileName)).c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &cif, &pi)) {
			std::wcerr << L"������ �������� ��������. " + getErrMsg() << std::endl;
			exit(1);
		}
		std::wcout << L"������� ������ �������" << std::endl;


}


int main() {
	//ends_with �� c++ 20 ��� ��� ������������ � latest 
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
	std::wcout << L"������� 1" << std::endl << std::endl;
	init();
	std::wcout << std::endl << L"������� 2" << std::endl << std::endl;
	init1();
	return 0;
}