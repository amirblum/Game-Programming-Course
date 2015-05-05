#version 330

// Texture
uniform sampler2D textureSampler1;

in vec2 texcoords;
out vec4 outColor;

void main()
{
    outColor = texture(textureSampler1, texcoords);
}
