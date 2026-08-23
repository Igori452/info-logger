#include <system_error>

enum class LoggerError 
{
    SUCCESS = 0,
    FILTERED = 1,
    FILE_NOT_OPEN = 2,
    WRITE_FAILED = 3,
};

class LoggerCategory : public std::error_category 
{
    public:
        const char* name() const noexcept override;
        std::string message(int _Errval) const;
};

inline std::error_code make_error_code(LoggerError e) 
{
    static LoggerCategory instance {};
    return std::error_code(static_cast<int>(e), instance);
}