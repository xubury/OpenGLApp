#include "Core/LayerStack.hpp"
#include <algorithm>

namespace te {
LayerStack::LayerStack() : m_layerInsertId(0) {}

LayerStack::~LayerStack() {
    for (auto &layer : m_layers) {
        layer->onDetech();
    }
    m_layers.clear();
}

void LayerStack::pushLayer(Ref<Layer> layer) {
    m_layers.emplace(m_layers.begin() + m_layerInsertId, layer);
    ++m_layerInsertId;
    layer->onAttach();
}

void LayerStack::pushOverlay(Ref<Layer> overlay) {
    m_layers.emplace_back(overlay);
    overlay->onAttach();
}

void LayerStack::popLayer(Ref<Layer> layer) {
    auto iter =
        std::find(m_layers.begin(), m_layers.begin() + m_layerInsertId, layer);
    if (iter != m_layers.begin() + m_layerInsertId) {
        layer->onDetech();
        m_layers.erase(iter);
        --m_layerInsertId;
    }
}

void LayerStack::popOverlay(Ref<Layer> overlay) {
    auto iter =
        std::find(m_layers.begin() + m_layerInsertId, m_layers.end(), overlay);
    if (iter != m_layers.end()) {
        overlay->onDetech();
        m_layers.erase(iter);
    }
}

}  // namespace te
