#version 330

// Texture
uniform sampler2D textureSampler;
uniform sampler2D bumpSampler;

// Light
uniform vec3 lightPos;
uniform vec3 lightDir;

in vec3 myWorldPosition;
in vec2 texcoords;
out vec4 outColor;

#define EPS (0.01)
#define BUMP_SCALE (2.0f)

void main()
{
//    vec3 lVector = normalize(lightPos - myWorldPosition);
//
//    vec3 diffuseCoefficient = texture(textureSampler, texcoords).xyz;
//    vec3 diffuseComponent = max(diffuseCoefficient * dot(lVector, normal), 0);
//
//    outColor = vec4(diffuseComponent, 1.0f);
//    outColor = clamp(myWorldPosition, 0.0f, 1.0f);
    outColor = vec4(1.0f);
}
