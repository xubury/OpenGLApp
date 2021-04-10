#ifndef SAHDER_HPP
#define SAHDER_HPP

#include <stdint.h>
#include <string>

class Shader {
   public:
    uint32_t id;
    void load(const std::string& vertexPath, const std::string& fragmentPath);
    void use();

   private:
    static void checkCompileErrors(uint32_t shader, const std::string type);
};

#endif
