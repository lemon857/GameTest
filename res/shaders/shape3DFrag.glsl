#version 460
in vec2 texCoord;
in vec3 frag_normal;
in vec3 frag_position;
out vec4 frag_color;

uniform sampler2D tex;
uniform vec3 light_color;
uniform vec3 light_position;
uniform vec3 cam_position;
uniform float ambient_factor;
uniform float diffuse_factor;
uniform float specular_factor;
uniform float shininess;
uniform float metalic_factor;

void main() 
{	
	// ambient
	vec3 ambient = ambient_factor * light_color;
	
	// diffuse

	vec3 normal = normalize(frag_normal);
	vec3 light_direction = normalize(light_position - frag_position);	
	vec3 diffuse = diffuse_factor * light_color * max(dot(normal, light_direction), 0.0);

	// specular
	
	vec3 view_dir = normalize(cam_position - frag_position);
	vec3 reflect_dir = reflect(-light_direction, normal);
	vec3 specular;

	specular = specular_factor * (((light_color * (1.f - metalic_factor)) + (vec3(texture(tex, texCoord) * metalic_factor))) / 2) * pow(max(dot(view_dir, reflect_dir), 0.0), shininess);
	
	frag_color = texture(tex, texCoord) * vec4(ambient + diffuse + specular, 1.f);   
	if (frag_color == vec4(0.0))
	{
		discard;
	}
}