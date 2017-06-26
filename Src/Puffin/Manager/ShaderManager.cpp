//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Manager/ShaderManager.h"

using namespace puffin;

ShaderManager::ShaderManager(StateMachinePtr state_machine) :
    BaseManager("core_shader_manager")
{
    if (!state_machine)
        logErrorAndThrow(name_, "ShaderManager::ShaderManager()",
            "Object [StateMachine] pointer not set.");

    state_machine_ = state_machine;

    logDebug(name_, "ShaderManager::ShaderManager()",
        "Shader manager created.");
}

ShaderManager::~ShaderManager()
{
    logDebug(name_, "ShaderManager::~ShaderManager()",
        "Shader manager destroyed.");
}

ShaderProgramPtr ShaderManager::createShaderProgram(std::string program_name,
    std::string vs_path, std::string fs_path, std::string gs_path)
{
    if (program_name.empty())
        logErrorAndThrow(name_, "ShaderManager::createShaderProgram()",
            "Empty shader program name.");

    if (vs_path.empty() || fs_path.empty())
        logErrorAndThrow(name_, "ShaderManager::createShaderProgram()",
            "Empty shader file path.");

    ShaderProgramPtr shader_program(new ShaderProgram(gs_path.empty() ? false :
        true, program_name));

    std::vector<GLint> shaders = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
    if (shader_program->handle_gs_)
        shaders.push_back(GL_GEOMETRY_SHADER);

    for (const auto &type : shaders)
    {
        GLuint *handle_ptr = nullptr;
        std::string *file_ptr = nullptr;
        if (type == GL_VERTEX_SHADER)
        {
            handle_ptr = &shader_program->handle_vs_;
            file_ptr = &vs_path;
        }
        else if (type == GL_FRAGMENT_SHADER)
        {
            handle_ptr = &shader_program->handle_fs_;
            file_ptr = &fs_path;
        }
        else if (type == GL_GEOMETRY_SHADER)
        {
            handle_ptr = &shader_program->handle_gs_;
            file_ptr = &gs_path;
        }

        std::string shader_data;
        if (loadShaderCode(*file_ptr, shader_data))
            logErrorAndThrow(name_, "ShaderManager::createShaderProgram()",
                "Opening shader file [" + *file_ptr + "] error.");

        logInfo(name_, "ShaderManager::createShaderProgram()",
            "Shader file [" + *file_ptr + "] loaded.");

        const GLchar *shader_code = shader_data.c_str();
        glShaderSource(*handle_ptr, 1, &shader_code, nullptr);

        GLint result = compileShader(*handle_ptr);
        logInfo(name_, "ShaderManager::createShaderProgram()", "Shader file [" +
            *file_ptr + "] compile message:\n" +
            getShaderCompileMessage(*handle_ptr));

        if (result)
            logError(name_, "ShaderManager::createShaderProgram()",
                "Shader file [" + *file_ptr + "] compile error.");

        logInfo(name_, "ShaderManager::createShaderProgram()",
            "Shader file [" + *file_ptr + "] compile success.");
    }

    if (linkProgram(shader_program))
        logErrorAndThrow(name_, "ShaderManager::createShaderProgram()",
            "Shader program [" + program_name + "] link error.");

    logInfo(name_, "ShaderManager::createShaderProgram()",
        "Shader program [" + program_name + "] link success.");

    shader_program->fetchUniforms();

    shader_program_container_.push_back(shader_program);
    return shader_program;
}

GLint ShaderManager::compileShader(GLuint shader_handle) const
{
    glCompileShader(shader_handle);
    if (checkShaderCompileStatus(shader_handle))
        return -1;

    return 0;
}

GLint ShaderManager::checkShaderCompileStatus(GLuint shader_handle) const
{
    GLint shader_status = -1;
    glGetShaderiv(shader_handle, GL_COMPILE_STATUS, &shader_status);
    if (shader_status != GL_TRUE)
        return -1;

    return 0;
}

std::string ShaderManager::getShaderCompileMessage(GLuint shader_handle) const
{
    GLint log_size = 256;
    glGetShaderiv(shader_handle, GL_INFO_LOG_LENGTH, &log_size);

    std::string compile_msg;

    // There are some errors in shader, so log is not empty
    if (log_size > 1)
    {
        GLchar *log_text = new GLchar[log_size];
        glGetShaderInfoLog(shader_handle, log_size, nullptr, log_text);
        compile_msg = std::string(log_text);

        delete log_text;
    }
    else
        compile_msg = "[NO WARNINGS AND ERRORS]";

    return compile_msg;
}

GLint ShaderManager::loadShaderCode(std::string file_path,
    std::string &shader_code) const
{
    std::ifstream shader_file(file_path, std::ios::in);
    if (shader_file.is_open())
    {
        std::string line;
        while (std::getline(shader_file, line))
            shader_code += line + "\n";

        shader_file.close();
    }
    else
        return -1;

    return 0;
}

GLint ShaderManager::linkProgram(ShaderProgramPtr shader_program) const
{
    glAttachShader(shader_program->handle_, shader_program->handle_vs_);
    glAttachShader(shader_program->handle_, shader_program->handle_fs_);

    if (shader_program->handle_gs_)
        glAttachShader(shader_program->handle_, shader_program->handle_gs_);

    glLinkProgram(shader_program->handle_);

    if (checkProgramLinkStatus(shader_program))
        return -1;

    return 0;
}

GLint ShaderManager::checkProgramLinkStatus(ShaderProgramPtr
    shader_program) const
{
    GLint link_status = -1;
    glGetProgramiv(shader_program->handle_, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE)
        return -1;

    return 0;
}