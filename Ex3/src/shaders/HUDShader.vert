#version 330

layout(location = 0) in vec4 position;

out vec2 texcoords;

void main()
{
    // Start with base position
    texcoords = vec2((position.x + 1.0f) / 2.0f, (-position.y + 0.5f));
    
    gl_Position = position;
}