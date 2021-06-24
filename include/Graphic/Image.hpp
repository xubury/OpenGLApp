#ifndef IMAGE_HPP
#define IMAGE_HPP

#include "Graphic/OpenGL.hpp"
#include <stdint.h>
#include <string>
#include <vector>

namespace te {

struct ImageHeader {
    int width;
    int height;
    int channels;
    uint8_t bitDepth;
};

class Image {
   public:
    Image() = default;

    ~Image() = default;

    Image(const Image &) = delete;

    Image &operator=(const Image &) = delete;

    bool loadFromFile(const std::string &filename, bool flip = false);

    bool valid() const { return m_buffer.size(); }

    const uint8_t *data() const { return m_buffer.data(); }

    uint8_t *data() { return m_buffer.data(); }

    int width() const { return m_header.width; }

    int height() const { return m_header.height; }

    int channels() const { return m_header.channels; }

    int bitDepth() const { return m_header.bitDepth; }

    void allocate();

    GLenum getGLFormat() const;

    GLenum GetGLType() const;

   private:
    bool loadJPEG(const std::string &filename, bool flip);

    bool loadPNG(const std::string &filename, bool flip);

    std::vector<uint8_t> m_buffer;
    ImageHeader m_header;
};

}  // namespace te

#endif /* IMAGE_HPP */
