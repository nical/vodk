#version 130

uniform sampler2D in_Texture;
in vec2 tex_coords;

void main(void)
{
    gl_FragColor = (texture2D(in_Texture, tex_coords)
                   + vec4(tex_coords, 0.0, 1.0)) / 2.0;
}
