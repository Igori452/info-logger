#pragma once

#include "OutputInterface.hpp"
#include "LoggerMessage.hpp"

#include <memory>

class Logger 
{
    private:
        MessageLevel defaultMessageLevel;
        std::unique_ptr<OutputInterface> out;

    public:
        Logger() = delete;
        Logger(MessageLevel defaultMessageLevel, std::unique_ptr<OutputInterface> out_);
        Logger(MessageLevel defaultMessageLevel, std::string_view journalName);

        void setDefaultMessageLevel(MessageLevel defaultMessageLevel_);
        MessageLevel getDefaultMessageLevel() const;
        
        std::error_code log(const LoggerMessage& lgmsg);
        std::error_code log(std::string_view message, MessageLevel messageLevel);
};