#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <Graphic/Export.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Graphic/ElementBuffer.hpp>
#include <glm/glm.hpp>
#include <vector>

class CameraBase;

class GRAPHIC_API Primitive {
   public:
    static Primitive &instance();

    void setDrawingView(const CameraBase *camera);

    void drawLine(const glm::vec3 &start, const glm::vec3 &end,
                  const glm::vec4 &color, float thickness);

    void drawPath(const std::vector<glm::vec3> &pts, const glm::vec4 &color,
                  float thickness);

    void drawCircle(const glm::vec3 &center, const glm::vec4 &color,
                    float radius, int fragments = 100);

    void drawCircleFilled(const glm::vec3 &center, const glm::vec4 &color,
                          float radius, int fragments = 100);

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

    VertexBuffer m_vertices;

    ElementBuffer m_elements;

   private:
    Shader m_shader;
};

#endif /* PRIMITIVE_HPP */
