#version 410

// Minimal shader — only needs to write gl_Position for depth.
// Works for both indexed (Sphere) and non-indexed (TriMesh) geometry.

layout(location = 0) in vec3 in_Position;

uniform mat4 lightSpaceMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = lightSpaceMatrix * modelMatrix * vec4(in_Position, 1.0);
}
