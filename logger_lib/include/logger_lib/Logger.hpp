#pragma once

#include "OutputInterface.hpp"
#include "LoggerMessage.hpp"

#include <memory>

/**
 * Синхронный логгер с фильтрацией по уровню важности.
 * 
 * Делегирует запись через полиморфный интерфейс OutputInterface.
 * Все ошибки возвращаются через std::error_code (без исключений).
 */

class Logger 
{
    private:
        MessageLevel defaultMessageLevel;
        std::unique_ptr<OutputInterface> out;

    public:
        Logger() = delete;
        Logger(MessageLevel defaultMessageLevel, std::unique_ptr<OutputInterface> out_);
        Logger(MessageLevel defaultMessageLevel, std::string_view journalName);
        Logger(MessageLevel defaultMessageLevel, std::string_view ip, uint16_t port);

        void setDefaultMessageLevel(MessageLevel defaultMessageLevel_);
        MessageLevel getDefaultMessageLevel() const;
        
        std::error_code log(const LoggerMessage& lgmsg);
        std::error_code log(std::string_view message, MessageLevel messageLevel);
};