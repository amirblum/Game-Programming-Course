#version 330

layout(location = 0) in vec4 position;

uniform mat4 W;
uniform mat4 PV;

out vec2 myPositionPercent;

void main()
{
    // Start with base position
    myPositionPercent = position.xy + vec2(0.5f);
    
    // Set the world position
    gl_Position = PV * W * position;
}