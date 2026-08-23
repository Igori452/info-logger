#include "ThreadManager/ThreadManager.hpp"
#include "AppTests.hpp"

#include <cassert>
#include <iostream>
#include <thread>

void AppTests::test_thread_manager_queue() 
{
    std::cout << "Start test_thread_manager_queue...\n";

    ThreadManager threadManager {};

    std::vector<LoggerMessage> lgmsgs 
    {
        {"Test1", MessageLevel::INFO},
        {"Test2", MessageLevel::WARNING},
    };

    for (const auto& lgmsg : lgmsgs)
    {
        threadManager.push(lgmsg);
    }
    
    std::string testText = "Test3";
    MessageLevel testLvl = MessageLevel::ERROR;
    threadManager.push(testText, testLvl);

    for (const auto& lgmsg : lgmsgs)
    {
        auto res = threadManager.pop();
        assert(res);
        assert(LoggerMessageFormater::formatToText(*res) == LoggerMessageFormater::formatToText(lgmsg));
    }

    auto res = threadManager.pop();
    assert(res);
    assert(res->getMessage() == testText);
    assert(res->getMessageLevel() == testLvl);

    std::cout << "Test SUCCESS!\n\n";
}

void AppTests::test_thread_manager_stop_operation() 
{
    std::cout << "Start test_thread_manager_stop_operation...\n";

    ThreadManager threadManager {};

    LoggerMessage lgmsg {"Test1", MessageLevel::INFO};
    threadManager.push(lgmsg);

    threadManager.stop();

    auto res = threadManager.pop();
    assert(LoggerMessageFormater::formatToText(*res) == LoggerMessageFormater::formatToText(lgmsg));

    threadManager.push(lgmsg);
    res = threadManager.pop();
    assert(!res);

    threadManager.setError(make_error_code(LoggerError::FILE_NOT_OPEN));
    assert(!threadManager.getError());

    std::cout << "Test SUCCESS!\n\n";
}

void AppTests::test_thread_manager_error_operation() 
{
    std::cout << "Start test_thread_manager_error_operation...\n";

    ThreadManager threadManager {};

    LoggerMessage lgmsg1 {"Test1", MessageLevel::INFO};
    LoggerMessage lgmsg2 {"Test2", MessageLevel::INFO};
    threadManager.push(lgmsg1);
    threadManager.push(lgmsg2);

    threadManager.setError(make_error_code(LoggerError::FILTERED));
    auto res = threadManager.pop();
    assert(res);
    assert(!threadManager.hasStop());

    threadManager.setError(make_error_code(LoggerError::FILE_NOT_OPEN));

    res = threadManager.pop();
    assert(!res);
    assert(threadManager.hasStop());

    threadManager.setError(make_error_code(LoggerError::WRITE_FAILED));
    assert(threadManager.getError() == make_error_code(LoggerError::FILE_NOT_OPEN));

    std::cout << "Test SUCCESS!\n\n";
}

void AppTests::test_thread_manager_with_two_thread() 
{
    std::cout << "Start test_thread_manager_with_two_thread...\n";

    ThreadManager threadManager {};
    std::vector<LoggerMessage> lgmsgs 
    {
        {"Test1", MessageLevel::INFO},
        {"Test2", MessageLevel::WARNING},
        {"Test3", MessageLevel::ERROR},
    };

    std::thread testWorker {[&threadManager, &lgmsgs](){

        size_t cnt = 0;
        for (const auto& lgmsg : lgmsgs)
        {
            auto res = threadManager.pop();
            assert(res);
            assert(LoggerMessageFormater::formatToText(*res) == LoggerMessageFormater::formatToText(lgmsg));
            ++cnt;
        } 

        assert(cnt == lgmsgs.size());

        std::cout << "Consumer thread successfully completed\n";

    }};

    for (const auto& lgmsg : lgmsgs)
    {
        threadManager.push(lgmsg);
    }

    threadManager.stop();

    if (testWorker.joinable())
    {
        testWorker.join();
    }

    std::cout << "Producer thread successfully completed\n";

    std::cout << "Test SUCCESS!\n\n";
}

void AppTests::test_thread_manager_with_two_thread_error_operation() 
{
    std::cout << "Start test_thread_manager_with_two_thread_error_operation...\n";

    ThreadManager threadManager {};
    std::vector<LoggerMessage> lgmsgs 
    {
        {"Test1", MessageLevel::INFO},
        {"Test2", MessageLevel::WARNING},
        {"Test3", MessageLevel::ERROR},
    };

    const std::error_code testErr = make_error_code(LoggerError::FILE_NOT_OPEN);

    std::thread testWorker {[&threadManager, &lgmsgs, &testErr](){

        const size_t errorBarier = lgmsgs.size() - 1;

        for (size_t i = 0; i < errorBarier; ++i)
        {
            auto res = threadManager.pop();
            assert(res);
            assert(LoggerMessageFormater::formatToText(*res) == LoggerMessageFormater::formatToText(lgmsgs[i]));
        } 

        threadManager.setError(testErr);

        auto res = threadManager.pop();
        assert(!res);

        std::cout << "Consumer thread successfully completed\n";

    }};

    for (const auto& lgmsg : lgmsgs)
    {
        threadManager.push(lgmsg);
    }

    if (testWorker.joinable())
    {
        testWorker.join();
    }

    assert(threadManager.getError() == testErr);

    std::cout << "Producer thread successfully completed\n";

    std::cout << "Test SUCCESS!\n\n";
}