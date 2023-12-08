#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 uModelViewProjection;

void main(void)
{
    gl_Position = uModelViewProjection * vec4(aPos.xyz, 1.0);
}
