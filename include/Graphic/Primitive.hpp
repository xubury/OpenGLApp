#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <glm/glm.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/VertexBuffer.hpp>
#include <Graphic/ElementBuffer.hpp>
#include <vector>

class Camera;

class Primitive {
   public:
    static Primitive &instance();

    void setDrawingView(const Camera *camera);

    void drawLine(const DebugVertex &start, const DebugVertex &end,
                  float thickness);

    void drawPath(const std::vector<DebugVertex> &pts, float thickness);

    void drawCircle(const DebugVertex &center, float radius, int fragments = 0);

    void drawCircleFilled(const DebugVertex &center, float radius,
                          int fragments = 0);

    void drawQuad(const std::vector<DebugVertex> &corners, float thickness);

    void drawQuadFilled(const std::vector<DebugVertex> &corners);

   private:
    Primitive();

    VertexBuffer m_vertices;

    ElementBuffer m_elements;

   private:
    friend class Shader;
    static Shader s_shader;
};

#endif /* PRIMITIVE_HPP */
