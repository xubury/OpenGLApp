#include "Application.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

Application::Application(int width, int height, const std::string& title)
    : m_window(width, height, title) {
    m_shader.load("shader/vertex.glsl", "shader/fragment.glsl");

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_shader.setInt("texture1", texture);

    int texWidth;
    int texHeight;
    int nChannels;

    stbi_set_flip_vertically_on_load(true);
    const char* path = "resources/textures/container.jpg";
    uint8_t* data = stbi_load(path, &texWidth, &texHeight, &nChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture." << std::endl;
    }
    stbi_image_free(data);
}

void Application::update() {}

void Application::render() {
    m_window.clear();
    m_shader.use();
    m_cube.draw(m_window);
    m_window.swapBuffers();
}

void Application::run() {
    while (!m_window.shouldClose()) {
        m_window.pollEvents();
        update();
        render();
    }
    m_window.close();
}
