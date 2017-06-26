//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_EXCEPTION_H
#define PUFFIN_EXCEPTION_H

#include <string>

namespace puffin
{
    class Exception
    {
    public:
        Exception(std::string object_name, std::string function_name,
            std::string message)
        {
            if (!object_name.empty())
                object_name_ = object_name;

            if (!function_name.empty())
                function_name_ = function_name;

            if (!message.empty())
                message_ = message;
        }

        std::string getObjectName() const
        {
            return object_name_;
        }

        std::string getFunctionName() const
        {
            return function_name_;
        }

        std::string getMessage() const
        {
            return message_;
        }

        std::string what() const
        {
            return std::string("Object [" + object_name_ + "] threw exception "
                "from function [" + function_name_ + "] with message [" +
                message_ + "].");
        }

    protected:
        std::string object_name_{"unknown_object"};
        std::string function_name_{"unknown_function"};
        std::string message_{"no_message"};
    };
} // namespace puffin

#endif // PUFFIN_EXCEPTION_H