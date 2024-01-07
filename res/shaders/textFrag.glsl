#version 330 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 sourceColor;

const float min_tex = 0.1;

void main()
{    
    vec4 tex = texture(text, TexCoords);
    vec4 sampled = vec4(1.0, 1.0, 1.0, tex.r);
    color = vec4(sourceColor, 1.0) * sampled;
    if (tex.x < min_tex && tex.y < min_tex && tex.z < min_tex) // fix not right rendering text border
    {
        discard;
    }
}  