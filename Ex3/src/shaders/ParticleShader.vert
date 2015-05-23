#version 330

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 localPosition;
layout(location = 2) in float size;
layout(location = 3) in float transparency;
layout(location = 4) in vec3 tint;

uniform mat4 W;
uniform mat4 PV;

uniform vec3 cameraWorldPosition;

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
    vec4 centerWorldPosition = W * vec4(localPosition, 1.0f);
    
    vec3 toCamera = normalize(cameraWorldPosition - centerWorldPosition.xyz);
    
    vec3 billboardUp = normalize(cross(toCamera, vec3(1.0f, 0.0f, 0.0f))); // Normalize to fix case where cross between vectors that are almost parralel isn't normalized
    vec3 billboardRight = -cross(toCamera, billboardUp);
    
    centerWorldPosition += vec4(billboardRight, 0.0f) * position.x * size;
    centerWorldPosition += vec4(billboardUp, 0.0f) * position.y * size;
    
    gl_Position = PV * centerWorldPosition;
}