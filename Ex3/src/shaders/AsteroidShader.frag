#version 330

// Texture
uniform sampler2D textureSampler1;

in vec2 texcoords;
out vec4 outColor;

void main()
{
    vec4 texel = texture(textureSampler1, texcoords);
    if (texel.a < 0.5f) {
        discard;
    }
    outColor = texel;
}
