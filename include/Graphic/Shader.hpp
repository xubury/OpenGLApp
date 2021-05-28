#ifndef SAHDER_HPP
#define SAHDER_HPP

#include <Graphic/Export.hpp>
#include <stdint.h>
#include <string>
#include <glm/glm.hpp>

class GRAPHIC_API Shader {
   public:
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

    uint32_t m_id;

   public:
    static void initDefaultShaders();

};

#endif
