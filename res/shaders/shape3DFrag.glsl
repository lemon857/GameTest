#version 460
in vec2 texCoord;
out vec4 frag_color;

uniform sampler2D tex;
uniform vec3 light_color;
uniform float ambient_factor;

void main() 
{	
	// ambient
	vec3 ambient = ambient_factor * light_color;
	
	// diffuse

	vec3 diffuse = vec3(0.f);
	
	// specular
	
	vec3 specular = vec3(0.f);
	
	frag_color = texture(tex, texCoord) * vec4(ambient + diffuse + specular, 1.f);   
	if (frag_color.rgb == vec3(0.0))
	{
		discard;
	}
}