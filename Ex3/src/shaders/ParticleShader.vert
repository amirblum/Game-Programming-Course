#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 particleCenter;
layout(location = 2) in float particleSize;
layout(location = 3) in float transparency;

uniform mat4 W;
uniform mat4 PV;

uniform vec3 cameraRight;
uniform vec3 cameraUp;

out vec2 texcoords;
out float myTransparency;

void main()
{
    // Start with base position
    texcoords = position.xy + vec2(0.5f);
    
    // Transparency
    myTransparency = transparency;
    
    // Set the world position
    vec3 myLocalPosition = particleCenter;
    myLocalPosition += cameraRight * position.x * particleSize;
    myLocalPosition += cameraUp * position.y * particleSize;
    
    gl_Position = PV * W * vec4(myLocalPosition, 1.0f);
}