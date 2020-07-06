#include <iomanip>
#include <Windows.h>
#include <iostream>


int main() {

	HANDLE pipe = CreateFileW(L"\\\\.\\pipe\\Pipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (pipe != INVALID_HANDLE_VALUE) {

		char message[] = "[Client]: Here is my message.";
	
		WriteFile(pipe, message, strlen(message) * sizeof(char),  nullptr, NULL);

		char response[32];
		DWORD bytes_read;


		if (ReadFile(pipe, response, sizeof(response) - sizeof(char), &bytes_read, NULL)) {

			response[bytes_read / sizeof(char)] = '\0';

			std::cout << "[Client]: Message " << std::quoted(response) << " read." << std::endl;
		}
		 
		CloseHandle(pipe);
	}


	 
}
