#include "ErrorCodes.hpp"

const char* LoggerCategory::name() const noexcept
{
    return "LoggerCategory";
}

std::string LoggerCategory::message(int _Errval) const 
{
    switch ( static_cast<LoggerError>(_Errval))
    {
        case LoggerError::SUCCESS: return "Successful operation";
        case LoggerError::FILTERED: return "The message is unsigned because its level is lower than the default level";
        case LoggerError::FILE_NOT_OPEN: return "Cannot open the file at the specified path";
        case LoggerError::WRITE_FAILED: return "Error writing message";
        default: return "unknown error";   
    }
}