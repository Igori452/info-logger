#include "ThreadManager.hpp"

ThreadManager::ThreadManager() : stopFlag(false), err(make_error_code(LoggerError::SUCCESS)) {};

void ThreadManager::push(const LoggerMessage& lgmsg) 
{
    std::lock_guard<std::mutex> lock {mt};

    if (stopFlag) 
    {
        return;
    }

    loggerMessageQueue.push(lgmsg);
    workerFlag.notify_one();
}

void ThreadManager::push(std::string_view message, MessageLevel messageLevel) 
{
    this->push(LoggerMessage {std::string {message}, messageLevel});
}

std::optional<LoggerMessage> ThreadManager::pop() 
{
    std::unique_lock<std::mutex> lock {mt};

    workerFlag.wait(lock, [this](){ 
        return !loggerMessageQueue.empty() || stopFlag;
    });

    if (err || (loggerMessageQueue.empty() && stopFlag)) 
    {
        return std::nullopt;
    }

    LoggerMessage lgmsg {std::move(loggerMessageQueue.front())};
    loggerMessageQueue.pop();

    return lgmsg;
}

void ThreadManager::stop() 
{
    std::lock_guard<std::mutex> lock {mt};

    if (stopFlag) 
    {
        return;
    }

    stopFlag = true;
    workerFlag.notify_all();
}

void ThreadManager::setError(std::error_code ec) 
{
    std::lock_guard<std::mutex> lock {mt};

    if (stopFlag) 
    {
        return;
    }

    err = ec;
    stopFlag = true;
    workerFlag.notify_all();
}

std::optional<std::error_code> ThreadManager::getError() const 
{
    std::lock_guard lock {mt};

    if (err)
    {
        return err;
    }
    return std::nullopt;
}