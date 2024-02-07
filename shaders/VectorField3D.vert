#version 330 core
layout (location = 0) in vec3 aPos;

#define MAX_ARROW_COUNT (32768)

layout (std140) uniform uVectorFieldData
{
    mat4 ModelMatrices[MAX_ARROW_COUNT];
    vec4 InterpolatedMagnitude[(MAX_ARROW_COUNT + 3) / 4];
};
uniform mat4 uViewProjection;

out float vInterpolatedMagnitude;

void main(void)
{
    gl_Position = uViewProjection * ModelMatrices[gl_InstanceID] * vec4(aPos.xyz, 1.0);
    vInterpolatedMagnitude = InterpolatedMagnitude[gl_InstanceID / 4][gl_InstanceID % 4];
}
