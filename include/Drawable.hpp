#ifndef DRAWABLE_HPP
#define DRAWABLE_HPP

class RenderWindow;

class Drawable {
   public:
    Drawable();
    virtual void draw(RenderWindow &window) const = 0;

   protected:
    bool m_initilaized;
};

#endif
