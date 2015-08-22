#version 330

layout(location = 0) in vec4 position;

uniform mat4 W;
uniform mat4 PV;

void main()
{
   gl_Position = PV * W * position;
}
