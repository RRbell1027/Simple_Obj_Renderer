#pragma once

#include "glm/glm.hpp"
#include "ObjLoader.h"
#include <vector>
#include <string>

class Renderer
{
    public:
    static void Init();
    static void Shutdown();
    
    static void BeginBatch();
    static void EndBatch();

    static void Flush();

    static void DrawObject(Object &Object);

    static void SetShader();

    static void CompileShader(const std::string& shaderpath);
    static void SetUniform(const std::string& name, const glm::mat4 & data);
};