#include <windows.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <memory>

struct Close { typedef HANDLE pointer; void operator()(HANDLE h) { if (h != INVALID_HANDLE_VALUE)  CloseHandle(h); } };
using unique_handle = std::unique_ptr<HANDLE, Close>;

std::wstring getErrMsg() {
	wchar_t buf[256];
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
	return(std::wstring(buf));
}

HANDLE start(wchar_t* name) {
	return CreateThread(nullptr, NULL, [](void* name) -> DWORD {
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(STARTUPINFO);
		if (!CreateProcessW(NULL, static_cast<wchar_t*>(name), nullptr, nullptr, false, NULL, nullptr, nullptr, &si, &pi)) {
			std::wcerr << L"[Сервер]: Упс. Клиента не будет. " << getErrMsg() << std::endl;
			exit(1);
		}
		return 0;
	}, name, NULL, nullptr);
}

int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	const wchar_t* slotname = L"\\\\.\\mailslot\\demoslot";
	 

	unique_handle SLOT(CreateMailslot(slotname, 0, MAILSLOT_WAIT_FOREVER, NULL));

	
	if (SLOT.get() == INVALID_HANDLE_VALUE) {
		std::wcerr << L"[Сервер]: Ошибка открытия. " << getErrMsg() << std::endl;
		exit(1);
	}
	 
	wchar_t Cname[] = L"Client.exe";
	unique_handle Client(start(Cname));
	Sleep(20);

	wchar_t Cname1[] = L"Client1.exe";
	unique_handle Client1(start(Cname1));


	std::wcout << L"[Сервер]: Ожидание..." << std::endl;
	Sleep(300);

	int i_toread;

	if (!ReadFile(SLOT.get(), &i_toread, sizeof(int), nullptr, nullptr)) {
		std::wcerr << L"[Сервер]: Ошибка чтения. " << getErrMsg() << std::endl;
		exit(1);
	}
	std::wcout << L"[Сервер]: Данные прочитаны: " << i_toread << std::endl;
	Sleep(300);

	float f_toread;

	if (!ReadFile(SLOT.get(), &f_toread, sizeof(float), nullptr, nullptr)) {
		std::wcerr << L"[Сервер]: Ошибка чтения. " << getErrMsg() << std::endl;
		exit(1);
	}

	std::wcout << L"[Сервер]: Данные прочитаны: " << f_toread << std::endl;
	Sleep(300);


	wchar_t* s_toread = (wchar_t*)malloc(256);
	ZeroMemory(s_toread, 256);


	if (!ReadFile(SLOT.get(), s_toread, 256, nullptr, nullptr)) {
		std::wcerr << L"[Сервер]: Ошибка чтения. " << getErrMsg() << std::endl;
		exit(1);
	}
	std::wcout << L"[Сервер]: Данные прочитаны: " << s_toread << std::endl;
	free(s_toread);

	std::wcout << L"[Сервер]: Работа завершена" << std::endl;


}
