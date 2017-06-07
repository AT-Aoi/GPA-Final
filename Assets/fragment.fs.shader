#version 410

layout(location = 0) out vec4 fragColor;

uniform mat4 um4mv;
uniform mat4 um4p;

in vec2 vv2tex_coord;

uniform sampler2D tex;

void main()
{
    vec3 texColor = texture(tex, vv2tex_coord).rgb;
    fragColor = vec4(texColor, 1.0);
}