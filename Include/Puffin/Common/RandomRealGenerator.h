//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_RANDOM_REAL_GENERATOR_H
#define PUFFIN_RANDOM_REAL_GENERATOR_H

#include <GL/glew.h>

#include <memory>
#include <random>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class RandomRealGenerator
    {
    public:
        RandomRealGenerator(GLfloat min, GLfloat max, std::string name = "") :
            distribution_(min, max)
        {
            if (!name.empty())
                name_ = name;

            min_value_ = min;
            max_value_ = max;

            logDebug(name_, "RandomRealGenerator::RandomRealGenerator()",
                "Random real generator created.");
        }

        virtual ~RandomRealGenerator()
        {
            logDebug(name_, "RandomRealGenerator::~RandomRealGenerator()",
                "Random real generator destroyed.");
        }

        std::string getName() const
        {
            return name_;
        }

        GLfloat getMinValue() const
        {
            return min_value_;
        }

        GLfloat getMaxValue() const
        {
            return max_value_;
        }

        GLfloat getRandom()
        {
            return distribution_(random_engine_);
        }

    protected:
        std::string name_{"unnamed_random_real_generator"};

        std::default_random_engine random_engine_{};
        std::uniform_real_distribution<GLfloat> distribution_{};

        GLfloat min_value_{0.0f};
        GLfloat max_value_{0.0f};
    };

    using RandomRealGeneratorPtr = std::shared_ptr<RandomRealGenerator>;
} // namespace puffin

#endif // PUFFIN_RANDOM_REAL_GENERATOR_H