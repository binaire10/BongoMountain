#ifndef GAMEENGINEBONGO_CAMERA_HPP
#define GAMEENGINEBONGO_CAMERA_HPP

#include <glm/mat4x4.hpp>

class Camera {
public:
    constexpr Camera() = default;
    [[maybe_unused]] constexpr explicit Camera(const glm::mat4 &projection) noexcept
        : projection(projection)
    {}
    virtual ~Camera() = default;

    [[nodiscard]] inline glm::mat4 getProjection() const noexcept
    {
        return projection;
    }

protected:
    glm::mat4 projection;
};

#endif// GAMEENGINEBONGO_CAMERA_HPP
