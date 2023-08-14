#version 460
layout(location = 0) in vec3 vertex_position;

uniform mat4 modelMat;
uniform mat4 view_projectionMat;

void main() 
{
   gl_Position = view_projectionMat * modelMat * vec4(vertex_position * 0.1, 1.0);   
}