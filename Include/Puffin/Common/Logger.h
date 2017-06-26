//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_LOGGER_H
#define PUFFIN_LOGGER_H

#include <GL/glew.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "Puffin/Common/Exception.h"

namespace puffin
{
    enum class MessageType
    {
        INFO,
        WARNING,
        ERROR,
        DEBUG,
    };

    class Logger
    {
    public:
        static Logger& instance()
        {
            static Logger singleton;
            return singleton;
        }

        std::string getName() const
        {
            return name_;
        }

        void enable(std::string file_name, GLboolean console_enabled = false,
            GLboolean append = false);
        void disable();

        GLboolean isEnabled() const
        {
            return enabled_;
        }

        void enableTimeStamp(GLboolean state);
        void enableDebug(GLboolean state);
        void enableNames(GLboolean state);

        void log(std::string object_name, std::string function_name,
            std::string message, MessageType type);
        void logAndThrow(std::string object_name, std::string function_name,
            std::string message, MessageType type);

    protected:
        Logger() {}

        ~Logger()
        {
            disable();
        }

        Logger(const Logger &) = delete;
        void operator=(const Logger &) = delete;

        std::string getHeader() const;
        std::string getTimeStampNow() const;

        std::string name_{"core_logger"};

        GLboolean enabled_{false};
        GLboolean console_enabled_{false};
        GLboolean debug_enabled_{false};
        GLboolean add_timestamp_{false};
        GLboolean add_names_{false};

        std::fstream log_file_;
    };

    void logInfo(std::string object_name, std::string function_name,
        std::string message);
    void logWarning(std::string object_name, std::string function_name,
        std::string message);
    void logError(std::string object_name, std::string function_name,
        std::string message);
    void logDebug(std::string object_name, std::string function_name,
        std::string message);
    void logErrorAndThrow(std::string object_name, std::string function_name,
        std::string message);
} // namespace puffin

#endif // PUFFIN_LOGGER_H