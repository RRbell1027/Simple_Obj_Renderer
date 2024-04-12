#shader vertex
#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 vertexNormal;

layout(location = 3) in vec4 m1;
layout(location = 4) in vec4 m2;
layout(location = 5) in vec4 m3;
layout(location = 6) in vec4 m4;

uniform mat4 u_View;
uniform mat4 u_Proj;

out vec3 o_Normal;

void main()
{
   mat4 model = mat4(m1, m2, m3, m4);
   gl_Position = u_Proj * u_View * model * vec4(position, 1.0);
   o_Normal = vertexNormal;
};

#shader fragment
#version 450 core

layout (location = 0) out vec4 o_Color;

in vec3 o_Normal;

void main()
{

   float ambi = 0.2; 

   vec3 norm = normalize(o_Normal);
   vec3 lightDir = vec3(0.0, 1.0, 0.0);
   float diff = max(dot(norm, lightDir), 0.0);

   vec3 lightColor = vec3(1.0, 0.8, 0.5);

   o_Color = vec4((ambi + diff) * lightColor, 1.0);
};