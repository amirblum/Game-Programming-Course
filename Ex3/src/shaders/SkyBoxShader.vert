#version 330

layout(location = 0) in vec4 position;

uniform mat4 W;
uniform mat4 PV;

out vec3 texcoords;

void main()
{
    // Start with base position
    texcoords = position.xyz;
    
    gl_Position = (PV * W * position).xyww;
}