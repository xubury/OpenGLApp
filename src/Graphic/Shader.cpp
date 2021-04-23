#include <Graphic/Shader.hpp>
#include <glad/glad.h>
#include <Graphic/Vertex.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() : m_VAO(0) {}

Shader::~Shader() { glDeleteVertexArrays(1, &m_VAO); }

void Shader::loadFromFile(const std::string& vertexPath,
                          const std::string& fragmentPath) {
    glGenVertexArrays(1, &m_VAO);
    if (!m_VAO) {
        std::cerr << "Could not create vertex buffer." << std::endl;
        return;
    }
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    compile(vertexCode, fragmentCode);
}

void Shader::compile(const std::string& vertexCode,
                     const std::string& fragmentCode) {
    // compile shaders
    // vertex shader
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::bind() const { glBindVertexArray(m_VAO); }

void Shader::setupAttribute() const {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, color));

    // texture coord
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)(offsetof(Vertex, texCoord)));

    // normal attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)(offsetof(Vertex, normal)));
}

void Shader::use() const { glUseProgram(id); }

void Shader::checkCompileErrors(uint32_t shader, const std::string type) {
    int success;
    std::string infoLog(1024, 0);
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog.data());
            std::cout
                << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                << infoLog
                << "\n -- --------------------------------------------------- "
                   "-- "
                << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog.data());
            std::cout
                << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                << infoLog
                << "\n -- --------------------------------------------------- "
                   "-- "
                << std::endl;
        }
    }
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(id, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE,
                       &value[0][0]);
}

DebugShader& DebugShader::instance() {
    static DebugShader s_instance("shader/debugVertex.glsl",
                                  "shader/debugFragment.glsl");
    return s_instance;
}

DebugShader::DebugShader(const std::string& vertexPath,
                         const std::string& fragmentPath) {
    loadFromFile(vertexPath, fragmentPath);
}

void DebugShader::setDrawingMode(DrawingMode mode) { m_drawingMode = mode; }

void DebugShader::setupAttribute() const {
    glBindVertexArray(m_VAO);
    switch (m_drawingMode) {
        case DrawingMode::NORMAL:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case DrawingMode::WIRE_FRAME:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
    }
    // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex),
                          (void*)0);

    // color attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugVertex),
                          (void*)offsetof(DebugVertex, color));
}
