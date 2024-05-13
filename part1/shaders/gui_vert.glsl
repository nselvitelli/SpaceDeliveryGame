#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;

uniform mat4 projection;
uniform mat4 model;

out vec2 TexCoords;

void main ()
{
    TexCoords = texCoord;
    gl_Position = projection * model * vec4(position, 1.0);
}