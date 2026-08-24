#include "logger_lib/OutputInterface.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

OFileInterface::OFileInterface(std::string_view path_) 
    : path(std::filesystem::path(path_)), ofile(path, std::ios::app) {}

std::error_code OFileInterface::write(std::string_view message) 
{
    if (!ofile.is_open()) 
    {
        return make_error_code(LoggerError::FILE_NOT_OPEN);
    }

    ofile << message;

    if (!ofile) 
    {
        return make_error_code(LoggerError::WRITE_FAILED);
    }

    return make_error_code(LoggerError::SUCCESS);
}

OSocketInterface::OSocketInterface(std::string_view ip_, uint16_t port_) 
    : ip(std::string {ip_}), port(port_), socketObject(-1) {}

std::error_code OSocketInterface::connectToServer() 
{
    if (socketObject != -1)
    {
        return make_error_code(LoggerError::SUCCESS);
    }

    const auto IP_V = AF_INET;

    // Последний параметр - 0 означает выбрать протокол по умолчанию (TCP) для сочетания IpV4 + SOCK_STREAM
    socketObject = ::socket(IP_V, SOCK_STREAM, 0);
    if (socketObject == -1)
    {
        return std::error_code(errno, std::generic_category());
    }

    sockaddr_in server_addr {};

    // Явно зануляем память внтутри server_addr
    std::memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = IP_V;
    server_addr.sin_port = htons(port);


    if (::inet_pton(IP_V, ip.c_str(), &server_addr.sin_addr) <= 0) 
    {
        disconnect();
        return std::error_code(EINVAL, std::generic_category());
    }

    if (::connect(socketObject, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) < 0) 
    {
        int err {errno};
        disconnect();
        return std::error_code(err, std::generic_category());
    }

    return make_error_code(LoggerError::SUCCESS);
}

void OSocketInterface::disconnect() 
{
    if (socketObject != -1) 
    {
        ::close(socketObject);
        socketObject = -1;
    }
}

std::error_code OSocketInterface::write(std::string_view message) 
{
    if (socketObject == -1) 
    {
        if (const std::error_code err {connectToServer()}; err) 
        {
            return err;
        }
    }

    size_t sentSize {0};
    size_t sizeToSend {message.size()};
    const char* messagePtrToSend {message.data()};

    while(sentSize < sizeToSend) 
    {
        // Флаг MSG_NOSIGNAL запрещает ОС генерировать сигнал SIGPIPE, 
        // если сервер внезапно разорвал соединение. Вместо падения приложения мы получим ошибку EPIPE
        ssize_t sent = ::send(socketObject, messagePtrToSend + sentSize, sizeToSend - sentSize, MSG_NOSIGNAL);
    
        if (sent < 0) 
        {
            int err {errno};
            disconnect();
            return std::error_code(err, std::generic_category());
        }

        sentSize += sent;
    }

    return make_error_code(LoggerError::SUCCESS);
}

OSocketInterface::OSocketInterface(OSocketInterface&& other) noexcept 
    : ip(std::move(other.ip)), port(other.port), socketObject(other.socketObject)
{
    other.socketObject = -1;
}
OSocketInterface& OSocketInterface::operator=(OSocketInterface&& other) noexcept 
{
    if (this != &other) 
    {
        ip = std::move(other.ip);
        port = other.port;
        socketObject = other.socketObject;

        other.socketObject = -1;
    }

    return *this;
}


OSocketInterface::~OSocketInterface() 
{
    disconnect();
}