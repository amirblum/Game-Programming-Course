#version 330

layout(location = 0) in vec4 position;

uniform mat4 W;
uniform mat4 PV;

uniform float size;
uniform vec2 billboardRight;
uniform vec3 cameraWorldPosition;

out vec2 texcoords;

void main()
{
    // Start with base position
    texcoords = position.xy + vec2(0.5f);
    
    // Set the world position
    vec4 centerWorldPosition = W * vec4(0.0f, 0.0f, 0.0f, 1.0f);
    
    vec3 toCamera = normalize(cameraWorldPosition - centerWorldPosition.xyz);
    
    vec3 billboardUp = normalize(cross(toCamera, vec3(billboardRight, 0.0f))); // Normalize to fix case where cross between vectors that are almost parralel isn't normalized
    vec3 billboardRight = -cross(toCamera, billboardUp);
    
    centerWorldPosition += vec4(billboardRight, 0.0f) * position.x * size;
    centerWorldPosition += vec4(billboardUp, 0.0f) * position.y * size;
    
    gl_Position = PV * centerWorldPosition;
}