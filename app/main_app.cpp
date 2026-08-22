#include "ThreadManager/ThreadManager.hpp"

#include <iostream>
#include <thread>

void loggerWorker(ThreadManager& thm, Logger&& lg) 
{
    auto msg = thm.pop();
    while(true) 
    {
        if (!msg)
        {
            break;
        }

        if (std::error_code err = lg.log(*msg); err.value())
        {
            if (err.value() != static_cast<int>(LoggerError::FILTERED)) 
            {
                std::cout << "Предупреждение! Сообщение: " << LoggerMessageFormater::formatToText(*msg) << "\nБыло отфильтровано\n";
            }
            else 
            {
                thm.setError(err);
                break;
            }
        }

        msg = thm.pop();
    }
}

std::optional<MessageLevel> parseMessageLevel (std::string_view levelStr) 
{
    if (levelStr == "INFO" || levelStr == "info" || levelStr == "0") return MessageLevel::INFO;
    else if (levelStr == "WARNING" || levelStr == "warning" || levelStr == "1") return MessageLevel::WARNING;
    else if (levelStr == "ERROR" || levelStr == "error" || levelStr == "2") return MessageLevel::ERROR;
    else return std::nullopt;
}

int main(int argc, char* argv[]) 
{
    if (argc != 3)
    {
        std::cout << "Ошибка! Неверное количество параметров;\n Использование: <уровень_по_умолчанию> <путь_к_файлу>\n";
        return 1;
    }
    
    auto defaultLvl = parseMessageLevel(argv[1]);
    if (!defaultLvl) 
    {
        std::cout << "Ошибка! Неверный формат параметра <уровень_по_умолчанию>\n";
        return 1;
    }

    ThreadManager threadManager {};
    std::thread logger {loggerWorker, std::ref(threadManager), Logger{*defaultLvl, argv[2]}};

    while (true) 
    {
        if (auto err = threadManager.getError(); err) 
        {
            std::cout << "Возникла ошибка обработки сообщения!\n" << err->message() << '\n';
            break;
        }

        std::cout << "\nВведите сообщение, для выхода пустая строка: ";
        std::string msg {};
        std::getline(std::cin, msg);

        if (msg.empty()) 
        {
            threadManager.stop();
            break;
        }

        std::cout << "Введите уровень важности (INFO, WARNING, ERROR): ";

        std::string lineLvl {};
        std::getline(std::cin, lineLvl);

        if (lineLvl.empty()) 
        {
            threadManager.push(msg, *defaultLvl);
            std::cout << "Предупреждение! Использовался уровень по умолчанию\n";
        }
        else if (auto lvl = parseMessageLevel(lineLvl); lvl)
        {
            threadManager.push(msg, *lvl);
        }
        else 
        {
            std::cout << "Ошибка! Неверный формат уровня важности\n";
        }
    }

    std::cout << "\nЗавершение работы программы...\n";
    if (logger.joinable())
    {
        logger.join();
    }
    
    return threadManager.getError() ? 1 : 0;
}