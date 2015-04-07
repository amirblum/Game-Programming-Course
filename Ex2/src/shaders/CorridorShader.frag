#version 330

uniform samplerCube textureSampler;

in vec3 texcoords;
out vec4 outColor;

void main()
{
    // Set color from texture
    vec3 sampleCoords = texcoords;
    if (sampleCoords.z > 0.5f) {
        sampleCoords.z -= 1.0f;
    } else if (sampleCoords.z < -0.5f) {
        sampleCoords.z += 1.0f;
    }
    outColor = texture(textureSampler, sampleCoords);
    
    // Check if visible by flashlight
}
