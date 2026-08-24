#include "logger_lib/LoggerMessage.hpp"

#include <sstream>
#include <iomanip>

LoggerMessage::LoggerMessage(std::string message_, MessageLevel messageLevel_) 
    : message(std::move(message_)), messageLevel(messageLevel_), 
      timeCreation(std::chrono::system_clock::now()) {}

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
    
    ss << "[ " << std::put_time(std::gmtime(&tt), "%Y-%m-%d %H:%M:%S") << " ] ";

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