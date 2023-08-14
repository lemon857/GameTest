#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 2) in vec2 texture_coord;

out vec4 colors;

uniform mat4 modelMat;
uniform mat4 view_projectionMat;
uniform vec4 sourceColor;

void main() 
{
	colors = sourceColor;
	gl_Position = view_projectionMat * modelMat * vec4(vertex_position, 1.0);   
}