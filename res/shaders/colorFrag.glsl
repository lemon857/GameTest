#version 410
out vec4 frag_color;

uniform vec4 sourceColor;

void main() 
{
   gl_FragColor = sourceColor;
}