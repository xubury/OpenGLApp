#include "Core/Assert.hpp"
#include "Utils/File.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/OpenGL.hpp"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <iostream>

namespace te {

Shader::Shader() : m_id(0) {}


void Shader::loadFromFile(const std::string& vertexPath,
                          const std::string& fragmentPath,
                          const std::string& geometryPath) {
    std::string vertexCode;
    std::string fragmentCode;
    fileToString(vertexPath, vertexCode);
    fileToString(fragmentPath, fragmentCode);
    if (!geometryPath.empty()) {
        std::string geometryCode;
        fileToString(geometryPath, geometryCode);
        compile(vertexCode.c_str(), fragmentCode.c_str(), geometryCode.c_str());
    }
    compile(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::compile(const char* vertexCode, const char* fragmentCode,
                     const char* geometryCode) {
    // compile shaders
    // vertex shader
    uint32_t vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "Vertex");
    // fragment Shader
    uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "Fragment");

    uint32_t geometry = 0;
    if (geometryCode != nullptr) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometryCode, nullptr);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "Geometry");
    }
    // shader Program
    m_id = glCreateProgram();
    glAttachShader(m_id, fragment);
    glAttachShader(m_id, vertex);
    if (geometry != 0) glAttachShader(m_id, geometry);

    glLinkProgram(m_id);
    checkCompileErrors(m_id, "Program");
    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometry != 0) glDeleteShader(geometry);
}

void Shader::bind() const { glUseProgram(m_id); }

void Shader::unbind() const { glUseProgram(0); }

void Shader::checkCompileErrors(uint32_t shader, const std::string &type) {
    int success;
    std::string infoLog(1024, 0);
    if (type != "Program") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog.data());
            TE_CORE_ERROR("{0} Shader compilation error:\n{1}\n", type,
                          infoLog);
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog.data());
            TE_CORE_ERROR("Shader Program linking error:\n{0}\n", infoLog);
        }
    }
}

void Shader::setBool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const {
    glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const {
    glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(value));
}

void Shader::setUniformBlock(const std::string& name,
                             uint32_t bindingPoint) const {
    uint32_t index = glGetUniformBlockIndex(m_id, name.c_str());
    if (index != GL_INVALID_INDEX)
        glUniformBlockBinding(m_id, index, bindingPoint);
}

void ShaderLibrary::add(const std::string& name) {
    m_library.emplace(name, createRef<Shader>());
}

Ref<Shader> ShaderLibrary::get(const std::string& name) {
    TE_CORE_ASSERT(exists(name),
                   "ShaderLibrary::get requires for a non-existent shader");
    return m_library[name];
}

bool ShaderLibrary::exists(const std::string& name) const {
    return m_library.find(name) != m_library.end();
}

}  // namespace te
