#ifndef BUFFER_OBJECT_HPP
#define BUFFER_OBJECT_HPP

#include <Graphic/Export.hpp>
#include <Graphic/Vertex.hpp>
#include <Graphic/Drawable.hpp>
#include <Graphic/OpenGL.hpp>

class GRAPHIC_API BufferObject : public Drawable {
   public:
    typedef void (*AttrFunc)();

   protected:
    std::size_t m_size;
    GLenum m_mode;
    GLenum m_primitiveType;
    AttrFunc m_attrFunction;

   public:
    BufferObject();

    ~BufferObject();

    BufferObject(const BufferObject &other);

    BufferObject(BufferObject &&other);

    friend void swap(BufferObject &first, BufferObject &second);

    void draw(RenderTarget &target, RenderStates states) const override;

    bool isInit() const;

    virtual bool initialize();

    virtual void drawPrimitive() const = 0;

    std::size_t size() const;

    bool empty() const;

    void bindVertexArray() const;

   private:
    uint32_t m_VAO;
};

#endif /* BUFFER_OBJECT_HPP */
