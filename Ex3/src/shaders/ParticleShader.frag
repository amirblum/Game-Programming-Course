#version 330

// Texture
uniform sampler2D textureSampler1;

in vec2 texcoords;
in float myTransparency;
in vec3 myTint;

out vec4 outColor;

void main()
{
    vec4 texel = texture(textureSampler1, texcoords);
    
    // Discard transparent pixels (this assumes pixels are either fully opaque
    // or fully transparent
    if (texel.a < 0.5f) {
        discard;
    }
    
    // Set the global transparency
    texel.a = myTransparency;
    
    // Tint
    texel.r *= myTint.r;
    texel.g *= myTint.g;
    texel.b *= myTint.b;
    
    outColor = texel;
}
