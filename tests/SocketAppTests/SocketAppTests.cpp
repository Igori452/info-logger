#include "SocketAppTests.hpp"

#include "MessageStatistics/MessageStatistics.hpp"
#include "logger_lib/Logger.hpp"
#include "SocketReceiver/SocketReceiver.hpp"

#include <iostream>
#include <cassert>
#include <cmath>
#include <thread>

#include <sys/socket.h>
#include <sys/unistd.h>

void SocketAppTests::message_statistics_test()
{
    std::cout << "Start message_statistics_test...\n";

    MessageStatistics msgStat {};

    size_t cnt {1};
    size_t cntInfo {0}, cntWarning {0}, cntError {0};

    size_t maxLen {0}, minLen {std::numeric_limits<size_t>::max()};
    size_t totalLen {0};

    for (; cnt <= 100; ++cnt) 
    {
        size_t sizeMsg {0};

        switch (cnt % 3)
        {
            case 0:
            {
                LoggerMessage msg {"Message #" + cnt, MessageLevel::INFO};
                msgStat.setData(msg);
                sizeMsg = LoggerMessageFormater::formatToText(msg).size();   
                ++cntInfo;
                break;
            }
            case 1:
            {
                LoggerMessage msg {"Message #" + cnt, MessageLevel::WARNING};
                msgStat.setData(msg);
                sizeMsg = LoggerMessageFormater::formatToText(msg).size(); 
                ++cntWarning;
                break;
            }
            case 2:
            {
                LoggerMessage msg {"Message #" + cnt, MessageLevel::ERROR};
                msgStat.setData(msg);
                sizeMsg = LoggerMessageFormater::formatToText(msg).size(); 
                ++cntError;
                break;
            }
        }


        maxLen = std::max(sizeMsg, maxLen);
        minLen = std::min(sizeMsg, minLen);

        totalLen += sizeMsg;
    }

    --cnt;

    assert(msgStat.getTotalMessages() == cnt);
    const auto msgLevel {msgStat.getTotalLevelMessage()};
    assert(msgLevel.at(MessageLevel::INFO) == cntInfo);
    assert(msgLevel.at(MessageLevel::WARNING) == cntWarning);
    assert(msgLevel.at(MessageLevel::ERROR) == cntError);

    assert(msgStat.getMaxLenMessage() == maxLen);
    assert(msgStat.getMinLenMessage() == minLen);
    assert(std::fabs(msgStat.getAverageLenMessage() - (totalLen * 1.0 / cnt)) < 0.00001);

    assert(msgStat.hasChanged());
    assert(!msgStat.hasChanged());

    const auto twoHourAgo {std::chrono::system_clock::now() - std::chrono::hours(2)};
    msgStat.setData(LoggerMessage {"Message an hour ago", MessageLevel::INFO, twoHourAgo});

    assert(msgStat.hasChanged());
    assert(msgStat.getTotalMessagesAnHour() == cnt);

    std::cout << "Test SUCCESS!\n\n";
}

void SocketAppTests::server_receiver_test() 
{
    std::cout << "Start server_receiver_test...\n";

    int socketPair[2] {};
    if(::socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair) < 0) 
    {
        assert(false);
    }

    int clientSocket {socketPair[0]};
    int serverSocket {socketPair[1]};

    SocketReceiver socketReceiver {serverSocket};

    LoggerMessage msg1 {"Info message", MessageLevel::INFO};
    LoggerMessage msg2 {"Warning message", MessageLevel::WARNING};

    std::string msgStr1 {LoggerMessageFormater::formatToText(msg1)};
    std::string msgStr2 {LoggerMessageFormater::formatToText(msg2)};

    const size_t separator {10};
    std::string part1 {msgStr1 + msgStr2.substr(0, separator)};
    std::string part2 {msgStr2.substr(separator)};

    std::thread senderThread([clientSocket, &part1, &part2](){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ssize_t bytesSend {::write(clientSocket, part1.data(), part1.size())};    
        assert(bytesSend == static_cast<ssize_t>(part1.size()));

        std::this_thread::sleep_for(std::chrono::seconds(1));

        bytesSend = ::write(clientSocket, part2.data(), part2.size());
        assert(bytesSend == static_cast<ssize_t>(part2.size()));    
    });

    assert(!socketReceiver.waitData(2));
    assert(socketReceiver.readFromNetwork());

    auto msgStrReceived1 {socketReceiver.getNextMessage('\n')};
    assert(msgStrReceived1);
    assert(*msgStrReceived1 == msgStr1);
    assert(!socketReceiver.getNextMessage('\n'));  
    
    assert(!socketReceiver.waitData(2));
    assert(socketReceiver.readFromNetwork());

    auto msgStrReceived2 {socketReceiver.getNextMessage('\n')};
    assert(msgStrReceived2);
    assert(*msgStrReceived2 == msgStr2);

    if (senderThread.joinable()) 
    {
        senderThread.join();
    }

    assert(socketReceiver.waitData(0) == std::errc::timed_out);

    ::close(clientSocket);
    assert(!socketReceiver.readFromNetwork());

    ::close(serverSocket);

    std::cout << "Test SUCCESS!\n\n";
}

void SocketAppTests::server_runner_test() 
{
    std::cout << "Start server_runner_test...\n";

    int socketPair[2] {};

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, socketPair) < 0)
    {
        assert(false);
    }

    int clientSocket {socketPair[0]};
    int serverSocket {socketPair[1]};

    SocketReceiver socketReceiver {serverSocket}; 
    Logger logger {MessageLevel::INFO, std::make_unique<OSocketInterface>(clientSocket)};

    std::vector<LoggerMessage> lgmsgs 
    {
        LoggerMessage {"Info message", MessageLevel::INFO},
        LoggerMessage {"Warning message", MessageLevel::WARNING},
        LoggerMessage {"Error message", MessageLevel::ERROR},
    };
    std::vector<std::string> msgs {};
    
    for (const auto& lgmsg : lgmsgs) 
    {
        msgs.emplace_back(LoggerMessageFormater::formatToText(lgmsg));
        logger.log(lgmsg);
    }

    assert(socketReceiver.readFromNetwork());
    for (const auto& msg : msgs) 
    {
        auto res {socketReceiver.getNextMessage('\n')};
        assert(res);
        assert(*res == msg);
    }

    ::close(clientSocket);
    ::close(serverSocket);

    std::cout << "Test SUCCESS!\n\n";
}