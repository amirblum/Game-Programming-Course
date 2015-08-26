#version 330

#define NUM_SAMPLES (3)

// Textures
uniform sampler2D fbo_color_texture;
uniform sampler2D fbo_depth_texture;
uniform mat4 inverseViewProjection;
uniform mat4 prevViewProjection;

// PP
in vec2 UV;
out vec4 color;

void main() {
//    color = texture(fbo_color_texture, UV);
//    return;
    
    float depth = texture(fbo_depth_texture, UV).x;
    vec4 viewportPos = vec4(UV.x * 2 - 1, (1 - UV.y) * 2 - 1, depth, 1);
    
    vec4 worldPos = inverseViewProjection * viewportPos;
    worldPos /= worldPos.w;
    
    vec4 prevPos = prevViewProjection * worldPos;
    prevPos /= prevPos.w;
    
    vec2 velocity = (viewportPos - prevPos).xy / 2.0f;
    
    vec2 texCoord = UV;
    vec4 outColor = texture(fbo_color_texture, texCoord);
    texCoord += velocity;
    
    for (int i = 0; i < NUM_SAMPLES; ++i, texCoord += velocity) {
        outColor += texture(fbo_color_texture, texCoord);
    }
    
    color = outColor / NUM_SAMPLES;
}