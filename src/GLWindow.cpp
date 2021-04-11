#include <iostream>
#include "RenderWindow.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void RenderWindow::framebufferSizeCB(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void RenderWindow::errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << ", Code: " << error << std::endl;
}

RenderWindow::RenderWindow() : m_window(nullptr), m_initilaized(false) {
    initialize();
}

RenderWindow::~RenderWindow() {
    if (m_initilaized) close();
}

bool RenderWindow::initialize() {
    if (m_initilaized) return true;

    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        std::cout << "Failed to initialize glfw." << std::endl;
        return false;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (m_window == NULL) {
        std::cout << "Failed to create GLFW window." << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCB);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD." << std::endl;
        return false;
    }

    m_shader.load("shader/vertex.glsl", "shader/fragment.glsl");
    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };
    unsigned int indices[] = {
        0, 1, 3,  // first triangle
        1, 2, 3   // second triangle
    };
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture coord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    uint32_t texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_shader.setInt("texture1", texture);

    int width;
    int height;
    int nChannels;

    stbi_set_flip_vertically_on_load(true);
    const char* path = "resources/textures/container.jpg";
    uint8_t* data = stbi_load(path, &width, &height, &nChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture." << std::endl;
    }
    stbi_image_free(data);

    m_initilaized = true;
    return true;
}

void RenderWindow::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

void RenderWindow::processEvents() {
    glfwPollEvents();
    processInput();
}

void RenderWindow::update() {}

void RenderWindow::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader.use();
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(m_window);
}

bool RenderWindow::shouldClose() { return glfwWindowShouldClose(m_window); }

void RenderWindow::run() {
    if (!m_initilaized) {
        return;
    }
    while (!shouldClose()) {
        processEvents();
        update();
        render();
    }
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);

    close();
}

void RenderWindow::close() { glfwTerminate(); }
