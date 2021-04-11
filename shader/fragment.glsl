#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture0;

void main() {
    fragColor = texture(texture0, texCoord);
}

