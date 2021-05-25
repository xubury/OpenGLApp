#ifndef SAHDER_HPP
#define SAHDER_HPP

#include <stdint.h>
#include <string>
#include <glm/glm.hpp>

class Shader {
   public:
    uint32_t id;

    Shader();

    Shader(const Shader&) = delete;

    Shader& operator=(const Shader&) = delete;

    void compile(const char* vertexCode, const char* fragmentCode);

    void loadFromFile(const std::string& vertexPath,
                      const std::string& fragmentPath);

    void use() const;

    void setBool(const std::string& name, bool value) const;

    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;

    void setVec4(const std::string& name, const glm::vec4& value) const;

    void setMat4(const std::string& name, const glm::mat4& value) const;

   private:
    static void checkCompileErrors(uint32_t shader, const std::string type);
};

class PrimitiveShader : public Shader {
   public:
    static PrimitiveShader& instance();

   private:
    PrimitiveShader(const std::string& vertexPath, const std::string& fragmentPath);
};

#endif
