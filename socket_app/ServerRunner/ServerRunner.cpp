#include "ServerRunner.hpp"

void ServerRunner::printStatistic(const MessageStatistics& msgStat, std::ostream& os) 
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

void ServerRunner::run(int sock, size_t T, size_t N, std::ostream& os) 
{

    SocketReceiver socketReceiver {sock};
    MessageStatistics msgStat {};
    size_t messageCnt {0};

    while (true) 
    {
        std::error_code err {socketReceiver.waitData(T)};

        if (err == std::errc::timed_out)
        {
            if (msgStat.hasChanged()) 
            {
                os << "\nВывод статистики после таймаута T:";
                printStatistic(msgStat, os);
            }
            continue;
        } 
        else if (err)
        {
            break;
        }

        if (socketReceiver.readFromNetwork()) 
        {
            while (auto msg = socketReceiver.getNextMessage('\n')) 
            {
                if (auto formatMsg = LoggerMessageFormater::formatToLoggerMessage(*msg); formatMsg)
                {
                    os << "Получено сообщение:\n" << *msg;
                    msgStat.setData(*formatMsg);

                    if (++messageCnt >= N) 
                    {
                        os << "Вывод статистики после каждого " << N << "-го сообщения:";
                        printStatistic(msgStat, os);  
                        messageCnt = 0;
                    }
                }
                else 
                {
                    os << "Ошибка! Сообщение проигнорировано, так как не соответствует формату лога!\n";
                }
            }
        }
        else 
        {
            break;
        }
    }

    os << "Работа с клиентом завершена. Итоговая статистика:\n";
    printStatistic(msgStat, os);
}