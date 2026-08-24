#include "SocketReceiver.hpp"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <poll.h>

#include <vector>

SocketReceiver::SocketReceiver(int socketObject_) 
    : socketObject(socketObject_) {}

std::error_code SocketReceiver::waitData(size_t timeoutSeconds) const 
{
    pollfd pfd {};
    pfd.fd = socketObject;
    pfd.events = POLLIN;

    int pollAction = ::poll(&pfd, 1, timeoutSeconds * 1000);

    // Ошибка
    if (pollAction < 0) 
    {
        return std::error_code(errno, std::generic_category());
    }

    // Таймаут
    if (pollAction == 0) 
    {
        return std::make_error_code(std::errc::timed_out);
    }

    // Данные пришли, но это событие ошибки сокета 
    if (pfd.revents & (POLLERR | POLLHUP | POLLNVAL)) 
    {
        return std::make_error_code(std::errc::broken_pipe);
    }

    return std::error_code {};
}

bool SocketReceiver::readFromNetwork()
{
    int bytesAvailable {0};
    if (::ioctl(socketObject, FIONREAD, &bytesAvailable) < 0 || bytesAvailable == 0)
    {
        return false;
    } 

    std::vector<char> msgBuffer (bytesAvailable);
    ssize_t bytesRecv = ::recv(socketObject, msgBuffer.data(), msgBuffer.size(), 0);

    if (bytesRecv <= 0)
    {
        return false;
    }

    streamBuffer.append(msgBuffer.data(), bytesRecv);
    
    return true;
}

std::optional<std::string> SocketReceiver::getNextMessage(const char separator) 
{
    size_t msgPos = streamBuffer.find(separator);
    if (msgPos == std::string::npos) 
    {
        return std::nullopt;
    }

    std::string outMessage {streamBuffer.substr(0, msgPos + 1)};
    streamBuffer.erase(0, msgPos + 1);
    return outMessage;
}