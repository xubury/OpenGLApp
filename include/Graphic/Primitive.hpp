#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <glm/glm.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <vector>

class Camera;

class PrimitiveShader : public Shader {
   public:
    static PrimitiveShader &instance();

   private:
    PrimitiveShader();
};

class Primitive {
   public:
    static Primitive &instance();

    void setDrawingView(const Camera *camera);

    void drawLine(const glm::vec3 &start, const glm::vec3 &end,
                  const glm::vec4 &color, float thickness);

    void drawPath(const std::vector<glm::vec3> &pts,
                  const std::vector<glm::vec4> &colors, float thickness);

    void drawCircle(const glm::vec3 &center, float radius,
                    const glm::vec4 &color,  int fragments = 0);

    void drawCircleFilled(const glm::vec3 &center, float radius,
                    const glm::vec4 &color,  int fragments = 0);

   private:
    Primitive();

    VertexBuffer m_vertices;
};

#endif /* PRIMITIVE_HPP */
