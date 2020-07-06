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
		std::wcerr << L"[Клиент 1]: Ошибка открытия. " << getErrMsg() << std::endl;
		exit(1);
	}
	Sleep(450);
	

	const float f_towrite = 3.14159f;

	if (!WriteFile(SLOT.get(), &f_towrite, sizeof(float), nullptr, nullptr)) {
		std::wcerr << L"[Клиент 1]: Ошибка записи. " << getErrMsg() << std::endl;
		exit(1);
	}
	std::wcout << L"[Клиент 1]: Запись 1 добавлена в ящик: " << f_towrite << std::endl;
}

