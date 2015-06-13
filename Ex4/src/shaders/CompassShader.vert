#version 330

layout(location = 0) in vec4 position;

uniform vec3 beaconPosition;
uniform mat4 view, projection;
uniform float aspectRatio;

out vec2 texcoords;

void main()
{
    // determine texture coordinates
    texcoords = position.xy + 0.5;
    
    // move position to the line connecting eye and object, but at distance 1,
    // so that it always appear at constant size on the screen
    vec4 pos = projection * view * vec4(beaconPosition, 1.0f);
    pos /= pos.w;
    
    // if the object is behind camera, move towards screen edge
    if((view * vec4(beaconPosition, 1.0f)).z > 0.0f){
        pos.xy *= -200.0f;
    }
    
    // clamp screen coordinates, so that the crosshair always appears on the screen
    pos.xy = clamp(pos.xy, vec2(-1.0f, -1.0f), vec2(1.0f ,1.0f));
    
    // Offset for square
    vec3 aspects = vec3(1.0f, aspectRatio, 1.0f);
    pos += vec4(position.xyz * aspects * 0.1f, 0.0f);
    pos.z = 0;
    
    gl_Position = pos;
}
