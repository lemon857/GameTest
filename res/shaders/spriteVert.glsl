#version 410
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 texture_coord;

out vec2 texCoord;

uniform mat4 modelMat;
uniform mat4 view_projectionMat;

void main() 
{
   texCoord = texture_coord;
   gl_Position = view_projectionMat * modelMat * vec4(vertex_position, 1.0);   
}