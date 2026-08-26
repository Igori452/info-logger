#include "logger_lib/LoggerMessage.hpp"

#include <sstream>
#include <iomanip>
#include <ctime>

LoggerMessage::LoggerMessage(std::string message_, MessageLevel messageLevel_) 
    : message(std::move(message_)), messageLevel(messageLevel_), 
      timeCreation(std::chrono::system_clock::now()) {}

LoggerMessage::LoggerMessage(std::string message_, MessageLevel messageLevel_, TimePoint timeCreation_)
    : message(std::move(message_)), messageLevel(messageLevel_), timeCreation(timeCreation_) {}

const std::string& LoggerMessage::getMessage() const 
{
    return message;
}

MessageLevel LoggerMessage::getMessageLevel() const 
{
    return messageLevel;
}

const LoggerMessage::TimePoint& LoggerMessage::getTimeCreation() const 
{
    return timeCreation;
}

std::string LoggerMessageFormater::formatToText(const LoggerMessage& lgmsg) 
{
    std::time_t tt = std::chrono::system_clock::to_time_t(lgmsg.getTimeCreation());

    std::stringstream ss;
    
    tt += timeMSKCoff; 

    ss << "[ " << std::put_time(std::gmtime(&tt), timeMask.data()) << " ] ";

    std::string_view messageLevelString;
    switch (lgmsg.getMessageLevel())
    {
        case MessageLevel::INFO:    messageLevelString = "INFO   "; break;
        case MessageLevel::WARNING: messageLevelString = "WARNING"; break;
        case MessageLevel::ERROR:   messageLevelString = "ERROR  "; break;
    }

    ss << "[ " << messageLevelString << " ] " << lgmsg.getMessage() << ".\n";

    return ss.str();
}

std::optional<LoggerMessage> LoggerMessageFormater::formatToLoggerMessage(std::string_view formatedMessage) 
{
    // Минимальная длина: префикс (36) + точка с переводом строки (2) + минимум 1 символ сообщения = 39
    if (formatedMessage.size() < 39) 
    {
        return std::nullopt;
    }

    std::tm tm {};
    std::stringstream ss {std::string {formatedMessage.substr(2, 20)}};
    ss >> std::get_time(&tm, timeMask.data());

    if (ss.fail()) 
    {
        return std::nullopt;
    }

    tm.tm_isdst = -1; 
    std::time_t tt = ::timegm(&tm);
    tt -= timeMSKCoff; 

    auto messageTimeCreation = std::chrono::system_clock::from_time_t(tt);

    std::string levelMessageStr {formatedMessage.substr(26, 7)};

    MessageLevel messageLevelMessage {};
    if (levelMessageStr.find("INFO") != std::string::npos) 
    {
        messageLevelMessage = MessageLevel::INFO;
    }
    else if (levelMessageStr.find("WARNING") != std::string::npos) 
    {
        messageLevelMessage = MessageLevel::WARNING;
    }
    else if (levelMessageStr.find("ERROR")!= std::string::npos) 
    {   
        messageLevelMessage = MessageLevel::ERROR;
    }
    else 
    {
        return std::nullopt;
    }
 
    std::string message {formatedMessage.substr(36, formatedMessage.size() - 36 - 2)};
    
    return LoggerMessage {message, messageLevelMessage, messageTimeCreation};
}