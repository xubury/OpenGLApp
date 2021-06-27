#ifndef LAYOUT_HPP
#define LAYOUT_HPP

#include <string>
#include <stdint.h>
#include "Core/Assert.hpp"

namespace te {

enum class ShaderDataType {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool
};

class TE_API BufferElement {
   public:
    BufferElement() = default;

    BufferElement(ShaderDataType type, const std::string &name,
                  bool normalized = false);

    uint32_t getComponentCount() const;

    ShaderDataType getType() const;

    uint32_t getSize() const;

    std::size_t getOffset() const;

    bool isNormalized() const;

   private:
    friend class BufferLayout;

    ShaderDataType m_type;
    std::string m_name;
    uint32_t m_size;
    std::size_t m_offset;
    bool m_normalized;
};

class TE_API BufferLayout {
   public:
    BufferLayout() = default;

    BufferLayout(std::initializer_list<BufferElement> elements);

    std::vector<BufferElement>::iterator begin();

    std::vector<BufferElement>::iterator end();

    std::vector<BufferElement>::const_iterator begin() const;

    std::vector<BufferElement>::const_iterator end() const;

    std::size_t getStride() const;

   private:
    void calculateOffsetAndStride();

    std::vector<BufferElement> m_elements;

    std::size_t m_stride;
};

}  // namespace te

#endif
