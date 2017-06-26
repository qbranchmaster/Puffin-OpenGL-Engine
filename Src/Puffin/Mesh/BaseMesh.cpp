//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Mesh/BaseMesh.h"

using namespace puffin;

BaseMesh::BaseMesh(std::string name)
{
    if (!name.empty())
        name_ = name;

    glGenVertexArrays(1, &handle_);

    logDebug(name_, "BaseMesh::BaseMesh()", "Base mesh created.");
}

BaseMesh::~BaseMesh()
{
    for (auto &buffer : data_buffers_)
    {
        if (buffer.second != 0)
            glDeleteBuffers(1, &buffer.second);
    }

    if (handle_)
        glDeleteVertexArrays(1, &handle_);

    logDebug(name_, "BaseMesh::~BaseMesh()", "Base mesh destroyed.");
}