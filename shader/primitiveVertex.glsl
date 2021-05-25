#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

out vec4 color;

uniform float screenWidth;
uniform float screenHeight;

void main() {
    vec4 screenPos = vec4(aPos, 1.0);

    screenPos.x = screenPos.x / screenWidth * 2 - 1; 
    screenPos.y = 1 - screenPos.y / screenHeight * 2;

    gl_Position = screenPos;
    color = aColor;
}

