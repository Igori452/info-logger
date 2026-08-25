#include "ServerRunner/ServerRunner.hpp"

#include <iostream>
#include <charconv>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/unistd.h>

int main(int argc, char* argv[]) 
{
    if (argc < 5) 
    {   
        std::cout << "Параметры ввода: <ip>, <port>, <T>, <N>\n";
        return 1;
    }

    std::string ipStr = argv[1];
    uint16_t port {0};
    size_t T {0};
    size_t N {0};
    
    auto resPort {std::from_chars(argv[2], argv[2] + std::strlen(argv[2]), port)};
    if (resPort.ec != std::errc{}) 
    {
        std::cout << "Ошибка! Неверный формат порта: " << argv[2] << "\n";
        return 1;
    }

    auto resT {std::from_chars(argv[3], argv[3] + std::strlen(argv[3]), T)};
    if (resT.ec != std::errc{}) 
    {
        std::cout << "Ошибка! Неверный формат таймаута T: " << argv[3] << "\n";
        return 1;
    }

    auto resN {std::from_chars(argv[4], argv[4] + std::strlen(argv[4]), N)};
    if (resN.ec != std::errc{}) 
    {
        std::cout << "Ошибка! Неверный формат количества сообщений N: " << argv[4] << "\n";
        return 1;
    }

    const auto IP_V = AF_INET;

    int listenSock = ::socket(IP_V, SOCK_STREAM, 0);
    if (listenSock == -1) 
    {
        std::cout << "[СЕРВЕР] Ошибка! Не удалось создать сокет.\n";
        return 1;
    }

    // Опция SO_REUSEADDR, чтобы порт не блокировался ОС после перезапуска сервера
    int opt = 1;
    ::setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr {};
    std::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = IP_V;
    serverAddr.sin_port = htons(port);

    auto closeSocket {[&listenSock](){
        ::close(listenSock);
        return 1;
    }};

    if (::inet_pton(IP_V, ipStr.c_str(), &serverAddr.sin_addr)  <= 0)
    {
        std::cout << "[СЕРВЕР] Ошибка! Некорректный IP-адрес: " << ipStr << "\n";
        return closeSocket();
    }

    if (::bind(listenSock, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0) 
    {
        std::cout << "[СЕРВЕР] Ошибка bind! Возможно, порт " << port << " уже занят.\n";
        return closeSocket();
    }

    // Переводим сокет в режим прослушивания (максимум 5 подключений в очереди)
    if (::listen(listenSock, 5) < 0) 
    {
        std::cout << "[СЕРВЕР] Ошибка listen!\n";
        return closeSocket();
    }

    while (true) 
    {
        std::cout << "[СЕРВЕР] Ожидание подключения логгера...\n";

        sockaddr_in clientAddr {};
        socklen_t clientSize {sizeof(clientAddr)};

        int clientSock = ::accept(listenSock, reinterpret_cast<sockaddr*>(&clientAddr), &clientSize);
        if (clientSock < 0)
        {
            continue;
        }

        std::cout << "[СЕРВЕР] Логгер успешно подключился! Запуск ранера...\n";

        ServerRunner::run(clientSock, T, N, std::cout);

        ::close(clientSock);
        std::cout << "[СЕРВЕР] Сессия завершена. Ресурсы очищены.\n\n";
    }

    ::close(listenSock);
    return 0;
}