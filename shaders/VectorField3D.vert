#version 330 core
layout (location = 0) in vec3 aPos;

layout (std140) uniform uVectorFieldData
{
    mat4 ModelMatrices[MAX_ARROW_COUNT];
    vec4 Color[MAX_ARROW_COUNT];
};
uniform mat4 uViewProjection;

out float vInterpolatedMagnitude;
out vec4  vColor;

void main(void)
{
    gl_Position = uViewProjection * ModelMatrices[gl_InstanceID] * vec4(aPos.xyz, 1.0);
    vColor = Color[gl_InstanceID];
}
