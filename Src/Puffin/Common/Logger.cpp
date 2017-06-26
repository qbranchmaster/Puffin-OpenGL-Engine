//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Common/Logger.h"

using namespace puffin;

void Logger::enable(std::string file_name, GLboolean console_enabled,
    GLboolean append)
{
    disable();

    if (file_name.empty())
        file_name = "puffin_engine.log";

    if (append)
        log_file_.open(file_name.c_str(), std::ios::out | std::ios::app);
    else
        log_file_.open(file_name.c_str(), std::ios::out | std::ios::trunc);

    if (!log_file_.is_open())
        throw Exception(name_, "Logger::enable()", "Opening file [" +
            file_name + "] error. Check file write or read permission.");

    if (!append)
        log_file_ << getHeader();

    console_enabled_ = console_enabled;
    enabled_ = true;
}

void Logger::disable()
{
    if (enabled_)
    {
        log_file_.close();
        console_enabled_ = false;
        enabled_ = false;
    }
}

void Logger::enableTimeStamp(GLboolean state)
{
    add_timestamp_ = state;
}

void Logger::enableDebug(GLboolean state)
{
    debug_enabled_ = state;
}

void Logger::enableNames(GLboolean state)
{
    add_names_ = state;
}

std::string Logger::getHeader() const
{
    constexpr std::size_t header_size = 80;
    std::string separator(header_size, '-');
    std::string header_text = "Puffin Engine Log";
    std::string header(header_size, ' ');
    header.replace(header_size / 2 - header_text.size() / 2,
        header_text.size(), header_text);

    return std::string(separator + "\n" + header + "\n" + separator + "\n");
}

std::string Logger::getTimeStampNow() const
{
    time_t actual_time = 0;
    std::time(&actual_time);

    tm time_info = {};
#ifdef UNIX
    localtime_r(&actual_time, &time_info);
#else
    localtime_s(&time_info, &actual_time);
#endif // UNIX
    constexpr GLshort buffer_size = 64;
    GLchar time_buffer[buffer_size];
    std::strftime(time_buffer, buffer_size, "%H:%M:%S", &time_info);

    return std::string(time_buffer);
}

void Logger::log(std::string object_name, std::string function_name,
    std::string message, MessageType type)
{
    if (!enabled_)
        return;

    if (type == MessageType::DEBUG && !debug_enabled_)
        return;

    if (object_name.empty())
        object_name = "unknown_object";

    if (function_name.empty())
        function_name = "unknown_function";

    if (message.empty())
        message = "no_message";

    std::string time;
    if (add_timestamp_)
    {
        time = std::string(8, ' ');
        std::string stamp_str = getTimeStampNow();
        time.replace(0, stamp_str.size(), stamp_str);
        time += " | ";
    }

    std::string msg_type(10, ' ');
    switch (type)
    {
    case MessageType::INFO:
        msg_type.replace(0, 6, "[INFO]");
        break;
    case MessageType::WARNING:
        msg_type.replace(0, 9, "[WARNING]");
        break;
    case MessageType::ERROR:
        msg_type.replace(0, 7, "[ERROR]");
        break;
    case MessageType::DEBUG:
        msg_type.replace(0, 7, "[DEBUG]");
    }

    std::size_t new_line_pos = 0;
    do
    {
        new_line_pos = message.find('\n');
        std::string str = message.substr(0, new_line_pos);

        std::string complete_message = time + msg_type + " | " + (add_names_ ?
            "[" + object_name + "] " : "") + function_name + ": " + str + "\n";
        log_file_ << complete_message << std::flush;

        if (console_enabled_)
            std::cout << complete_message << std::flush;

        message = message.substr(new_line_pos + 1);
    } while (new_line_pos != std::string::npos);
}

void Logger::logAndThrow(std::string object_name, std::string function_name,
    std::string message, MessageType type)
{
    log(object_name, function_name, message, type);
    throw Exception(object_name, function_name, message);
}

void puffin::logInfo(std::string object_name, std::string function_name,
    std::string message)
{
    Logger::instance().log(object_name, function_name, message,
        MessageType::INFO);
}

void puffin::logWarning(std::string object_name, std::string function_name,
    std::string message)
{
    Logger::instance().log(object_name, function_name, message,
        MessageType::WARNING);
}

void puffin::logError(std::string object_name, std::string function_name,
    std::string message)
{
    Logger::instance().log(object_name, function_name, message,
        MessageType::ERROR);
}

void puffin::logDebug(std::string object_name, std::string function_name,
    std::string message)
{
    Logger::instance().log(object_name, function_name, message,
        MessageType::DEBUG);
}

void puffin::logErrorAndThrow(std::string object_name,
    std::string function_name, std::string message)
{
    Logger::instance().logAndThrow(object_name, function_name, message,
        MessageType::ERROR);
}