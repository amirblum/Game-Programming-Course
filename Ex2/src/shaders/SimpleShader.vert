#version 330

layout(location = 0) in vec4 position;

uniform mat4 wvp;

void main()
{
   gl_Position = wvp*position;
   //gl_Position = position;
}
