#version 330 core

uniform samplerCube skybox;

in vec3 TexCoords;

out vec4 FragColor;

void main()
{   
    FragColor = texture(skybox, TexCoords);
    // FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}