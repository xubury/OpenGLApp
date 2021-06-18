#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <stdint.h>
#include <string>

namespace te {

class Image {
   public:
    Image();

    ~Image();

    Image(const Image &) = delete;

    Image &operator=(const Image &) = delete;

    static void setFlip(bool flip);

    bool loadFromFile(const std::string &path);

    bool valid() const { return m_data != nullptr; }

    const uint8_t *data() const { return m_data; }

    int width() const { return m_width; }

    int height() const { return m_height; }

    int nChannels() const { return m_nChannels; }

   private:
    uint8_t *m_data;
    int m_width;
    int m_height;
    int m_nChannels;
};

}  // namespace te

#endif /* IMAGE_HPP */
