#include "Graphic/Image.hpp"
#include "stb_image.h"

namespace te {

Image::Image() : m_data(nullptr), m_width(0), m_height(0), m_nChannels(0) {}

Image::~Image() { stbi_image_free(m_data); }

void Image::setFlip(bool flip) { stbi_set_flip_vertically_on_load(flip); }

bool Image::loadFromFile(const std::string &path) {
    m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_nChannels, 0);
    return valid();
}

}  // namespace te
