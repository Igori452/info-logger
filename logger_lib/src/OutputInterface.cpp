#include "OutputInterface.hpp"

OFileInterface::OFileInterface(std::string_view path_) 
    : path(std::filesystem::path(path_)), ofile(path, std::ios::app) {}

std::error_code OFileInterface::write(std::string_view message) 
{
    if (!ofile.is_open()) 
    {
        return make_error_code(LoggerError::FILE_NOT_OPEN);
    }

    ofile << message;

    if (!ofile) 
    {
        return make_error_code(LoggerError::WRITE_FAILED);
    }

    return make_error_code(LoggerError::SUCCESS);
}