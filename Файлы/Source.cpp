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
	return buf; //���� ez implicit conversion
}

int main() {

	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);


	wstring firstdirectory, seconddirectory;
	wcout << L"������� ��� ������ ����������: ";
	getline(wcin, firstdirectory);
	wcout << L"������� ��� ������ ����������: ";
	getline(wcin, seconddirectory);

	if (!((CreateDirectoryW(firstdirectory.c_str(), NULL)) && (CreateDirectoryW(seconddirectory.c_str(), NULL)))) {
		wcerr << L"������ �������� ����������. " + getErrMsg() << endl;
		exit(1);
	}
	wcout << L"���������� ������� �������" << endl;


	SetCurrentDirectoryW(firstdirectory.c_str());

	wstring filename;
	wcout << L"������� ��� ���������� ����� ��� �������� � ������ ����������: ";
	getline(wcin, filename);

	HANDLE filetowrite = CreateFileW(((filename.ends_with(L".txt")) ? filename : filename += L".txt").c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);


	if (filetowrite == INVALID_HANDLE_VALUE) {
		wcerr << L"������ �������� �����. " + getErrMsg() << endl;
		exit(1);
	}
	wcout << L"���� ������ �������" << endl;


	wcout << L"������� ����� ��� ������ � ����: ";
	wstring texttowrite((istreambuf_iterator<wchar_t>(wcin)), istreambuf_iterator<wchar_t>()); //Ctrl+C �������� EOF
	clearerr(stdin);

	DWORD dwNumberOfBytes;

	if (!WriteFile(filetowrite, texttowrite.c_str(), texttowrite.size() * sizeof(wchar_t), &dwNumberOfBytes, NULL)) {
		wcerr << L"������ ������. " + getErrMsg() << endl;
		CloseHandle(filetowrite);
		exit(1);
	}
	wcout << L"������ ��������� �������" << endl;
	CloseHandle(filetowrite);


	if (!CopyFileW(filename.c_str(), (seconddirectory + L'\\' + filename).c_str(), NULL)) {
		wcerr << L"������ �����������. " + getErrMsg() << endl;
		exit(1);
	}
	wcout << L"����������� ���������� �������" << endl;


	if (!DeleteFileW(filename.c_str())) {
		wcerr << L"������ ��������. " + getErrMsg() << endl;
		exit(1);
	}
	wcout << L"�������� ���������� �������" << endl;


	SetCurrentDirectoryW(seconddirectory.c_str());

	wstring newfilename;
	wcout << L"������� ����� ��� �����: ";
	getline(wcin, newfilename);

	if (!MoveFileW(filename.c_str(), ((newfilename.ends_with(L".txt")) ? newfilename : newfilename += L".txt").c_str())) {
		wcerr << L"������ ��������������. " + getErrMsg() << endl;
		exit(1);
	}
	wcout << L"���� ������������ �������" << endl;


	HANDLE filetoread = CreateFileW(newfilename.c_str(), GENERIC_READ, false, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (filetoread == INVALID_HANDLE_VALUE) {
		wcerr << L"������ ��������. " + getErrMsg() << endl;
		exit(1);
	}

	wchar_t* buffer = (wchar_t*)malloc(dwNumberOfBytes);

	if (!ReadFile(filetoread, buffer, dwNumberOfBytes, &dwNumberOfBytes, NULL)) {
		wcerr << L"������ ������. " + getErrMsg() << endl;
		CloseHandle(filetoread);
		exit(1);
	}

	CloseHandle(filetoread);

	buffer[texttowrite.size()] = 0;
	wcout << buffer << endl;

	return 0;
}

