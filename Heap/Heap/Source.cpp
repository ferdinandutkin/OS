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
		std::wcerr << L"Ошибка создания массива" << std::endl;
		exit(1);
	}
	for (int i = 0; i < elcount; i++) arr[i] = i;

 
	while (true) {
		_wsystem(L"cls");
		std::wcout << L"1) Добавить справа\n2) Удалить справа\n3) Добавить в середину\n4) Удалить из середины\n5) Отобразить" << std::endl;
		switch (_getch()) {
		case '1': {
			arr = (int*)HeapReAlloc(heap.get(), NULL, arr, (arrsize = (++elcount) * sizeof(int)));
			int to_add;
			std::wcout << L"Элемент: ";
			std::cin >> to_add;
			std::wcout << L"Элемет " << (arr[elcount - 1] = to_add) << L" добавлен к массиву справа" << std::endl;
		}
				break;
		case '2':
			std::wcout << L"Элемет " << arr[elcount - 1] << L" удален справа" << std::endl;
			arr = (int*)HeapReAlloc(heap.get(), NULL, arr, (arrsize = (--elcount) * sizeof(int)));
			break;
		case '3': {
			std::string s_pos;
			std::wcout << L"Позиция: ";
			std::cin >> s_pos;
			if (std::all_of(s_pos.begin(), s_pos.end(), isdigit)) {
				int pos = std::stoi(s_pos);
				if (pos <= elcount - 1) {
					std::wcout << L"Элемент: ";
					int to_add;
					std::cin >> to_add;
					arr = (int*)HeapReAlloc(heap.get(), NULL, arr, (arrsize = (++elcount) * sizeof(int)));
					std::copy(arr + pos, arr + elcount - 1, arr + pos + 1);
					std::wcout << L"Элемент " << (arr[pos] = to_add) << L" добавлен на позицию " << pos << std::endl;					
				}
				else std::wcerr << L"Неверный индекс" << std::endl;
			}
			else std::wcerr << L"Некорректный ввод" << std::endl;
		}
				break;

		case '4': {
			std::string s_pos;
			std::wcout << L"Позиция: ";
			std::cin >> s_pos;
			if (std::all_of(s_pos.begin(), s_pos.end(), isdigit)) {
				int pos = std::stoi(s_pos);
				if (pos <= elcount - 1) {
					std::wcout << L"Элемент " << arr[pos] << L" удален с позиции " << pos << std::endl;
					std::copy(arr + pos + 1, arr + elcount + 1, arr + pos);
					arr = (int*)HeapReAlloc(heap.get(), NULL, arr, (arrsize = (--elcount) * sizeof(int)));	
				}
				else std::wcerr << L"Неверный индекс" << std::endl;
			}
			else std::wcerr << L"Некорректный ввод" << std::endl;
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