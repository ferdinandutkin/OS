#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")

struct win_sock {
    WSADATA wsa_data;
    win_sock() {
        WSAStartup(0x202, &wsa_data);
    }
    ~win_sock() {
        WSACleanup();
    }
};

struct socket_deleter {
    using pointer = SOCKET;

    void operator()(SOCKET h) {
        closesocket(h);
    }
};

using unique_socket = std::unique_ptr<SOCKET, socket_deleter>;

HANDLE run(wchar_t* name) {
    return CreateThread(nullptr, NULL, [](void* name) -> DWORD {
        STARTUPINFO si = { 0 };
        PROCESS_INFORMATION pi = { 0 };
        si.cb = sizeof(STARTUPINFO);
        if (!CreateProcessW(NULL, static_cast<wchar_t*>(name), nullptr, nullptr, false, NULL, nullptr, nullptr, &si, &pi)) {
            std::cerr << "[Сервер]: Ошибка создания клиента " << std::endl;
            exit(1);
        }
        return 0;
        }, name, NULL, nullptr);
}



int main() {
    setlocale(0, "");
    win_sock start;

    wchar_t client_name[] = L"client.exe";
    run(client_name);


    unique_socket server(socket(PF_INET, SOCK_STREAM, IPPROTO_TCP));

    if (server.get() == INVALID_SOCKET) {
        std::cerr << "[Сервер]: Ошибка создания сокета" << std::endl;
        return SOCKET_ERROR;
    }

    sockaddr_in sock_addr = { PF_INET, htons(1111), {INADDR_ANY} };

    if (bind(server.get(), reinterpret_cast<sockaddr*>(&sock_addr), sizeof(sockaddr_in)) == SOCKET_ERROR) {
        std::cerr << "[Сервер]: Ошибка связывания" << std::endl;
        return SOCKET_ERROR;
    }


    if (listen(server.get(), SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "[Сервер]: Ошибка при ожидании запроса" << std::endl;
        return SOCKET_ERROR;
    }



    unique_socket client(accept(server.get(), NULL, NULL));

    if (client.get() == INVALID_SOCKET) {
        std::cerr << "[Сервер]: Ошибка принятия запроса" << std::endl; 
        return SOCKET_ERROR;
    }

    char* keyboard_request = new char[256];
    ZeroMemory(keyboard_request, 256 * sizeof(char));

    if (recv(client.get(), keyboard_request, 255 * sizeof(char), 0) == SOCKET_ERROR) {
        std::cerr << "[Сервер]: Ошибка чтения" << std::endl;
        return SOCKET_ERROR;
    }

    std::cout << "[Сервер]: Получен запрос: " << keyboard_request << std::endl;
    Sleep(200);

    while (true) {
        int* request = new int;
      

        if (recv(client.get(), reinterpret_cast<char*>(request), sizeof(int), 0) == SOCKET_ERROR) {
            std::cerr << "[Сервер]: Ошибка чтения" << std::endl;
            return SOCKET_ERROR;

        }
        Sleep(100);
        std::cout << "[Сервер]: Получен запрос: " << *request << std::endl;

        int* response = new int{ *request };

        (*response)++;


        if (send(client.get(), reinterpret_cast<char*>(response), sizeof(int), 0) == SOCKET_ERROR) {
            std::cerr << "[Сервер]: Ошибка отправки" << std::endl;
            return SOCKET_ERROR;
        } 

        std::cout << "[Сервер]: Отправлен ответ: " << *response << std::endl;

        delete request;
        delete response;

    }





    

}