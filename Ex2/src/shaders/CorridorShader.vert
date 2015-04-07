#version 330

layout(location = 0) in vec4 position;

uniform mat4 wvp;
uniform float offset;

out vec3 texcoords;

void main()
{
    // Start with base position
    texcoords = position.xyz;
    // Add the offset
    texcoords += vec3(0.0f, 0.0f, offset);
    
    // Set the world position
    gl_Position = wvp * position;
}
