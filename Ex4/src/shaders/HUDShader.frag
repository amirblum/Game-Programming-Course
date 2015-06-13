#version 330

// Texture
uniform sampler2D textureSampler1;

in vec2 texcoords;
out vec4 outColor;

void main()
{
    vec4 texel = texture(textureSampler1, texcoords);
//    texel.a = 0.3;
    outColor = texel;
}
