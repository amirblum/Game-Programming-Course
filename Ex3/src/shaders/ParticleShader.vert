#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 localPosition;
layout(location = 2) in float size;
layout(location = 3) in float transparency;
layout(location = 4) in vec3 tint;

uniform mat4 W;
uniform mat4 PV;

uniform vec3 cameraRight;
uniform vec3 cameraUp;

out vec2 texcoords;
out float myTransparency;
out vec3 myTint;

void main()
{
    // Start with base position
    texcoords = position.xy + vec2(0.5f);
    
    // Transparency
    myTransparency = transparency;
    
    // Tint
    myTint = tint;
    
    // Set the world position
    vec4 myWorldPosition = W * vec4(localPosition, 1.0f);
    myWorldPosition += vec4(cameraRight, 0.0f) * position.x * size;
    myWorldPosition += vec4(cameraUp, 0.0f) * position.y * size;
    
    gl_Position = PV * myWorldPosition;
}