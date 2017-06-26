//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Shader/ShaderProgram.h"

using namespace puffin;

ShaderProgram::ShaderProgram(GLboolean geometry_shader, std::string name)
{
    if (!name.empty())
        name_ = name;

    handle_vs_ = glCreateShader(GL_VERTEX_SHADER);
    handle_fs_ = glCreateShader(GL_FRAGMENT_SHADER);
    if (geometry_shader)
        handle_gs_ = glCreateShader(GL_GEOMETRY_SHADER);

    if (!handle_vs_ || !handle_fs_ || (!handle_gs_ && geometry_shader))
        logErrorAndThrow(name_, "ShaderProgram::ShaderProgram()",
            "Creating shader error.");

    handle_ = glCreateProgram();
    if (!handle_)
        logErrorAndThrow(name_, "ShaderProgram::ShaderProgram()",
            "Creating shader program error.");

    logDebug(name_, "ShaderProgram::ShaderProgram()",
        "Shader program created.");
}

ShaderProgram::~ShaderProgram()
{
    if (handle_vs_)
    {
        glDetachShader(handle_, handle_vs_);
        glDeleteShader(handle_vs_);
    }

    if (handle_fs_)
    {
        glDetachShader(handle_, handle_fs_);
        glDeleteShader(handle_fs_);
    }

    if (handle_gs_)
    {
        glDetachShader(handle_, handle_gs_);
        glDeleteShader(handle_gs_);
    }

    if (handle_)
        glDeleteProgram(handle_);

    logDebug(name_, "ShaderProgram::~ShaderProgram()",
        "Shader program destroyed.");
}

void ShaderProgram::fetchUniforms()
{
    GLint uniforms_count = 0;

    std::vector<GLenum> properties;
    properties.push_back(GL_NAME_LENGTH);
    properties.push_back(GL_TYPE);
    properties.push_back(GL_ARRAY_SIZE);

    std::vector<GLint> values(properties.size());

    glGetProgramInterfaceiv(handle_, GL_UNIFORM, GL_ACTIVE_RESOURCES,
        &uniforms_count);

    std::vector<GLchar> name_buffer;

    for (GLint i = 0; i < uniforms_count; i++)
    {
        glGetProgramResourceiv(handle_, GL_UNIFORM, i, properties.size(),
            &properties[0], values.size(), NULL, &values[0]);

        name_buffer.clear();
        name_buffer.resize(values[0]);

        glGetProgramResourceName(handle_, GL_UNIFORM, i, name_buffer.size(),
            nullptr, &name_buffer[0]);
        std::string uniform_name((GLchar*) &name_buffer[0],
            name_buffer.size() - 1);

        GLint location = glGetUniformLocation(handle_, uniform_name.c_str());
        uniforms_[uniform_name] = location;
    }
}