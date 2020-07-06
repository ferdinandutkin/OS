#define _WINSOCK_DEPRECATED_NO_WARNINGS 
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


int main() {
    setlocale(0, "");

    win_sock start;
    hostent* host = gethostbyname("localhost");

    unique_socket client (socket(PF_INET, SOCK_STREAM, IPPROTO_TCP));

    if (client.get() == SOCKET_ERROR) {
        std::cerr << "[Клиент]: Ошибка создания сокета" << std::endl;
        return SOCKET_ERROR;
    }

    sockaddr_in serv_addr = { PF_INET, htons(1111), {*(reinterpret_cast<in_addr*>(*host->h_addr_list))} };


    if (connect(client.get(), reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "[Клиент]: Ошибка подключения" << std::endl;
        return SOCKET_ERROR;
    }
    
    char keyboard_input[256];
    std::cout << "[Клиент]: Введите строку для отправки на сервер: ";
    std::cin.getline(keyboard_input, 255);
   


    if (send(client.get(), keyboard_input, strlen(keyboard_input) * sizeof(char), 0) == SOCKET_ERROR) {
        std::cerr << "[Клиент]: Ошибка отправки" << std::endl;
        return SOCKET_ERROR;
    }
    Sleep(100);



    int* request = new int{0};

    while (true) {
        if (send(client.get(), reinterpret_cast<char*>(request), sizeof(int), 0) == SOCKET_ERROR) {
            std::cerr << "[Клиент]: Ошибка отправки" << std::endl;
            return SOCKET_ERROR;
        }
        std::cout << "[Клиент]: Отправлен запрос: " << *request << std::endl;
        

        int* response = new int;

        if (recv(client.get(), reinterpret_cast<char*>(response), sizeof(int), 0) == SOCKET_ERROR) {
            std::cerr << "[Клиент]: Ошибка чтения" << std::endl;
            return SOCKET_ERROR;
        }
        Sleep(100);
        std::cout << "[Клиент]: Получен ответ: " << *response << std::endl;

        *request = *response;

        delete response;

        Sleep(5000);

    }

    delete request;
}