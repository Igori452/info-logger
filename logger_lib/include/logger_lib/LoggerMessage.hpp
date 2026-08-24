#pragma once

#include <chrono>
#include <string>

/**
 * Уровни важности сообщений логирования.
 */
enum class MessageLevel {INFO = 0, WARNING = 1, ERROR = 2};

/**
 * Неизменяемый объект сообщения с автоматической фиксацией времени создания.
 * 
 * Время создания фиксируется в конструкторе через std::chrono::system_clock.
 * Все поля доступны только для чтения через геттеры.
 */
class LoggerMessage 
{
    private:
        using TimePoint = std::chrono::system_clock::time_point;

        std::string message;
        MessageLevel messageLevel;
        TimePoint timeCreation;

    public:
        LoggerMessage() = delete;
        LoggerMessage(std::string message_, MessageLevel messageLevel_);

        const std::string& getMessage() const;
        MessageLevel getMessageLevel() const;
        const TimePoint& getTimeCreation() const;
};

/**
 * Класс-фабрика для форматирования сообщений в текстовый вид.
 */
class LoggerMessageFormater 
{
    public:

        // [2026-08-21 14:54:44] [ INFO    ] message.
        // [2026-08-21 14:54:44] [ WARNING ] message.
        // [2026-08-21 14:54:44] [ ERROR   ] message.
        static std::string formatToText(const LoggerMessage& lgmsg);
};