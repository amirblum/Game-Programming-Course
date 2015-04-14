#version 330

// Texture
uniform sampler2D textureSampler;

// Light
uniform vec3 lightPos;
uniform vec3 lightDir;
uniform float lightIntensity;
uniform float lightCutoff;
uniform float lightDarkenStart;
uniform float lightDarkenEnd;

in vec3 myWorldPosition;
in vec2 texcoords;
out vec4 outColor;

void main()
{
    float darkenRatio = 0.0f;
    if (-myWorldPosition.z < lightDarkenStart) {
        darkenRatio = 1.0 - (-myWorldPosition.z / lightDarkenStart);
    }
    outColor = texture(textureSampler, texcoords) * darkenRatio;
}
