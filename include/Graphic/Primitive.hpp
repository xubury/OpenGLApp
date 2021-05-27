#ifndef PRIMITIVE_HPP
#define PRIMITIVE_HPP

#include <glm/glm.hpp>
#include <Graphic/Shader.hpp>
#include <Graphic/VertexBuffer.hpp>
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

   private:
    Primitive();

    VertexBuffer m_vertices;

   private:
    friend class Shader;
    static Shader s_shader;
};

#endif /* PRIMITIVE_HPP */
