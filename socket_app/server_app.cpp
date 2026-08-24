#include "MessageStatistics/MessageStatistics.hpp"
#include "SocketReceiver/SocketReceiver.hpp"

void printStatistic(const MessageStatistics& msgStat, std::ostream os) 
{
    os << "\n========================================\n";
    os << "          LOGS STATISTICS REPORT        \n";
    os << "========================================\n";
    
    os << " [Quantity Metrics]\n";
    os << "   - Сообщений всего:      " << msgStat.getTotalMessages() << "\n";
    
    auto msgTotalLevel = msgStat.getTotalLevelMessage();

    auto getLevelCount = [&msgTotalLevel](MessageLevel lvl) {
        auto it = msgTotalLevel.find(lvl);
        return (it != msgTotalLevel.end()) ? it->second : 0;
    };
    
    os << "   - Сообщений по уровню важности:\n";
    os << "     * INFO:              " << getLevelCount(MessageLevel::INFO) << "\n";
    os << "     * WARNING:           " << getLevelCount(MessageLevel::WARNING) << "\n";
    os << "     * ERROR:             " << getLevelCount(MessageLevel::ERROR) << "\n";
    os << "   - Сообщений за последний час:    " << msgStat.getTotalMessagesAnHour() << "\n";
    
    os << "----------------------------------------\n";
    
    os << " [Статистика длин сообщений (кол-во символов)]\n";
    os << "   - Минимальная длина:      " << msgStat.getMinLenMessage() << "\n";
    os << "   - Максимальная длина:     " << msgStat.getMaxLenMessage() << "\n";
    os << "   - Средняя длина:          " << msgStat.getAverageLenMessage() << "\n";
    os << "========================================\n\n";
}

int main() 
{


    return 0;
}