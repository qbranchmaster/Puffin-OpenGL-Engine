//------------------------------------------------------------------------------
// Puffin OpenGL Engine
// Version: 0.3.1
// Author: Sebastian 'qbranchmaster' Tabaka
//------------------------------------------------------------------------------
#include "Puffin/Camera/CameraBox.h"

using namespace puffin;

CameraBox::CameraBox(std::string name)
{
    if (!name.empty())
        name_ = name;

    logDebug(name_, "CameraBox::CameraBox()", "Camera box created.");
}

CameraBox::~CameraBox()
{
    logDebug(name_, "CameraBox::~CameraBox()", "Camera box destroyed.");
}

void CameraBox::update(GLfloat box_size)
{
    // This function calculates camera's view bounding box.
    // Far clipping plane value can be set to camera's far clipping plane,
    // but it would reduce shadow map's resolution if camera box is used
    // for shadow mapping calculations.

    if (box_size <= 0.0f)
        logErrorAndThrow(name_, "CameraBox::update()",
            "Far clipping plane value out of range: {0.0 < VALUE}.");

    GLboolean planes_updated = false;
    if (box_size != distance_far_ || recalculate_planes_size_)
    {
        distance_far_ = box_size;
        calculatePlanesSize();

        planes_updated = true;
    }

    if (camera_vectors_changed_ || planes_updated || camera_matrix_changed_)
    {
        // Vectors pointing at center points of view planes
        glm::vec3 to_near_center = forward_vector_ * distance_near_;
        glm::vec3 to_far_center = forward_vector_ * distance_far_;

        calculateBox(to_near_center, to_far_center);
        findMinAndMax();

        camera_vectors_changed_ = false;
        camera_matrix_changed_ = false;
        box_updated_ = true;
    }
}

void CameraBox::calculateBox(const glm::vec3 &center_near,
    const glm::vec3 &center_far)
{
    // -UP vector = DOWN vector
    // -RIGHT vector = LEFT vector
    glm::vec3 far_top = center_far + up_vector_ * (far_height_ / 2.0f);
    glm::vec3 far_bottom = center_far + -up_vector_ * (far_height_ / 2.0f);
    glm::vec3 near_top = center_near + up_vector_ * (near_height_ / 2.0f);
    glm::vec3 near_bottom = center_near + -up_vector_ * (near_height_ / 2.0f);

    constexpr int points_count = 8;
    glm::vec3 points[points_count];
    points[0] = calculatePointPosition(far_top, right_vector_, far_width_);
    points[1] = calculatePointPosition(far_top, -right_vector_, far_width_);
    points[2] = calculatePointPosition(far_bottom, right_vector_, far_width_);
    points[3] = calculatePointPosition(far_bottom, -right_vector_, far_width_);
    points[4] = calculatePointPosition(near_top, right_vector_, near_width_);
    points[5] = calculatePointPosition(near_top, -right_vector_, near_width_);
    points[6] = calculatePointPosition(near_bottom, right_vector_, near_width_);
    points[7] = calculatePointPosition(near_bottom, -right_vector_, near_width_);

    box_points_.clear();
    for (GLint i = 0; i < points_count; i++)
        box_points_.push_back(points[i]);
}

glm::vec3 CameraBox::calculatePointPosition(const glm::vec3 &start,
    const glm::vec3 &direction, GLfloat width) const
{
    glm::vec3 point = start + direction * (width / 2.0f);
    point = glm::vec3(view_matrix_inverted_ * glm::vec4(point, 1.0f));

    return point;
}

void CameraBox::findMinAndMax()
{
    GLboolean first = true;
    for (const auto &point : box_points_)
    {
        if (first)
        {
            minX_ = point.x;
            maxX_ = point.x;
            minY_ = point.y;
            maxY_ = point.y;
            minZ_ = point.z;
            maxZ_ = point.z;
            first = false;
            continue;
        }

        if (point.x > maxX_)
            maxX_ = point.x;
        else if (point.x < minX_)
            minX_ = point.x;

        if (point.y > maxY_)
            maxY_ = point.y;
        else if (point.y < minY_)
            minY_ = point.y;

        if (point.z > maxZ_)
            maxZ_ = point.z;
        else if (point.z < minZ_)
            minZ_ = point.z;
    }

    GLfloat x = (minX_ + maxX_) / 2.0f;
    GLfloat y = (minY_ + maxY_) / 2.0f;
    GLfloat z = (minZ_ + maxZ_) / 2.0f;

    center_ = glm::vec3(x, y, z);
}

void CameraBox::recalculateSpace(const glm::mat4 &space_matrix)
{
    // Recalculates box points from world space to other space, e.g. to
    // light space for shadow mapping

    // Recalculate only if box points were updated -> prevent from multiple
    // recalculating points to the same space
    if (box_updated_)
    {
        for (auto &point : box_points_)
            point = glm::vec3(space_matrix * glm::vec4(point, 1.0f));

        findMinAndMax();

        box_updated_ = false;
    }
}

void CameraBox::setCameraParameters(GLfloat distance_near, GLfloat aspect,
    GLfloat fov)
{
    if (distance_near != distance_near_ || aspect != aspect_ || fov != fov_)
    {
        distance_near_ = distance_near;
        aspect_ = aspect;
        fov_ = fov;

        recalculate_planes_size_ = true;
    }
}

void CameraBox::calculatePlanesSize()
{
    // Calculates distances to clipping planes of the box
    far_width_ = distance_far_ * std::tan(fov_ / 2.0f) * 2.0f;
    near_width_ = distance_near_ * std::tan(fov_ / 2.0f) * 2.0f;

    far_height_ = far_width_ / aspect_;
    near_height_ = near_width_ / aspect_;

    recalculate_planes_size_ = false;
}

void CameraBox::setCameraMatrix(const glm::mat4 &view_matrix_inverted)
{
    if (view_matrix_inverted != view_matrix_inverted_)
    {
        view_matrix_inverted_ = view_matrix_inverted;
        camera_matrix_changed_ = true;
    }
}

void CameraBox::setCameraVectors(const glm::vec3 &direction,
    const glm::vec3 &up, const glm::vec3 &right)
{
    forward_vector_ = direction;
    up_vector_ = up;
    right_vector_ = right;

    camera_vectors_changed_ = true;
}