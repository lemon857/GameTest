#version 460
layout(location = 0) in vec2 vertex_position;

out vec4 colors;

uniform mat4 modelMat;
uniform mat4 projectionMat;
uniform float layer;
uniform vec4 sourceColor;

void main() 
{
	colors = sourceColor;
	gl_Position = projectionMat * modelMat * vec4(vertex_position, layer, 1.0);   
}