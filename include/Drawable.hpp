#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

class RenderTarget;

class RenderStates;

class Drawable {
   public:
    virtual void draw(RenderTarget &target, RenderStates states) const = 0;
};

#endif
