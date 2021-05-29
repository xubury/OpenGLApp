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

    void drawLine(const DebugVertex &start, const DebugVertex &end,
                  float thickness);

    void drawPath(const std::vector<DebugVertex> &pts, float thickness);

    void drawCircle(const DebugVertex &center, float radius,
                    int fragments = 100);

    void drawCircleFilled(const DebugVertex &center, float radius,
                          int fragments = 100);

    // TODO: optmize sphere drawing
    void drawSphere(const DebugVertex &center, float radius,
                    int fragments = 100);

    void drawQuad(const std::vector<DebugVertex> &corners, float thickness);

    void drawQuadFilled(const std::vector<DebugVertex> &corners);

    void drawCube(const glm::vec3 &min, const glm::vec3 &max,
                  const glm::vec4 &color);

    void drawCubeFilled(const glm::vec3 &min, const glm::vec3 &max,
                        const glm::vec4 &color);

   private:
    Primitive();

    VertexBuffer m_vertices;

    ElementBuffer m_elements;

   private:
    friend class Shader;
    static Shader s_shader;
};

#endif /* PRIMITIVE_HPP */
