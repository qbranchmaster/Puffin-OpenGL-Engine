//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_FPS_COUNTER_H
#define PUFFIN_FPS_COUNTER_H

#include <GL/glew.h>

#include <chrono>
#include <memory>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class FpsCounter
    {
        friend class MasterRenderer;

    public:
        FpsCounter()
        {
            logDebug(name_, "FpsCounter::FpsCounter()", "Fps counter created.");
        }

        virtual ~FpsCounter()
        {
            logDebug(name_, "FpsCounter::~FpsCounter()",
                "Fps counter destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

        GLint getFpsRate() const
        {
            return frame_rate_;
        }

        GLdouble getDelta() const
        {
            return delta_;
        }

    protected:
        void update()
        {
            static auto prev_time = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::now();

            frames_++;

            if (std::chrono::duration_cast<std::chrono::milliseconds>
                (time - prev_time).count() >= 1000)
            {
                prev_time = time;
                frame_rate_ = frames_;
                frames_ = 0;
            }
        }

        void startDeltaMeasure()
        {
            delta_t0_ = std::chrono::system_clock::now();
        }

        void endDeltaMeasure()
        {
            delta_t1_ = std::chrono::system_clock::now();
            delta_ = static_cast<GLdouble>(std::chrono::duration_cast
                <std::chrono::microseconds>(delta_t1_ - delta_t0_).count()) /
                1000000.0;
        }

        std::string name_{"core_fps_counter"};

        GLint frames_{0};
        GLint frame_rate_{99};

        std::chrono::time_point<std::chrono::system_clock> delta_t0_;
        std::chrono::time_point<std::chrono::system_clock> delta_t1_;
        GLdouble delta_{0.0};
    };

    using FpsCounterPtr = std::shared_ptr<FpsCounter>;
} // namespace puffin

#endif // PUFFIN_FPS_COUNTER_H