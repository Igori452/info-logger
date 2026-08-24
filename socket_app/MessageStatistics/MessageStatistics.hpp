#pragma once

#include "logger_lib/Logger.hpp"

#include <map>
#include <vector>

class MessageStatistics 
{
    private:
        using TimePoint = std::chrono::system_clock::time_point;

        bool dataChanged {false};

        size_t totalMessages {0};
        std::map<MessageLevel, size_t> totalLevelMessage;
        std::vector<TimePoint> totalMessagesAnHour;

        size_t minLenMessage {std::numeric_limits<size_t>::max()};
        size_t maxLenMessage {0};
        size_t averageLenMessage {0};

    public:
        MessageStatistics() = default;

        bool hasChanged();
        void setData(const LoggerMessage& lgmsg);

        size_t getTotalMessages() const;
        const std::map<MessageLevel, size_t>& getTotalLevelMessage() const;
        size_t getTotalMessagesAnHour() const;

        size_t getMinLenMessage() const;
        size_t getMaxLenMessage() const;
        size_t getAverageLenMessage() const;
};