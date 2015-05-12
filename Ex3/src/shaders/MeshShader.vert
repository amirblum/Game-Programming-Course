#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoords;
layout(location = 2) in vec3 normal;

uniform mat4 W;
uniform mat4 PV;

out vec3 myWorldPosition;
out vec2 myTexcoords;
out vec3 myNormal;

void main()
{
    vec4 myPosition = vec4(position, 1.0f);
    myTexcoords = texcoords;
    myNormal = normal;
    
    // Set the world position
    myWorldPosition = (W * myPosition).xyz;
    gl_Position = PV * W * myPosition;
}
