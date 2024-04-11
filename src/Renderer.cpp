#include "Renderer.h"
#include "glad/glad.h"
#include "ErrorCheck.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <array>
#include <vector>
#include <unordered_map>

#define MAX_VERTEX_COUNT 10000
#define MAX_INDEX_COUNT MAX_VERTEX_COUNT * 6
#define MAX_TEXTURE_COUNT 32

struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoord;
    glm::vec3 Normal;
};

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

struct RendererContext {
    uint32_t vb;
    uint32_t ib;
    uint32_t va;
    uint32_t sh;

    std::array<uint32_t, MAX_TEXTURE_COUNT> TextureSlots;
    uint32_t TextureSlotsTop = 1;

    Vertex VertexBuffer[MAX_VERTEX_COUNT];
    uint32_t VertexCount = 0;



    std::unordered_map<std::string, int> UniCache;
};

static RendererContext renderer;

void Renderer::Init()
{
    GLCall(glGenBuffers(1, &renderer.vb));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer.vb));
    GLCall(glBufferData(GL_ARRAY_BUFFER, MAX_VERTEX_COUNT * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

    GLCall(glGenVertexArrays(1, &renderer.va));
    GLCall(glBindVertexArray(renderer.va));

    GLCall(glEnableVertexArrayAttrib(renderer.va, 0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, Position)));

    GLCall(glEnableVertexArrayAttrib(renderer.va, 1));
    GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, TexCoord)));

    GLCall(glEnableVertexArrayAttrib(renderer.va, 2));
    GLCall(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, Normal)));

    // IndexBuffer
    int IndexBuffer[MAX_INDEX_COUNT];
    for (int i = 0; i < MAX_INDEX_COUNT; i ++)
    {
        IndexBuffer[i] = i;
    }

    GLCall(glGenBuffers(1, &renderer.ib));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer.ib));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexBuffer), IndexBuffer, GL_STATIC_DRAW));

    for (int i = 0; i < MAX_TEXTURE_COUNT; i++)
        renderer.TextureSlots[i] = 0;

    GLCall(glGenTextures(1, &renderer.TextureSlots[0]));
    GLCall(glBindTexture(GL_TEXTURE_2D, renderer.TextureSlots[0]));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

    uint32_t yellow = 0xff00ffff;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &yellow));
}

void Renderer::Shutdown()
{
    GLCall(glDeleteBuffers(1, &renderer.vb));
    GLCall(glDeleteBuffers(1, &renderer.ib));
    GLCall(glDeleteBuffers(1, &renderer.va));
}

void Renderer::BeginBatch()
{
    renderer.VertexCount = 0;
}

void Renderer::EndBatch()
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, renderer.vb));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, renderer.VertexCount * sizeof(Vertex), &renderer.VertexBuffer[0]));
}

void Renderer::Flush()
{
    for (uint32_t i = 0; i < renderer.TextureSlotsTop; i++) {
        GLCall(glBindTextureUnit(i, renderer.TextureSlots[i]));
    }

    GLCall(glBindVertexArray(renderer.va));
    GLCall(glDrawElements(GL_TRIANGLES, renderer.VertexCount, GL_UNSIGNED_INT, nullptr));

    renderer.TextureSlotsTop = 1;
}

static inline void BufferPushBack(Object &object, int i)
{
    int vi = object.f_buffer[i] - 1; 
    renderer.VertexBuffer[renderer.VertexCount].Position = 
        glm::vec3(object.v_buffer[vi * 3], object.v_buffer[vi * 3 + 1], object.v_buffer[vi * 3 + 2]);

    int vti = object.f_buffer[i+1] - 1;
    renderer.VertexBuffer[renderer.VertexCount].TexCoord = 
        glm::vec2(object.vt_buffer[vti * 2], object.vt_buffer[vti * 2 + 1]);

    int vni = object.f_buffer[i+2] - 1;
    renderer.VertexBuffer[renderer.VertexCount].Normal =
        glm::vec3(object.vn_buffer[vni * 3], object.vn_buffer[vni * 3 + 1], object.vn_buffer[vni * 3 + 2]);

    renderer.VertexCount++;
}

void Renderer::DrawObject(Object &object)
{

    for (int i = 0; i < object.f_buffer.size(); i += 12) {
        if (renderer.VertexCount + 6 > MAX_VERTEX_COUNT) {
            EndBatch();
            Flush();
            BeginBatch();
        }
        if (object.f_buffer[i+9] == 0) {
            BufferPushBack(object, i);
            BufferPushBack(object, i+3);
            BufferPushBack(object, i+6);
        } else  {
            BufferPushBack(object, i);
            BufferPushBack(object, i+3);
            BufferPushBack(object, i+6);
            BufferPushBack(object, i+3);
            BufferPushBack(object, i+6);
            BufferPushBack(object, i+9);
        }
    }
}

#pragma region CompileShader_inline
inline static ShaderProgramSource ParseShader(const std::string &filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            if (type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

inline static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[1024];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

inline static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

#pragma endregion

void Renderer::CompileShader(const std::string &filepath)
{
    ShaderProgramSource source = ParseShader(filepath);
    renderer.sh = CreateShader(source.VertexSource, source.FragmentSource);
}

#pragma region SetUniform_inline

inline static int GetUniformLocation(const std::string& name)
{
    if (renderer.UniCache.find(name) != renderer.UniCache.end())
        return renderer.UniCache[name];

    GLCall(int location = glGetUniformLocation(renderer.sh, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform " << name << ", doesn't exist!" << std::endl;
    
    renderer.UniCache[name] = location;
    return location;
}

#pragma endregion

void Renderer::SetUniform(const std::string& name, const glm::mat4 & data)
{
    GLCall(glUseProgram(renderer.sh));
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &data[0][0]));
}

void Renderer::SetShader()
{
    GLCall(glUseProgram(renderer.sh));
}
