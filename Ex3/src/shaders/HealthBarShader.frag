#version 330

#define BORDER_PERCENT (0.05)

in vec2 myPositionPercent;

uniform int maxUnits;
uniform int currentUnits;

out vec4 outColor;

void main()
{
    // If border, finish
    if (myPositionPercent.x < BORDER_PERCENT || myPositionPercent.x > 1.0f - BORDER_PERCENT ||
        myPositionPercent.y < BORDER_PERCENT || myPositionPercent.y > 1.0f - BORDER_PERCENT) {
        outColor = vec4(1.0f); // White
        return;
    }
    
    // Else, draw health
    float healthPercent = float(currentUnits) / maxUnits;
    if (1.0f - myPositionPercent.x < healthPercent) {
        outColor = vec4(0.0f, 1.0f, 0.0f, 0.5f); // Transparent green
    } else {
        outColor = vec4(1.0f, 0.0f, 0.0f, 0.5f); // Transparent red
    }
}
