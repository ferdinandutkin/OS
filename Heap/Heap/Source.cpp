#include <Windows.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
 
struct Close { typedef HANDLE pointer; void operator()(HANDLE h) { if (h != INVALID_HANDLE_VALUE)  HeapDestroy(h); } };
using unique_heap = std::unique_ptr<HANDLE, Close>;
 
int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
	int elcount = 16;
	int arrsize = elcount * sizeof(int);
	int hsize = 256;
	unique_heap heap(HeapCreate(HEAP_NO_SERIALIZE, hsize, hsize));
	int* arr = (int*)HeapAlloc(heap.get(), NULL, arrsize);

	if (arr == nullptr) {
		std::wcerr << L"������ �������� �������" << std::endl;
		exit(1);
	}
	for (int i = 0; i < elcount; i++) arr[i] = i;

 
	while (true) {
		_wsystem(L"cls");
		std::wcout << L"1) �������� ������\n2) ������� ������\n3) �������� � ��������\n4) ������� �� ��������\n5) ����������" << std::endl;
		switch (_getch()) {
		case '1': {
			arr = (int*)HeapReAlloc(heap.get(), NULL, arr, (arrsize = (++elcount) * sizeof(int)));
			int to_add;
			std::wcout << L"�������: ";
			std::cin >> to_add;
			std::wcout << L"������ " << (arr[elcount - 1] = to_add) << L" �������� � ������� ������" << std::endl;
		}
				break;
		case '2':
			std::wcout << L"������ " << arr[elcount - 1] << L" ������ ������" << std::endl;
			arr = (int*)HeapReAlloc(heap.get(), NULL, arr, (arrsize = (--elcount) * sizeof(int)));
			break;
		case '3': {
			std::string s_pos;
			std::wcout << L"�������: ";
			std::cin >> s_pos;
			if (std::all_of(s_pos.begin(), s_pos.end(), isdigit)) {
				int pos = std::stoi(s_pos);
				if (pos <= elcount - 1) {
					std::wcout << L"�������: ";
					int to_add;
					std::cin >> to_add;
					arr = (int*)HeapReAlloc(heap.get(), NULL, arr, (arrsize = (++elcount) * sizeof(int)));
					std::copy(arr + pos, arr + elcount - 1, arr + pos + 1);
					std::wcout << L"������� " << (arr[pos] = to_add) << L" �������� �� ������� " << pos << std::endl;					
				}
				else std::wcerr << L"�������� ������" << std::endl;
			}
			else std::wcerr << L"������������ ����" << std::endl;
		}
				break;

		case '4': {
			std::string s_pos;
			std::wcout << L"�������: ";
			std::cin >> s_pos;
			if (std::all_of(s_pos.begin(), s_pos.end(), isdigit)) {
				int pos = std::stoi(s_pos);
				if (pos <= elcount - 1) {
					std::wcout << L"������� " << arr[pos] << L" ������ � ������� " << pos << std::endl;
					std::copy(arr + pos + 1, arr + elcount + 1, arr + pos);
					arr = (int*)HeapReAlloc(heap.get(), NULL, arr, (arrsize = (--elcount) * sizeof(int)));	
				}
				else std::wcerr << L"�������� ������" << std::endl;
			}
			else std::wcerr << L"������������ ����" << std::endl;
		}
				break;
		case '5':
			for (int i = 0; i < elcount; i++) {
				std::wcout << L"arr[" << i << L"] = " << arr[i] << std::endl;
			}
			break;

		}
		_getch();

	}
}