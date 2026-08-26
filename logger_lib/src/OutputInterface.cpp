#include "logger_lib/OutputInterface.hpp"

#include <sys/socket.h>
#include <netinet/in.h>

#include <cstring>

OFileInterface::OFileInterface(std::string_view path_) 
    : ofile(std::string {path_}, std::ios::app) {}

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

OSocketInterface::OSocketInterface(int socketObject_) : socketObject(socketObject_) {}

std::error_code OSocketInterface::write(std::string_view message) 
{
    if (socketObject == -1) 
    {
        return make_error_code(LoggerError::INVALID_SOCKET_VAL);
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
            return std::error_code(errno, std::generic_category());
        }

        sentSize += sent;
    }

    return make_error_code(LoggerError::SUCCESS);
}

OSocketInterface::OSocketInterface(OSocketInterface&& other) noexcept 
    : socketObject(other.socketObject)
{
    other.socketObject = -1;
}
OSocketInterface& OSocketInterface::operator=(OSocketInterface&& other) noexcept 
{
    if (this != &other) 
    {
        socketObject = other.socketObject;

        other.socketObject = -1;
    }

    return *this;
}