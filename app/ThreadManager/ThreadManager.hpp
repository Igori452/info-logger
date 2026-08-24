#pragma once

#include "logger_lib/Logger.hpp"

#include <optional>
#include <queue>

#include <mutex>
#include <condition_variable>

/**
 * Потокобезопасный менеджер очереди сообщений.
 * 
 * Реализует паттерн Producer-Consumer с использованием std::mutex
 * и std::condition_variable. Обеспечивает безопасную остановку
 * и аварийный режим при критических ошибках.
 */
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

        /**
         * Добавляет сообщение в очередь.
         * Если менеджер остановлен, сообщение игнорируется.
         */
        void push(const LoggerMessage& lgmsg);
        void push(std::string_view message, MessageLevel messageLevel);

        /**
         * Извлекает сообщение из очереди.
         * 
         * Возвращает std::optional<LoggerMessage> или std::nullopt,
         * если очередь пуста или менеджер остановлен.
         */
        std::optional<LoggerMessage> pop();
        
        void stop();
        bool hasStop() const;

        /**
         * Устанавливает ошибку.
         * 
         * Критические ошибки (FILE_NOT_OPEN, WRITE_FAILED)
         * автоматически переводят менеджер в режим остановки (stopFlag = true).
         */
        void setError(std::error_code ec);
        std::optional<std::error_code> getError() const;
};