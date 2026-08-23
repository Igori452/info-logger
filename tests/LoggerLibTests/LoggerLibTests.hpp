#pragma once

#include "logger_lib/Logger.hpp"

#include <vector>
#include <sstream>

class LoggerLibTests 
{
    public:
        static void test_logger_with_mock();
        static void test_logger_with_file();
};

namespace TestContext 
{

class MockInterface : public OutputInterface 
{
    public:
        std::vector<std::string> messages;

        std::error_code write(std::string_view message) override;
};

}