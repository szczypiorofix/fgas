#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 pos;
uniform vec2 texXY;
// uniform vec2 texWH;


void main() {
    gl_Position = pos * vec4(aPos, 0.0, 1.0);
    ourColor = aColor;
    TexCoord = vec2(aTexCoord.x + texXY.x, aTexCoord.y) + texXY.y;
}
