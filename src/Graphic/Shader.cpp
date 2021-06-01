#include <Graphic/Shader.hpp>
#include <Graphic/Vertex.hpp>
#include <Graphic/FrameBuffer.hpp>
#include <Graphic/Primitive.hpp>

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <iostream>

#include <glbinding/gl/gl.h>
using namespace gl;

void Shader::initDefaultShaders() {
    const char* fbVertex =
        "#version 330 core\n"
        "layout (location = 0) in vec2 aPos;\n"
        "layout (location = 1) in vec2 aTexCoords;\n"
        "out vec2 texCoords;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
        "    texCoords = aTexCoords;\n"
        "}";

    const char* fbFragment =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "in vec2 texCoords;\n"
        "uniform sampler2D uScreenTexture;\n"
        "void main() {\n"
        "    fragColor = texture(uScreenTexture, texCoords);\n"
        "}";

    FrameBuffer::s_shader.compile(fbVertex, fbFragment);
    FrameBuffer::s_shader.use();
    FrameBuffer::s_shader.setInt("uScreenTexture", 0);

    const char* shadowVertex =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 uLightSpaceMatrix;\n"
        "uniform mat4 uModel;\n"
        "void main() {\n"
        "    gl_Position = uLightSpaceMatrix * uModel * vec4(aPos, 1.0);\n"
        "}";
    const char* shadowFragment =
        "#version 330 core\n"
        "void main() {\n"
        "}";

    ShadowBuffer::s_shadowShader.compile(shadowVertex, shadowFragment);

    const char* primitiveVertex =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "uniform mat4 uProjection; "
        "uniform mat4 uView;"
        "void main() {\n"
        "    gl_Position = uProjection * uView * vec4(aPos, 1.0f);\n"
        "}";

    const char* primitiveFragment =
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "uniform vec4 uColor;\n"
        "void main() {\n"
        "    fragColor = uColor;\n"
        "}";
    Primitive::s_shader.compile(primitiveVertex, primitiveFragment);
}

Shader::Shader() : m_id(0) {}

void fileToString(const std::string& path, std::string& string) {
    std::ifstream file;
    // ensure ifstream objects can throw exceptions:
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // open files
        file.open(path);
        std::stringstream fileStream;
        // read file's buffer contents into streams
        fileStream << file.rdbuf();
        // close file handlers
        file.close();
        // convert stream into string
        string = fileStream.str();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
}

void Shader::load(const std::string& vertexPath,
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
    glShaderSource(vertex, 1, &vertexCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // fragment Shader
    uint32_t fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    uint32_t geometry = 0;
    if (geometryCode != nullptr) {
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometryCode, NULL);
        glCompileShader(geometry);
        checkCompileErrors(geometry, "GEOMETRY");
    }
    // shader Program
    m_id = glCreateProgram();
    glAttachShader(m_id, fragment);
    glAttachShader(m_id, vertex);
    if (geometry != 0) glAttachShader(m_id, geometry);

    glLinkProgram(m_id);
    checkCompileErrors(m_id, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer
    // necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometry != 0) glDeleteShader(geometry);
}

void Shader::use() const { glUseProgram(m_id); }

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
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
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
