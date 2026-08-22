#pragma once

#include "logger_lib/include/Logger.hpp"

#include <optional>
#include <queue>

#include <mutex>
#include <condition_variable>

class ThreadManager 
{
    private:
        std::queue<LoggerMessage> loggerMessageQueue;
        
        mutable std::mutex mt;
        std::condition_variable workerFlag;
        bool stopFlag;

        std::error_code err;
        
    public:
        ThreadManager();

        void push(const LoggerMessage& lgmsg);
        void push(std::string_view message, MessageLevel messageLevel);

        std::optional<LoggerMessage> pop();
        
        void stop();

        void setError(std::error_code ec);
        std::optional<std::error_code> getError() const;

};