//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Camera/Camera.h"

using namespace puffin;

Camera::Camera(std::string name)
{
    if (!name.empty())
        name_ = name;

    camera_box_.reset(new CameraBox(name_ + "_view_box"));

    calculateRotationMatrix();
    calculateViewVectors();
    calculateViewMatrix();

    calculateProjectionMatrix();

    logDebug(name_, "Camera::Camera()", "Camera created.");
}

Camera::~Camera()
{
    logDebug(name_, "Camera::~Camera()", "Camera destroyed.");
}

void Camera::setProjection(GLfloat fov, GLfloat aspect, GLfloat near_plane,
    GLfloat far_plane)
{
    if (fov <= 0.0f)
        logErrorAndThrow(name_, "Camera::setProjection()",
            "Camera FOV value out of range: {0.0 < VALUE}.");

    if (aspect <= 0.0f)
        logErrorAndThrow(name_, "Camera::setProjection()",
            "Camera aspect value out of range: {0.0 < VALUE}.");

    if (near_plane <= 0.0f || far_plane <= 0.0f)
        logErrorAndThrow(name_, "Camera::setProjection()",
            "Camera clip plane value out of range: {0.0 < VALUE}.");

    fov_ = fov;
    aspect_ = aspect;
    near_plane_ = near_plane;
    far_plane_ = far_plane;

    calculateProjectionMatrix();
}