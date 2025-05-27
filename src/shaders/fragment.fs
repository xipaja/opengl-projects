#version 330 core
out vec4 FragColor;

in vec3 customColor;
in vec2 TexCoord;

uniform sampler2D customTexture;

void main()
{
    // FragColor = vec4(customColor, 1.0f);
    FragColor = texture(customTexture, TexCoord) * vec4(customColor, 1.0);
}