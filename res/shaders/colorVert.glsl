#version 410
layout(location = 0) in vec3 vertex_position;
layout(location = 2) in vec2 texture_coord;

uniform mat4 modelMat;
uniform mat4 view_projectionMat;

void main() 
{
	gl_Position = view_projectionMat * modelMat * vec4(vertex_position, 1.0);   
}