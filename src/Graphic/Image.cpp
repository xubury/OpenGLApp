#include "Graphic/Image.hpp"
#include "Core/Assert.hpp"
#include "Core/Log.hpp"
#include <fstream>
#include <jpeglib.h>
#include <png.h>

namespace te {

bool Image::loadFromFile(const std::string &filename, bool flip) {
    std::string ext = filename.substr(filename.find_last_of('.') + 1);
    if (ext == "jpg" || ext == "jpeg") {
        return loadJPEG(filename, flip);
    } else if (ext == "png") {
        return loadPNG(filename, flip);
    }

    TE_CORE_ERROR("Unsupported image format");
    return false;
}

void Image::allocate() {
    TE_CORE_ASSERT(m_header.bitDepth % 8 == 0, "Invalid bit-depth");
    m_buffer.resize(m_header.width * m_header.height * m_header.channels *
                    m_header.bitDepth / 8);
}

GLenum Image::getGLFormat() const {
    GLenum format = GL_RGB;
    if (m_header.channels == 1) {
        format = GL_RED;
    } else if (m_header.channels == 2) {
        format = GL_RG;
    } else if (m_header.channels == 3) {
        format = GL_RGB;
    } else if (m_header.channels == 4) {
        format = GL_RGBA;
    } else {
        TE_CORE_ERROR("Unsupported image channels");
    }
    return format;
}

GLenum Image::GetGLType() const {
    GLenum type = GL_UNSIGNED_BYTE;
    if (m_header.bitDepth == 8) {
        type = GL_UNSIGNED_BYTE;
    } else if (m_header.bitDepth == 16) {
        type = GL_UNSIGNED_SHORT;
    } else {
        TE_CORE_ERROR("Unsupported image bit-depth");
    }
    return type;
}

static void jpgErrorHandler(j_common_ptr /*cinfo*/) {
    throw std::runtime_error("Image::loadJPEG format not recognized");
}

static void jpgMsgHandler(j_common_ptr /*cinfo*/, int msg_level) {
    if (msg_level < 0) throw std::runtime_error("Image::loadJPEG data corrupt");
}

bool Image::loadJPEG(const std::string &filename, bool flip) {
    FILE *fp = std::fopen(filename.c_str(), "rb");
    if (fp == nullptr) {
        TE_CORE_ERROR("Image::loadJPEG Error opening file: {}",
                      std::strerror(errno));
        return false;
    }

    jpeg_decompress_struct cinfo;
    jpeg_error_mgr jerr;
    cinfo.err = jpeg_std_error(&jerr);
    jerr.error_exit = &jpgErrorHandler;
    jerr.emit_message = &jpgMsgHandler;
    try {
        jpeg_create_decompress(&cinfo);
        jpeg_stdio_src(&cinfo, fp);

        int ret = jpeg_read_header(&cinfo, static_cast<boolean>(false));
        if (ret != JPEG_HEADER_OK) {
            throw std::runtime_error("Image::loadJPEG header not recognized");
        }
        if (cinfo.out_color_space != JCS_GRAYSCALE &&
            cinfo.out_color_space != JCS_RGB) {
            throw std::runtime_error("Image::loadJPEG Invalid color space");
        }
        /* Start decompression. */
        jpeg_start_decompress(&cinfo);

        m_header.width = cinfo.image_width;
        m_header.height = cinfo.image_height;
        m_header.channels = (cinfo.out_color_space == JCS_RGB ? 3 : 1);
        m_header.bitDepth = 8;
        allocate();

        uint8_t *data_ptr;
        uint32_t stride = channels() * cinfo.output_width;
        if (flip) {
            data_ptr = &data()[(cinfo.output_height - 1) * stride];
        } else {
            data_ptr = &data()[0];
        }
        while (cinfo.output_scanline < cinfo.output_height) {
            jpeg_read_scanlines(&cinfo, &data_ptr, 1);
            if (flip) {
                data_ptr -= stride;
            } else {
                data_ptr += stride;
            }
        }

        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        std::fclose(fp);
    } catch (const std::exception &e) {
        TE_CORE_ERROR("{}", e.what());
        jpeg_destroy_decompress(&cinfo);
        std::fclose(fp);
        return false;
    }
    return true;
}

bool Image::loadPNG(const std::string &filename, bool flip) {
    FILE *fp = std::fopen(filename.c_str(), "rb");
    if (fp == nullptr) {
        TE_CORE_ERROR("Error opening file: {}", std::strerror(errno));
        return false;
    }
    try {
        png_structp png =
            png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        if (!png) throw std::runtime_error("Image::loadPNG Out of memory");

        png_infop info = png_create_info_struct(png);
        if (!info) throw std::runtime_error("Image::loadPNG Out of memory");

        png_init_io(png, fp);

        png_read_info(png, info);

        m_header.width = png_get_image_width(png, info);
        m_header.height = png_get_image_height(png, info);
        m_header.channels = png_get_channels(png, info);
        png_byte color_type = png_get_color_type(png, info);
        m_header.bitDepth = png_get_bit_depth(png, info);
        allocate();

        if (m_header.bitDepth == 16) png_set_strip_16(png);

        if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
        // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
        if (color_type == PNG_COLOR_TYPE_GRAY && m_header.bitDepth < 8)
            png_set_expand_gray_1_2_4_to_8(png);

        if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);

        // These color_type don't have an alpha channel then fill it with 0xff.
        if (color_type == PNG_COLOR_TYPE_RGB ||
            color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_PALETTE)
            png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

        if (color_type == PNG_COLOR_TYPE_GRAY ||
            color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
            png_set_gray_to_rgb(png);

        png_read_update_info(png, info);
        std::vector<png_bytep> row_pointers;
        row_pointers.resize(m_header.height);
        for (int i = 0; i < m_header.height; ++i)
            if (flip) {
                row_pointers[i] =
                    &m_buffer[(m_header.height - 1 - i) * m_header.width *
                              m_header.channels * m_header.bitDepth / 8];
            } else {
                row_pointers[i] =
                    &m_buffer[i * m_header.width * m_header.channels *
                              m_header.bitDepth / 8];
            }
        png_read_image(png, row_pointers.data());

        std::fclose(fp);
    } catch (const std::exception &e) {
        TE_CORE_ERROR("{}", e.what());
        std::fclose(fp);
        return false;
    }
    return true;
}

}  // namespace te
