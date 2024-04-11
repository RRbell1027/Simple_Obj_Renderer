
#include "glm/gtc/matrix_transform.hpp"
#include "Camera.h"

Camera::Camera()
{
    position = glm::vec3(0.0f, 0.0f, 10.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

bool first = false;
float lastX, lastY;
void Camera::SetFrontByCursor(double xpos, double ypos)
{
    if (!first){
        lastX = xpos;
        lastY = ypos;
        first = true;
    }
    float sensitivity = 0.1f;

    rotate.x += (xpos - lastX) * sensitivity;
    rotate.y += (lastY - ypos) * sensitivity;
    lastX = xpos;
    lastY = ypos;

    if (rotate.y > 89.0f)
        rotate.y = 89.0f;
    if (rotate.y < -89.0f)
        rotate.y = -89.0f;

    front = glm::vec3(
        cos(glm::radians(rotate.y)) * cos(glm::radians(rotate.x)),
        sin(glm::radians(rotate.y)),
        cos(glm::radians(rotate.y)) * sin(glm::radians(rotate.x))
    );

    front = glm::normalize(front);
}

void Camera::Update(glm::mat4 & view)
{
    view = glm::lookAt(position, position + front, up);
}
