#include <Windows.h>
#include <iostream>
#include <iomanip>
 


using unique_handle = std::unique_ptr<std::remove_pointer<HANDLE>::type, std::integral_constant<decltype(&CloseHandle), &CloseHandle>>;



HANDLE start(wchar_t* name) {
	return CreateThread(nullptr, NULL, [](void* name) -> DWORD {
		STARTUPINFO si = { 0 };
		PROCESS_INFORMATION pi = { 0 };
		si.cb = sizeof(STARTUPINFO);
		if (!CreateProcessW(NULL, static_cast<wchar_t*>(name), nullptr, nullptr, false, NULL, nullptr, nullptr, &si, &pi)) {
			std::cerr << "[Server]: Oops! Client is dead for some reason. " << std::endl;
			exit(1);
		}
		return 0;
		}, name, NULL, nullptr);
}


int main() {
	char response[] = "[Server]: Here is my response.";
	
	HANDLE pipe = CreateNamedPipeW(L"\\\\.\\pipe\\Pipe", PIPE_ACCESS_DUPLEX, PIPE_WAIT | PIPE_TYPE_BYTE | PIPE_READMODE_BYTE, 1, 1024 * 16, 1024 * 16, 0,  NULL);

	wchar_t client_name[] = L"client.exe";

	unique_handle client(start(client_name));

	if (pipe != INVALID_HANDLE_VALUE) {
		if (ConnectNamedPipe(pipe, NULL)){

			char message[32];
			DWORD bytes_read;

			if (ReadFile(pipe, message, sizeof(message) - sizeof(char), &bytes_read, NULL)) {
				 
				message[bytes_read / sizeof(char)] = '\0';

				std::cout << "[Server]: Message" << std::quoted(message) << " read." << std::endl;
			}
		}

		WriteFile(pipe, response, strlen(response) * sizeof(char),  nullptr, NULL);

		DisconnectNamedPipe(pipe);
	}


}

