#include "MessageStatistics.hpp"

#include <algorithm>

bool MessageStatistics::hasChanged() 
{
    bool dataChanged_ {dataChanged};
    dataChanged = false;
    return dataChanged_;
}

void MessageStatistics::setData(const LoggerMessage& lgmsg) 
{
    dataChanged = true;

    ++totalMessages, ++totalLevelMessage[lgmsg.getMessageLevel()];

    totalMessagesAnHour.emplace_back(lgmsg.getTimeCreation());

    const auto oneHourAgo = std::chrono::system_clock::now() - std::chrono::hours(1);
    const auto removeIt = std::remove_if(std::begin(totalMessagesAnHour), std::end(totalMessagesAnHour), [oneHourAgo](const TimePoint& tmp){
        return oneHourAgo > tmp;
    });
    totalMessagesAnHour.erase(removeIt, std::end(totalMessagesAnHour));

    const size_t messageLen = LoggerMessageFormater::formatToText(lgmsg).size();

    minLenMessage = std::min(messageLen, minLenMessage);
    maxLenMessage = std::max(messageLen, maxLenMessage);
    averageLenMessage = (averageLenMessage * (totalMessages - 1) + messageLen) / totalMessages;
}

size_t MessageStatistics::getTotalMessages() const 
{
    return totalMessages;
}
const std::map<MessageLevel, size_t>& MessageStatistics::getTotalLevelMessage() const 
{
    return totalLevelMessage;
}
size_t MessageStatistics::getTotalMessagesAnHour() const 
{
    const auto oneHourAgo = std::chrono::system_clock::now() - std::chrono::hours(1);
    return std::count_if(std::begin(totalMessagesAnHour), std::end(totalMessagesAnHour), 
        [oneHourAgo](const TimePoint& tmp) {
            return tmp > oneHourAgo;
        });
}

size_t MessageStatistics::getMinLenMessage() const 
{
    return (totalMessages == 0 ? 0 : minLenMessage);
}

size_t MessageStatistics::getMaxLenMessage() const 
{
    return maxLenMessage;
}

double MessageStatistics::getAverageLenMessage() const 
{
    return averageLenMessage;
}