#version 330

// Texture
uniform sampler2D textureSampler;

in vec3 myWorldPosition;
in vec2 myTexcoords;
in vec3 myNormal;
out vec4 outColor;

#define EPS (0.01)

vec3 lightPos = vec3(5.0f, 3.0f, -5.0f);

void main()
{
    vec3 lVector = normalize(lightPos - myWorldPosition);

    vec3 diffuseCoefficient = texture(textureSampler, myTexcoords).xyz;
//    vec3 diffuseCoefficient = vec3(0.5f, 0.5f, 0.5f);
//    vec3 diffuseCoefficient = vec3(0.5f, myTexcoords);
    vec3 normal = normalize(myNormal);
    vec3 diffuseComponent = max(diffuseCoefficient * dot(lVector, normal), 0);

    outColor = vec4(diffuseComponent, 1.0f);
//    outColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}
