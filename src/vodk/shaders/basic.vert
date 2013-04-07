#version 130

in vec3 in_Position;
in vec2 in_TexCoords;

uniform mat4 in_ModelView;
uniform mat4 in_Transform;

out vec2 tex_coords;

void main(void)
{
  gl_Position = in_ModelView * in_Transform * vec4(in_Position, 1.0);
  tex_coords = in_TexCoords;
}
