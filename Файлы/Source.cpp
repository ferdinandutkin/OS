#include <windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <io.h>
#include <fcntl.h>

using std::wcin;
using std::wcout;
using std::wcerr;
using std::endl;
using std::wstring;
using std::getline;
using std::istreambuf_iterator;

wstring getErrMsg() {
	wchar_t buf[256];
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
	return buf; //эммм ez implicit conversion
}

int main() {

	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);


	wstring firstdirectory, seconddirectory;
	wcout << L"Введите имя первой директории: ";
	getline(wcin, firstdirectory);
	wcout << L"Введите имя второй директории: ";
	getline(wcin, seconddirectory);

	if (!((CreateDirectoryW(firstdirectory.c_str(), NULL)) && (CreateDirectoryW(seconddirectory.c_str(), NULL)))) {
		wcerr << L"Ошибка создания директорий. " + getErrMsg() << endl;
		exit(1);
	}
	wcout << L"Директории созданы успешно" << endl;


	SetCurrentDirectoryW(firstdirectory.c_str());

	wstring filename;
	wcout << L"Введите имя текстового файла для создания в первой директории: ";
	getline(wcin, filename);

	HANDLE filetowrite = CreateFileW(((filename.ends_with(L".txt")) ? filename : filename += L".txt").c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


	if (filetowrite == INVALID_HANDLE_VALUE) {
		wcerr << L"Ошибка создания файла. " + getErrMsg() << endl;
		exit(1);
	}
	wcout << L"Файл создан успешно" << endl;


	wcout << L"Введите текст для записи в файл: ";
	wstring texttowrite((istreambuf_iterator<wchar_t>(wcin)), istreambuf_iterator<wchar_t>()); //Ctrl+C посылает EOF
	clearerr(stdin);

	DWORD dwNumberOfBytes;

	if (!WriteFile(filetowrite, texttowrite.c_str(), texttowrite.size() * sizeof(wchar_t), &dwNumberOfBytes, NULL)) {
		wcerr << L"Ошибка записи. " + getErrMsg() << endl;
		CloseHandle(filetowrite);
		exit(1);
	}
	wcout << L"Запись проведена успешно" << endl;
	CloseHandle(filetowrite);


	if (!CopyFileW(filename.c_str(), (seconddirectory + L'\\' + filename).c_str(), NULL)) {
		wcerr << L"Ошибка копирования. " + getErrMsg() << endl;
		exit(1);
	}
	wcout << L"Копирование выполенено успешно" << endl;


	if (!DeleteFileW(filename.c_str())) {
		wcerr << L"Ошибка удаления. " + getErrMsg() << endl;
		exit(1);
	}
	wcout << L"Удаление выполенено успешно" << endl;


	SetCurrentDirectoryW(seconddirectory.c_str());

	wstring newfilename;
	wcout << L"Введите новое имя файла: ";
	getline(wcin, newfilename);

	if (!MoveFileW(filename.c_str(), ((newfilename.ends_with(L".txt")) ? newfilename : newfilename += L".txt").c_str())) {
		wcerr << L"Ошибка переименования. " + getErrMsg() << endl;
		exit(1);
	}
	wcout << L"Файл переименован успешно" << endl;


	HANDLE filetoread = CreateFileW(newfilename.c_str(), GENERIC_READ, false, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (filetoread == INVALID_HANDLE_VALUE) {
		wcerr << L"Ошибка открытия. " + getErrMsg() << endl;
		exit(1);
	}

	wchar_t* buffer = (wchar_t*)malloc(dwNumberOfBytes);

	if (!ReadFile(filetoread, buffer, dwNumberOfBytes, &dwNumberOfBytes, NULL)) {
		wcerr << L"Ошибка чтения. " + getErrMsg() << endl;
		CloseHandle(filetoread);
		exit(1);
	}

	CloseHandle(filetoread);

	buffer[texttowrite.size()] = 0;
	wcout << buffer << endl;

	return 0;
}

