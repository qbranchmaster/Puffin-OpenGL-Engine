//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_SHADER_PROGRAM_H
#define PUFFIN_SHADER_PROGRAM_H

#include <GL/glew.h>

#include <memory>
#include <unordered_map>

#include "Puffin/Common/Logger.h"

namespace puffin
{
    class ShaderProgram
    {
        friend class ShaderManager;
        friend class StateMachine;

    public:
        explicit ShaderProgram(GLboolean geometry_shader = false,
            std::string name = "");
        virtual ~ShaderProgram();

        std::string getName() const
        {
            return name_;
        }

    protected:
        void fetchUniforms();

        GLint getUniformLocation(std::string uniform_name) const
        {
            auto location = uniforms_.find(uniform_name);
            if (location == uniforms_.end())
                return -1;

            return location->second;
        }

        std::string name_{"unnamed_shader_program"};

        GLuint handle_{0};
        GLuint handle_vs_{0};
        GLuint handle_fs_{0};
        GLuint handle_gs_{0};

        std::unordered_map<std::string, GLint> uniforms_;
    };

    using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
} // namespace puffin

#endif // PUFFIN_SHADER_PROGRAM_H