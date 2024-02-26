#version 450

layout (location = 0) out vec3 fragColour;

vec3 positions[3] = vec3[] (
    vec3(0.f, -0.4f, 0.f),
    vec3(0.4f, 0.4f, 0.f),
    vec3(-0.4f, 0.4f, 0.f)
);

vec3 colours[3] = vec3[] (
    vec3(1.f, 0.f, 0.f),
    vec3(0.f, 1.f, 0.f),
    vec3(0.f, 0.f, 1.f)
);

void main()
{
    gl_Position = vec4(positions[gl_VertexIndex], 1.f);
    fragColour = colours[gl_VertexIndex];
}