#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coord;

out vec2 texCoord;
out vec3 frag_normal;
out vec3 frag_position;

uniform mat4 modelMat;
uniform mat4 view_projectionMat;

void main() 
{
	texCoord = texture_coord;

	vec4 vertex_pos_world = modelMat * vec4(vertex_position, 1.0);

	frag_normal = mat3(transpose(inverse(modelMat))) * vertex_normal;
	frag_position = vertex_pos_world.xyz;
	gl_Position = view_projectionMat * vertex_pos_world;  
}