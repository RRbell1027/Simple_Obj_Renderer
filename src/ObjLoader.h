#pragma once

#include "glm/glm.hpp"
#include <string>
#include <vector>

class ObjectLoader {
    public:
    ObjectLoader(const std::string &filepath);
    void Show();

    std::vector<float> v_buffer;
    std::vector<float> vt_buffer;
    std::vector<float> vn_buffer;
    std::vector<int> f_buffer;   
};

class Object {
    public:
    
    Object(glm::mat4 m, ObjectLoader * ol)
    : model(m), objectLoader(ol)
    {}

    void Update(float deltaTime);

    glm::mat4 model;
    ObjectLoader *objectLoader;
};
