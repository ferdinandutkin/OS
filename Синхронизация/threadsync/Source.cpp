#include <windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>
 
std::wstring getErrMsg() {
	wchar_t buf[256];
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
	return(std::wstring(buf));
}
 
CRITICAL_SECTION cs;

const wchar_t* path = L"C:\\Users\\ferdinand\\Desktop\\OS\\filebuf.txt";

DWORD WINAPI Writer(void*) {

	for (int i = 0; i < 50; i++) {
		std::wstring to_write(std::to_wstring(i));

		EnterCriticalSection(&cs);

		HANDLE file = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file == INVALID_HANDLE_VALUE) {
			std::wcerr << L"Ошибка открытия файла. " << getErrMsg() << std::endl;
			exit(1);

		}
			
		if (!WriteFile(file, to_write.c_str(), to_write.size() * sizeof(wchar_t), nullptr, NULL)) {
			std::wcerr << L"Ошибка записи. " << getErrMsg() << std::endl;
			CloseHandle(file);
			exit(2);

		}
		CloseHandle(file);

		LeaveCriticalSection(&cs);

		std::wcout << L"Записано: " << to_write << std::endl;
 

		Sleep(200);

	}

	return 0;
}

DWORD WINAPI Reader(void*) {

	wchar_t buffer[256];
	ZeroMemory(buffer, 256 * sizeof(wchar_t));
	 

	for (int i = 0; i < 50; i++) {
		EnterCriticalSection(&cs);

		HANDLE file = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (file == INVALID_HANDLE_VALUE) {
			std::wcerr << L"Ошибка открытия файла. " << getErrMsg() << std::endl;
			exit(1);

		}

		if (!ReadFile(file, buffer, 256, nullptr, NULL)) {
			std::wcerr << L"Ошибка чтения. " << getErrMsg() << std::endl;
			CloseHandle(file);
			exit(2);

		}
		CloseHandle(file);

		LeaveCriticalSection(&cs);
		
		std::wcout << L"Прочитано: " << buffer << std::endl;

		Sleep(200);
	}

	return 0;

}


int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	InitializeCriticalSection(&cs);
	HANDLE threads[2] = { CreateThread(0, 0, Writer, nullptr, CREATE_SUSPENDED, nullptr), CreateThread(0, 0, Reader, nullptr, CREATE_SUSPENDED, nullptr) };
	ResumeThread(threads[0]);
	Sleep(100);
	ResumeThread(threads[1]);

	WaitForMultipleObjects(2, threads, true, 11000);
	 
	 
}
 