#include "logger_lib/Logger.hpp"

Logger::Logger(MessageLevel defaultMessageLevel, std::unique_ptr<OutputInterface> out_) 
    : defaultMessageLevel(defaultMessageLevel), out(std::move(out_)) {}

Logger::Logger(MessageLevel defaultMessageLevel, std::string_view journalName) 
    : Logger(defaultMessageLevel, std::make_unique<OFileInterface>(journalName)) {}

Logger::Logger(MessageLevel defaultMessageLevel, int socket) 
    : Logger(defaultMessageLevel, std::make_unique<OSocketInterface>(socket)) {}

void Logger::setDefaultMessageLevel(MessageLevel defaultMessageLevel_) 
{
    defaultMessageLevel = defaultMessageLevel_;
}

MessageLevel Logger::getDefaultMessageLevel() const 
{
    return defaultMessageLevel;
}

std::error_code Logger::log(const LoggerMessage& lgmsg) 
{
    if (lgmsg.getMessageLevel() < defaultMessageLevel)
    {
        return make_error_code(LoggerError::FILTERED);
    }

    return out->write(LoggerMessageFormater::formatToText(lgmsg));
}

std::error_code Logger::log(std::string_view message, MessageLevel messageLevel) 
{
    return this->log(LoggerMessage {std::string {message}, messageLevel});
}