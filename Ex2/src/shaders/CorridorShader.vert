#version 330

layout(location = 0) in vec4 position;

uniform mat4 W;
uniform mat4 PV;
uniform float offset;

out vec3 myWorldPosition;
out vec3 texcoords;

void main()
{
    // Start with base position
    texcoords = position.xyz;
    // Add the offset
    texcoords.z += offset;
    
    // Set the world position
    myWorldPosition = (W * position).xyz;
    gl_Position = PV * W * position;
}
