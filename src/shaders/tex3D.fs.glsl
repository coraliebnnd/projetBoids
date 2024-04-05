#version 330

in vec2 vTextcoords;

out vec4 fFragColor;

uniform sampler2D uTexture;

void main()
{
    vec4 color = texture(uTexture, vTextcoords);
    fFragColor = vec4(color.xyz,1.);
}