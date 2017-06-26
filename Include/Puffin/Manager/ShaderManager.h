//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#ifndef PUFFIN_SHADER_MANAGER_H
#define PUFFIN_SHADER_MANAGER_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <vector>

#include "Puffin/Configuration/StateMachine.h"
#include "Puffin/Manager/BaseManager.h"
#include "Puffin/Shader/ShaderProgram.h"

namespace puffin
{
    class ShaderManager : public BaseManager
    {
    public:
        explicit ShaderManager(StateMachinePtr state_machine);
        virtual ~ShaderManager();

        ShaderProgramPtr createShaderProgram(std::string program_name,
            std::string vs_path, std::string fs_path, std::string gs_path = "");

        void setUniform(ShaderProgramPtr shader_program,
            std::string uniform_name, const glm::mat4 &value) const
        {
            state_machine_->activateShaderProgram(shader_program);

            auto location = shader_program->getUniformLocation(uniform_name);
            if (location == -1)
            {
                logWarning(name_, "ShaderManager::setUniform()",
                    "Uniform [" + uniform_name + "] does not exist in shader "
                    "program [" + shader_program->getName() + "].");
                return;
            }

            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }

        void setUniform(ShaderProgramPtr shader_program,
            std::string uniform_name, const glm::vec3 &value) const
        {
            state_machine_->activateShaderProgram(shader_program);

            auto location = shader_program->getUniformLocation(uniform_name);
            if (location == -1)
            {
                logWarning(name_, "ShaderManager::setUniform()",
                    "Uniform [" + uniform_name + "] does not exist in shader "
                    "program [" + shader_program->getName() + "].");
                return;
            }

            glUniform3fv(location, 1, glm::value_ptr(value));
        }

        void setUniform(ShaderProgramPtr shader_program,
            std::string uniform_name, const glm::vec4 &value) const
        {
            state_machine_->activateShaderProgram(shader_program);

            auto location = shader_program->getUniformLocation(uniform_name);
            if (location == -1)
            {
                logWarning(name_, "ShaderManager::setUniform()",
                    "Uniform [" + uniform_name + "] does not exist in shader "
                    "program [" + shader_program->getName() + "].");
                return;
            }

            glUniform4fv(location, 1, glm::value_ptr(value));
        }

        void setUniform(ShaderProgramPtr shader_program,
            std::string uniform_name, GLint value) const
        {
            state_machine_->activateShaderProgram(shader_program);

            auto location = shader_program->getUniformLocation(uniform_name);
            if (location == -1)
            {
                logWarning(name_, "ShaderManager::setUniform()",
                    "Uniform [" + uniform_name + "] does not exist in shader "
                    "program [" + shader_program->getName() + "].");
                return;
            }

            glUniform1iv(location, 1, &value);
        }

        void setUniform(ShaderProgramPtr shader_program,
            std::string uniform_name, GLfloat value) const
        {
            state_machine_->activateShaderProgram(shader_program);

            auto location = shader_program->getUniformLocation(uniform_name);
            if (location == -1)
            {
                logWarning(name_, "ShaderManager::setUniform()",
                    "Uniform [" + uniform_name + "] does not exist in shader "
                    "program [" + shader_program->getName() + "].");
                return;
            }

            glUniform1fv(location, 1, &value);
        }

    protected:
        GLint loadShaderCode(std::string file_path,
            std::string &shader_code) const;

        GLint compileShader(GLuint shader_handle) const;
        GLint checkShaderCompileStatus(GLuint shader_handle) const;
        std::string getShaderCompileMessage(GLuint shader_handle) const;

        GLint linkProgram(ShaderProgramPtr shader_program) const;
        GLint checkProgramLinkStatus(ShaderProgramPtr shader_program) const;

        StateMachinePtr state_machine_{nullptr};

        std::vector<ShaderProgramPtr> shader_program_container_;
    };

    using ShaderManagerPtr = std::shared_ptr<ShaderManager>;
} // namespace puffin

#endif // PUFFIN_SHADER_MANAGER_H