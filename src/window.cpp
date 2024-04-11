#include <iostream>
#include <vector>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "glm/gtc/matrix_transform.hpp"

#include "Renderer.h"
#include "ObjLoader.h"
#include "Camera.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

Camera *cameraptr;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    // Init glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to load glad.");
        glfwTerminate();
        return -1;
    }

    glEnable(GL_DEPTH_TEST);


    Renderer::Init();

    Object suzanne("../asset/suzanne.obj");

    Renderer::CompileShader("../shader/basic.shader");
    Renderer::SetShader();
    

    // glm::mat4 projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -2.0f, 2.0f);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 15.0f / 12.0f, -2.0f, 2.0f);
    Renderer::SetUniform("u_Proj", projection);

    Camera camera;
    cameraptr = &camera;
    glm::mat4 view = glm::mat4(1.0f);
    
    glm::mat4 model = glm::mat4(1.0f);
    Renderer::SetUniform("u_Model", model);

    
    glfwSwapInterval(1);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        
        
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cameraptr->Update(view);
        Renderer::SetUniform("u_View", view);

        model = glm::rotate(model, 0.1f, glm::vec3(0.0f, 1.0f, 0.0f));
        Renderer::SetUniform("u_Model", model);

        Renderer::BeginBatch();
        Renderer::DrawObject(suzanne);
        Renderer::EndBatch();

        Renderer::Flush();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    Renderer::Shutdown();
    glfwTerminate();
    return 0;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    cameraptr->SetFrontByCursor(xpos, ypos);
}