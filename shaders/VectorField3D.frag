#version 330 core

in float vInterpolatedMagnitude;

out vec4 FragColor;

void main(void)
{
    vec3 colStart = vec3(0.0,0.0, 1.0);
    vec3 colEnd = vec3(1.0, 0.0, 0.0);
    vec3 col = (colEnd - colStart) * vInterpolatedMagnitude + colStart;
    FragColor = vec4(col,1.0);
}
