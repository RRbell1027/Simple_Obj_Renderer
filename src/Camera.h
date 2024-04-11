#pragma once

#include <glm/glm.hpp>

class Camera {
    public:
    Camera();
    void SetFrontByCursor(double xpos, double ypos);
    void Update(glm::mat4 & view);
    private:
    glm::vec3 front;
    glm::vec3 position;
    glm::vec3 up;
    glm::vec3 rotate;
};