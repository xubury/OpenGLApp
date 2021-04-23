#ifndef AXIS_HPP
#define AXIS_HPP

#include <Graphic/Drawable.hpp>
#include <Graphic/VertexBuffer.hpp>

class Arrow : public Drawable {
   public:
    enum Dir { DIR_X, DIR_Y, DIR_Z };

   public:
    Arrow();

    void create(Dir dir, int size);

    virtual void draw(RenderTarget &target, RenderStates states) const override;

   private:
    VertexBuffer m_cone;
    VertexBuffer m_line;
};

class Axis : public Drawable {
   public:
    Axis();

    void create(int xSize, int ySize, int zSize);

    virtual void draw(RenderTarget &target, RenderStates states) const override;

   private:
    Arrow m_axes[3];
};

#endif
