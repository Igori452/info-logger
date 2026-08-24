#include "LoggerLibTests.hpp"

#include <cassert>
#include <iostream>

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