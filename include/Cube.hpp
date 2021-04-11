#ifndef CUBE_HPP
#define CUBE_HPP

#include <stdint.h>

#include "Drawable.hpp"

class Cube : public Drawable {
   public:
    Cube();

    virtual ~Cube() = default;

    virtual void draw(RenderWindow &window) const override;

   private:
    static uint32_t m_VBO;
    static uint32_t m_VAO;
    static uint32_t m_EBO;
};

#endif
