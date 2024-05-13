#version 330 core

uniform sampler2D u_DiffuseMap;

in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    vec4 texColor = texture(u_DiffuseMap, TexCoords);

    if(texColor.r + texColor.g + texColor.b < 0.1) {
        texColor.a = 0.0;
    }
    FragColor = texColor;
}