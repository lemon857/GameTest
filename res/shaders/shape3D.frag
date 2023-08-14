#version 460
in vec2 texCoord;
out vec4 frag_color;

uniform sampler2D tex;

void main() 
{
   frag_color = texture(tex, texCoord);   
   if (frag_color.rgb == vec3(0.0))
   {
		discard;
   }
}