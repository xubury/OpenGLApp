#ifndef SAHDER_HPP
#define SAHDER_HPP

#include "Core/Base.hpp"
#include "Core/Export.hpp"
#include <string>
#include <stdint.h>
#include <glm/glm.hpp>
#include <unordered_map>

namespace te {

class TE_API Shader {
   public:
    Shader();

    ~Shader() = default;

    Shader(const Shader&) = delete;

    Shader& operator=(const Shader&) = delete;

    void compile(const char* vertexCode, const char* fragmentCode,
                 const char* geometryCode = nullptr);

    void load(const std::string& vertexPath, const std::string& fragmentPath,
              const std::string& geometryPath = "");

    void bind() const;

    void unbind() const;

    void setBool(const std::string& name, bool value) const;

    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;

    void setVec4(const std::string& name, const glm::vec4& value) const;

    void setMat4(const std::string& name, const glm::mat4& value) const;

   private:
    static void checkCompileErrors(uint32_t shader, const std::string type);

    uint32_t m_id;
};

class TE_API ShaderLibrary {
   public:
    void add(const std::string& name);

    Ref<Shader> get(const std::string& name);

    bool exists(const std::string& name) const;

   private:
    std::unordered_map<std::string, Ref<Shader>> m_library;
};

}  // namespace te

#endif
