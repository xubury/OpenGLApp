#include <iostream>

#include "GLWindow.hpp"

void GLWindow::framebufferSizeCB(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void GLWindow::errorCallback(int error, const char* description) {
    std::cerr << "Error: " << description << ", Code: " << error << std::endl;
}

GLWindow::GLWindow() : m_window(nullptr), m_initilaized(false) { initialize(); }

GLWindow::~GLWindow() {
    if (m_initilaized) close();
}

bool GLWindow::initialize() {
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
        // positions         // colors
        0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom left
        0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f   // top
    };
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    m_initilaized = true;
    return true;
}

void GLWindow::processInput() {
    if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(m_window, true);
}

void GLWindow::pollEvents() {
    glfwPollEvents();
    processInput();
}

void GLWindow::update() {}

void GLWindow::render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_shader.use();
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(m_window);
}

bool GLWindow::shouldClose() { return glfwWindowShouldClose(m_window); }

void GLWindow::run() {
    if (!m_initilaized) {
        return;
    }
    while (!shouldClose()) {
        pollEvents();
        update();
        render();
    }
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);

    close();
}

void GLWindow::close() { glfwTerminate(); }
