#pragma once

#include <system_error>
#include <optional>

class SocketReceiver 
{
    private:
        int socketObject;
        std::string streamBuffer;

    public:
        SocketReceiver() = delete;
        SocketReceiver(int socketObject_);

        /**
         * Ожидание появления данных в сокете с ограничением по времени.
         * 
         * std::error_code{} (успех), если данные готовы для чтения.
         * Ошибка errno (generic_category) при системном сбое poll.
         * std::errc::timed_out, если истекло время ожидания timeoutSeconds.
         * std::errc::broken_pipe при ошибках сокета (POLLERR, POLLHUP, POLLNVAL).
         */
        std::error_code waitData(size_t imeoutSeconds) const;

        /**
         * Чтение доступных данных из сетевого сокета во внутренний буфер.
         * 
         * true, если данные успешно прочитаны и добавлены в streamBuffer.
         * false при ошибке сети или если клиент штатно отключился.
         */

        bool readFromNetwork();

        /**
         * Извлечение следующего полного сообщения из накопленного буфера.
         * 
         * Извлеченная строка лога (включая разделитель), 
         * или std::nullopt, если символ разделителя (separator) еще не найден в буфере.
         */
        std::optional<std::string> getNextMessage(const char separator);
};