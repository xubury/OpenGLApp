#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

class RenderTarget;

class Drawable {
   public:
    Drawable();
    virtual void draw(RenderTarget &target) const = 0;

   protected:
    bool m_initilaized;
};

#endif
