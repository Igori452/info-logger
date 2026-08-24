#include <system_error>

/**
 * Коды ошибок системы логирования.
 * 
 * Разделены на некритические (FILTERED) и критические (FILE_NOT_OPEN, WRITE_FAILED).
 */
enum class LoggerError 
{
    SUCCESS = 0,            // Операция выполнена успешно
    FILTERED = 1,           // Сообщение отфильтровано (некритично)

    FILE_NOT_OPEN = 2,      // Не удалось открыть файл (критично)
    WRITE_FAILED = 3,       // Ошибка записи в файл (критично)
};

/**
 * Категория ошибок для интеграции с std::error_code.
 */
class LoggerCategory : public std::error_category 
{
    public:
        const char* name() const noexcept override;
        std::string message(int _Errval) const;
};

/**
 * Фабрика для создания std::error_code из LoggerError.
 */
inline std::error_code make_error_code(LoggerError e) 
{
    static LoggerCategory instance {};
    return std::error_code(static_cast<int>(e), instance);
}