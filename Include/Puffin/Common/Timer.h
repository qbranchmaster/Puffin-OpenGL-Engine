//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_TIMER_H
#define PUFFIN_TIMER_H

#include <GL/glew.h>

#include <chrono>
#include <functional>
#include <memory>
#include <thread>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    using TimeoutCallback = std::function<void(void)>;

    class Timer
    {
    public:
        explicit Timer(const TimeoutCallback &timeout, std::string name = "")
        {
            if (!name.empty())
                name_ = name;

            timeout_func_ = timeout;

            logDebug(name_, "Timer::Timer()", "Timer created.");
        }

        virtual ~Timer()
        {
            if (running_)
                stop();

            logDebug(name_, "Timer::~Timer()", "Timer destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

        GLboolean isRunning() const
        {
            return running_;
        }

        void start(GLint interval)
        {
            if (running_)
            {
                logWarning(name_, "Timer::start()",
                    "Timer is already running.");
                return;
            }

            if (interval <= 0)
                logErrorAndThrow(name_, "Timer::start()",
                    "Interval value out of range: {0 < VALUE}.");

            interval_ = std::chrono::milliseconds(interval);

            running_ = true;
            thread_ = std::thread(&Timer::timerThread, this);
        }

        void stop()
        {
            running_ = false;
            thread_.join();
        }

    protected:
        void timerThread()
        {
            while (running_)
            {
                std::this_thread::sleep_for(interval_);
                timeout_func_();
            }
        }

        std::string name_{"unnamed_timer"};

        GLboolean running_{false};
        std::chrono::milliseconds interval_{0};
        TimeoutCallback timeout_func_{nullptr};

        std::thread thread_;
    };

    using TimerPtr = std::shared_ptr<Timer>;
} // namespace puffin

#endif // PUFFIN_TIMER_H