#version 330 core

out vec4 FragColor;

uniform vec4 uMeshColor;

void main(void)
{
    FragColor = uMeshColor;
}
