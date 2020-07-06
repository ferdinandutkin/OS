#include <windows.h>
#include <iostream>
#include <chrono>

 
DWORD WINAPI foo(void* info) {

	int* turn = (static_cast<std::pair<int*, bool>*>(info))->first;
	bool id = (static_cast<std::pair<int*, bool>*>(info))->second;

	auto start = std::chrono::system_clock::now();

	while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() < 5) {
		if (((*turn) & 1) == id) {
			std::cout << "Thread " << id + 1 << " says: " << ((id) ? (*turn) + 20 : (*turn) - 20) << std::endl;
			(*turn)++;
			Sleep(500);
		}
	}
	return 0;
};

void init() {
	int* turn = new int(0);

	HANDLE threads[2] = { CreateThread(0, 0, foo, &std::make_pair(turn, true), 0, nullptr), CreateThread(0, 0, foo, &std::make_pair(turn, false), 0, nullptr) };

	WaitForMultipleObjects(2, threads, TRUE, INFINITE);
}

void init1() {

	HANDLE writer = CreateThread(nullptr, 0, [](void*) -> DWORD {

		int* counter = new int(0);

		HANDLE reader = CreateThread(nullptr, 0, [](void* counter) -> DWORD {

			auto start = std::chrono::system_clock::now(); //спасибо но никаких std::chrono::time_point<std::chrono::system_clock> 

			while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() < 5) {

				std::cout << *static_cast<int*>(counter) << std::endl;
				SuspendThread(GetCurrentThread());
			}

			return 0;
			}, counter, CREATE_SUSPENDED, nullptr);

		auto start = std::chrono::system_clock::now();

		while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start).count() < 5) {

			Sleep(100);
			ResumeThread(reader);
			(*counter)++;

		}
		WaitForSingleObject(reader, 500);
		CloseHandle(reader);
		return 0;
		}, nullptr, 0, nullptr);

	WaitForSingleObject(writer, INFINITE);
	CloseHandle(writer);

}


int main() {
	SetConsoleOutputCP(1251);
	std::cout << "Попеременный ввод" << std::endl << std::endl;
	init();
	std::cout << std::endl << "Читатель и писатель" << std::endl << std::endl;
	init1();
	return 0;

}



 