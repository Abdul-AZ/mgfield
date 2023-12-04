#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uViewProjection;

void main(void)
{
    gl_Position = uViewProjection * vec4(aPos.xyz, 1.0);
}
