#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform uModelMatricesBuffer
{
    mat4 uModelMatrices[32768];
};
uniform mat4 uViewProjection;

void main(void)
{
    gl_Position = uViewProjection * uModelMatrices[gl_InstanceID] * vec4(aPos.xyz, 1.0);
}
