#ifndef SAHDER_HPP
#define SAHDER_HPP

#include <stdint.h>
#include <string>

class Shader {
   public:
    uint32_t id;
    void load(const std::string& vertexPath, const std::string& fragmentPath);
    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, int value) const;

   private:
    static void checkCompileErrors(uint32_t shader, const std::string type);
};

#endif
