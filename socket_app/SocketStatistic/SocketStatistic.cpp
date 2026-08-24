#include "SocketStatistic.hpp"

#include <algorithm>

bool MessageStatistic::hasChanged() 
{
    bool dataChanged_ {dataChanged};
    dataChanged = false;
    return dataChanged_;
}

void MessageStatistic::setData(const LoggerMessage& lgmsg) 
{
    dataChanged = true;

    ++totalMessages, ++totalLevelMessage[lgmsg.getMessageLevel()];

    const TimePoint now = std::chrono::system_clock::now();
    totalMessagesAnHour.emplace_back(now);

    const auto oneHourAgo = now - std::chrono::hours(1);
    const auto removeIt = std::remove_if(std::begin(totalMessagesAnHour), std::end(totalMessagesAnHour), [oneHourAgo](const TimePoint& tmp){
        return oneHourAgo >= tmp;
    });
    totalMessagesAnHour.erase(removeIt, std::end(totalMessagesAnHour));

    const size_t messageLen = LoggerMessageFormater::formatToText(lgmsg).size();

    minLenMessage = std::min(messageLen, minLenMessage);
    maxLenMessage = std::max(messageLen, maxLenMessage);
    averageLenMessage = (averageLenMessage * (totalMessages - 1) + messageLen) / totalMessages;
}

void MessageStatistic::printStatistic(std::ostream& os) const 
{
    os << "\n========================================\n";
    os << "          LOGS STATISTICS REPORT        \n";
    os << "========================================\n";
    
    os << " [Quantity Metrics]\n";
    os << "   - Сообщений всего:      " << totalMessages << "\n";
    
    auto getLevelCount = [this](MessageLevel lvl) {
        auto it = totalLevelMessage.find(lvl);
        return (it != totalLevelMessage.end()) ? it->second : 0;
    };
    
    os << "   - Сообщений по уровню важности:\n";
    os << "     * INFO:              " << getLevelCount(MessageLevel::INFO) << "\n";
    os << "     * WARNING:           " << getLevelCount(MessageLevel::WARNING) << "\n";
    os << "     * ERROR:             " << getLevelCount(MessageLevel::ERROR) << "\n";
    os << "   - Сообщений за последний час:    " << totalMessagesAnHour.size() << "\n";
    
    os << "----------------------------------------\n";
    
    os << " [Статистика длин сообщений (кол-во символов)]\n";
    os << "   - Минимальная длина:      " << (totalMessages == 0 ? 0 : minLenMessage) << "\n";
    os << "   - Максимальная длина:     " << maxLenMessage << "\n";
    os << "   - Средняя длина:          " << averageLenMessage << "\n";
    os << "========================================\n\n";
}