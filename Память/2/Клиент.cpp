#include <windows.h>
#include <iostream>
#include <memory>
#include <io.h>
#include <fcntl.h>

struct Close { typedef HANDLE pointer; void operator()(HANDLE h) { if (h != INVALID_HANDLE_VALUE)  CloseHandle(h); } };
using unique_handle = std::unique_ptr<HANDLE, Close>;

std::wstring getErrMsg() {
	wchar_t buf[256];
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
	return(std::wstring(buf));
}


int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);

	const wchar_t* slotname = L"\\\\.\\mailslot\\demoslot";
	unique_handle SLOT(CreateFile(slotname, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL));


	if (SLOT.get() == INVALID_HANDLE_VALUE) {
		std::wcerr << L"[Клиент 0]: Ошибка открытия. " << getErrMsg() << std::endl;
		exit(1);
	}
	Sleep(150);

	const int n_towrite = 42;

	if (!WriteFile(SLOT.get(), &n_towrite, sizeof(int), nullptr, nullptr)) {
		std::wcerr << L"[Клиент 0]: Ошибка записи. " << getErrMsg() << std::endl;
		exit(1);
	}
	std::wcout << L"[Клиент 0]: Запись 1 произведена в ящик: " << n_towrite << std::endl;
	Sleep(600);

	const wchar_t* s_towrite = L"thirty";

	if (!WriteFile(SLOT.get(), s_towrite, wcslen(s_towrite) * sizeof(wchar_t), nullptr, nullptr)) {
		std::wcerr << L"Ошибка записи!" << getErrMsg() << std::endl;
		exit(1);
	}

	std::wcout << L"[Клиент 0]: Запись 2 произведена в ящик: " << s_towrite << std::endl;

}

