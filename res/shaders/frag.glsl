#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform vec4 newColor;

void main()
{
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor.x + newColor.x, ourColor.y + newColor.y, ourColor.z + newColor.z, 1.0);
}
