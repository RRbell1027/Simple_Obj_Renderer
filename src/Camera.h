#pragma once

#include <glm/glm.hpp>

class Camera {
    public:
    Camera();
    void SetFrontByCursor(double xpos, double ypos);
    void SetVelocity(glm::vec3 v);
    void Update(glm::mat4 & view, float deltaTime);
    private:
    glm::vec3 front;
    glm::vec3 rotate;

    glm::vec3 position;
    glm::vec3 velocity;
    
    glm::vec3 up;
};