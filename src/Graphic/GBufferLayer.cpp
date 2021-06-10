#include "Graphic/GBufferLayer.hpp"

namespace te {

GBufferLayer::GBufferLayer(int width, int height) {
    FrameBufferSpecification spec;
    spec.width = width;
    spec.height = height;
    spec.attachmentsSpecs = {
        {FramebufferTextureFormat::RGBA16F},
        {FramebufferTextureFormat::RGBA16F},
        {FramebufferTextureFormat::RGBA8},
    };
}

}  // namespace te
