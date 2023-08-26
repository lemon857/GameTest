#version 460
in vec2 texCoord;
out vec4 frag_color;

uniform sampler2D tex;

void main() 
{
   gl_FragColor = texture(tex, texCoord);   
   if (gl_FragColor == vec4(0.0))
   {
		discard;
   }
}