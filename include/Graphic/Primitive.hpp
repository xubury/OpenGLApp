#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include "Core/Export.hpp"
#include "Graphic/Shader.hpp"
#include "Graphic/VertexArray.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace te {

class CameraBase;

class TE_API Primitive {
   public:
    static Primitive &instance();

    void setDrawingView(const CameraBase &camera);

    void drawLine(const glm::vec3 &start, const glm::vec3 &end,
                  const glm::vec4 &color, float thickness);

    void drawPath(const std::vector<glm::vec3> &pts, const glm::vec4 &color,
                  float thickness);

    void drawCircle(const glm::vec3 &center, const glm::vec3 &normal,
                    const glm::vec4 &color, float radius, int fragments = 100);

    void drawCircleFilled(const glm::vec3 &center, const glm::vec3 &normal,
                          const glm::vec4 &color, float radius,
                          int fragments = 100);

    // TODO: optmize sphere drawing
    void drawSphere(const glm::vec3 &center, const glm::vec4 &color,
                    float radius, int sectorCount = 10, int stackCount = 10);

    void drawQuad(const std::vector<glm::vec3> &corners, const glm::vec4 &color,
                  float thickness);

    void drawQuadFilled(const std::vector<glm::vec3> &corners,
                        const glm::vec4 &color);

    void drawCube(const glm::vec3 &min, const glm::vec3 &max,
                  const glm::vec4 &color);

    void drawCubeFilled(const glm::vec3 &min, const glm::vec3 &max,
                        const glm::vec4 &color);

   private:
    Primitive();

   private:
    Ref<Shader> m_shader;
    Ref<VertexArray> m_vertexArray;
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer> m_indexBuffer;
};

}  // namespace te

#endif /* PRIMITIVE_HPP */
