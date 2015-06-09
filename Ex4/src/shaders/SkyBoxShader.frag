#version 330

// Texture
uniform samplerCube cubeSampler1;

in vec3 texcoords;

out vec4 outColor;

void main()
{
    outColor = texture(cubeSampler1, texcoords);
}
