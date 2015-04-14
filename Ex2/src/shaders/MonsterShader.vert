#version 330

layout(location = 0) in vec4 position;

uniform mat4 W;
uniform mat4 PV;

out vec3 myWorldPosition;
out vec2 texcoords;

void main()
{
    // Start with base position
    texcoords = position.xy;
    // Normalize to (0,1)
    texcoords += vec2(0.5f);
    texcoords.y = 1.0f - texcoords.y;
    
    // Set the world position
    myWorldPosition = (W * position).xyz;
    gl_Position = PV * W * position;
}
