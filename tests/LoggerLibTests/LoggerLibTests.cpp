#include "LoggerLibTests.hpp"

#include <cassert>
#include <iostream>
#include <filesystem>

std::error_code TestContext::MockInterface::write(std::string_view message) 
{
    messages.emplace_back(message);
    return make_error_code(LoggerError::SUCCESS);
}

void LoggerLibTests::test_logger_with_mock() 
{
    std::cout << "Start test_logger_with_mock...\n";

    auto mock = std::make_unique<TestContext::MockInterface>();
    auto* mockPtr = mock.get();

    Logger lg {MessageLevel::WARNING, std::move(mock)};
    assert(lg.getDefaultMessageLevel() == MessageLevel::WARNING);

    // Проверка корректной записи данных в интерфес вывода (MockInterface)
    LoggerMessage lgmsgInfo {"Info message", MessageLevel::INFO};
    std::error_code err {lg.log(lgmsgInfo)};
    assert(err.value() == static_cast<int>(LoggerError::FILTERED));
    assert(mockPtr->messages.empty());

    LoggerMessage lgmsgWarning {"Warning message", MessageLevel::WARNING};
    err = lg.log(lgmsgWarning);
    assert(err.value() == static_cast<int>(LoggerError::SUCCESS));
    assert(mockPtr->messages.back() == LoggerMessageFormater::formatToText(lgmsgWarning));

    LoggerMessage lgmsgError {"Error message", MessageLevel::ERROR};
    err = lg.log(lgmsgError);
    assert(err.value() == static_cast<int>(LoggerError::SUCCESS));
    assert(mockPtr->messages.back() == LoggerMessageFormater::formatToText(lgmsgError));

    //Пишем всё
    lg.setDefaultMessageLevel(MessageLevel::INFO);
    assert(lg.getDefaultMessageLevel() == MessageLevel::INFO);

    // Проверка создания объекта LoggerMessage из перегруженной функции log(...)
    mockPtr->messages.clear();
    err = lg.log(lgmsgInfo.getMessage(), lgmsgInfo.getMessageLevel());
    assert(err.value() == static_cast<int>(LoggerError::SUCCESS));
    assert(!mockPtr->messages.empty());

    mockPtr->messages.clear();
    err = lg.log(lgmsgWarning.getMessage(), lgmsgWarning.getMessageLevel());
    assert(err.value() == static_cast<int>(LoggerError::SUCCESS));
    assert(!mockPtr->messages.empty());

    mockPtr->messages.clear();
    err = lg.log(lgmsgError.getMessage(), lgmsgError.getMessageLevel());
    assert(err.value() == static_cast<int>(LoggerError::SUCCESS));
    assert(!mockPtr->messages.empty());

    std::cout << "Test SUCCESS!\n\n";
}

void LoggerLibTests::test_logger_with_file() 
{
    std::cout << "Start test_logger_with_file...\n";

    MessageLevel defaultMsgLevel = MessageLevel::INFO;
    LoggerMessage lgmsg {"Info message", defaultMsgLevel};

    // Проверяем поведение логгера при явно несуществующей дирректории
    {
        Logger lg{defaultMsgLevel, "test/test/test"};
        std::error_code err {lg.log(lgmsg)};
        assert(err.value() == static_cast<int>(LoggerError::FILE_NOT_OPEN));
    }

    // Корректная запись в файл
    {
        const std::string filename = "/tmp/log.txt";
        {
            Logger lg{defaultMsgLevel, filename};
            std::error_code err {lg.log(lgmsg)};
            assert(err.value() == static_cast<int>(LoggerError::SUCCESS));
        }

        std::ifstream ifile {filename};

        std::string fileData {};
        std::getline(ifile, fileData);

        assert((fileData + "\n") == LoggerMessageFormater::formatToText(lgmsg));

        ifile.close();
        std::filesystem::remove(filename);
    }

    std::cout << "Test SUCCESS!\n\n";
}

void LoggerLibTests::formater_roundtrip_test() 
{
    std::cout << "Start formater_roundtrip_test...\n";
    LoggerMessage lgmsg {"Info test text", MessageLevel::INFO};

    auto fromFormatedLgmsg = LoggerMessageFormater::formatToLoggerMessage(LoggerMessageFormater::formatToText(lgmsg));

    assert(fromFormatedLgmsg);
    assert(lgmsg.getMessage() == fromFormatedLgmsg->getMessage());
    assert(lgmsg.getMessageLevel() == fromFormatedLgmsg->getMessageLevel());

    auto originalTime = std::chrono::time_point_cast<std::chrono::seconds>(lgmsg.getTimeCreation());
    auto formatedTime = std::chrono::time_point_cast<std::chrono::seconds>(fromFormatedLgmsg->getTimeCreation());

    assert(originalTime == formatedTime);

    auto invalid = LoggerMessageFormater::formatToLoggerMessage("test");
    assert(!invalid);

    invalid = LoggerMessageFormater::formatToLoggerMessage("[ 2026-08-21 14:54:44 ] [ INF     ] message.\n");
    assert(!invalid);

    invalid = LoggerMessageFormater::formatToLoggerMessage("[ 2026-08-21 14:54:44 ] [ INFO     ].\n");
    assert(!invalid);

    invalid = LoggerMessageFormater::formatToLoggerMessage("[ 2026-08-21 14:54:44 ] [ INFO     ] .\n");
    assert(invalid);

    std::cout << "Test SUCCESS!\n\n";
}