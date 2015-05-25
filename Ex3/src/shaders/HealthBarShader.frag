#version 330

#define BORDER_PERCENT (0.05)

in vec2 myPositionPercent;

uniform int maxUnits;
uniform int currentUnits;

out vec4 outColor;

void main()
{
//    // If border, finish
//    if (myPositionPercent.x < BORDER_PERCENT || myPositionPercent.x > 1.0f - BORDER_PERCENT ||
//        myPositionPercent.y < BORDER_PERCENT || myPositionPercent.y > 1.0f - BORDER_PERCENT) {
//        outColor = vec4(1.0f); // White
//        return;
//    }
    
    // Draw green health
    vec4 color = vec4(0.0f, 0.6f, 0.0f, 1.0f); // Green
    
    // Draw red where applicable
    float healthPercent = float(currentUnits) / maxUnits;
    if (1.0f - myPositionPercent.x >= healthPercent) {
        color = vec4(0.6f, 0.0f, 0.0f, 1.0f); // Red
    }
    
//    // If border, lower alpha
//    if (myPositionPercent.x < BORDER_PERCENT || myPositionPercent.x > 1.0f - BORDER_PERCENT ||
//        myPositionPercent.y < BORDER_PERCENT || myPositionPercent.y > 1.0f - BORDER_PERCENT) {
//        color.a = 0.25f;
//    }
    
    outColor = color;
}
