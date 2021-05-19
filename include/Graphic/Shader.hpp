#ifndef SAHDER_HPP
#define SAHDER_HPP

#include <stdint.h>
#include <string>
#include <glm/glm.hpp>

class Shader {
   public:
    uint32_t id;

    Shader();

    ~Shader();

    Shader(const Shader&) = delete;

    Shader& operator=(const Shader&) = delete;

    void compile(const std::string& vertexCode,
                 const std::string& fragmentCode);

    void loadFromFile(const std::string& vertexPath,
                      const std::string& fragmentPath);

    virtual void setupAttribute() const;

    void use() const;

    void setBool(const std::string& name, bool value) const;

    void setInt(const std::string& name, int value) const;

    void setFloat(const std::string& name, float value) const;

    void setVec3(const std::string& name, const glm::vec3& value) const;

    void setVec4(const std::string& name, const glm::vec4& value) const;

    void setMat4(const std::string& name, const glm::mat4& value) const;

    bool isInitialized() const;
   protected:
    uint32_t m_VAO;

   private:
    static void checkCompileErrors(uint32_t shader, const std::string type);

    bool m_initialized;
};

class DebugShader : public Shader {
   public:
    enum DrawingMode { NORMAL, WIRE_FRAME };

   public:
    static DebugShader& instance();

    void setDrawingMode(DrawingMode mode);

    void setupAttribute() const override;

   private:
    DebugShader(const std::string& vertexPath, const std::string& fragmentPath);

    DrawingMode m_drawingMode;
};

#endif
