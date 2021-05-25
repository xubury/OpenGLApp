#include <Graphic/Shader.hpp>
#include <glad/glad.h>
#include <Graphic/Vertex.hpp>

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() {}

void Shader::loadFromFile(const std::string& vertexPath,
                          const std::string& fragmentPath) {
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
    compile(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::compile(const char* vertexCode, const char* fragmentCode) {
    // compile shaders
    // vertex shader
    uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
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

PrimitiveShader& PrimitiveShader::instance() {
    static PrimitiveShader s_instance("shader/primitiveVertex.glsl",
                                      "shader/primitiveFragment.glsl");
    return s_instance;
}

PrimitiveShader::PrimitiveShader(const std::string& vertexPath,
                                 const std::string& fragmentPath) {
    loadFromFile(vertexPath, fragmentPath);
}
