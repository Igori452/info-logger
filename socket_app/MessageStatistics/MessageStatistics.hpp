#pragma once

#include "logger_lib/Logger.hpp"

#include <map>
#include <vector>


/**
 * Класс для сбора и расчета статистики по логируемым сообщениям.
 * 
 * Накапливает информацию о количестве сообщений, распределении по уровням важности,
 * частоте логирования за последний час, а также вычисляет длину сообщений.
 */
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
        double averageLenMessage {0.0};

    public:
        MessageStatistics() = default;

        bool hasChanged();
        void setData(const LoggerMessage& lgmsg);

        size_t getTotalMessages() const;
        const std::map<MessageLevel, size_t>& getTotalLevelMessage() const;
        size_t getTotalMessagesAnHour() const;

        size_t getMinLenMessage() const;
        size_t getMaxLenMessage() const;
        double getAverageLenMessage() const;
};