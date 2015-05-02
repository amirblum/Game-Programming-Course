#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 particleCenter;

uniform mat4 W;
uniform mat4 PV;

uniform vec3 cameraRight;
uniform vec3 cameraUp;

out vec3 myWorldPosition;
out vec2 texcoords;

void main()
{
    // Start with base position
    texcoords = position.xy;
    
    // Set the world position
//    vec3 myLocalPosition = particleCenter;
    vec3 myLocalPosition = vec3(0.0f);
    myLocalPosition += vec3(1.0f, 0.0f, 0.0f) * position.x * 2.0f;
    myLocalPosition += vec3(0.0f, 1.0f, 0.0f) * position.y * 2.0f;
    
    vec4 myLocalPositionAffine = vec4(myLocalPosition, 1.0f);
    
    myWorldPosition = (W * myLocalPositionAffine).xyz;
    gl_Position = PV * W * myLocalPositionAffine;
//    gl_Position = PV * W * position;
}