#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 particleCenter;

uniform mat4 W;
uniform mat4 PV;

uniform vec3 cameraRight;
uniform vec3 cameraUp;

out vec2 texcoords;

void main()
{
    // Start with base position
    texcoords = position.xy + vec2(0.5f);
    
    // Set the world position
    vec3 myLocalPosition = particleCenter;
    myLocalPosition += cameraRight * position.x * 2.0f;
    myLocalPosition += cameraUp * position.y * 2.0f;
    
    gl_Position = PV * W * vec4(myLocalPosition, 1.0f);
}