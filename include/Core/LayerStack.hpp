#ifndef LAYER_STACK_HPP
#define LAYER_STACK_HPP

#include <vector>
#include "Core/Base.hpp"
#include "Core/Layer.hpp"
#include "Core/Export.hpp"

namespace te {

class TE_API LayerStack {
   public:
    LayerStack(); 

    ~LayerStack();

    void pushLayer(Ref<Layer> layer);

    void pushOverlay(Ref<Layer> overlay);

    void popLayer(Ref<Layer> layer);

    void popOverlay(Ref<Layer> overlay);

    std::vector<Ref<Layer>>::iterator begin() { return m_layers.begin(); }

    std::vector<Ref<Layer>>::iterator end() { return m_layers.end(); }

    std::vector<Ref<Layer>>::reverse_iterator rbegin() {
        return m_layers.rbegin();
    }

    std::vector<Ref<Layer>>::reverse_iterator rend() { return m_layers.rend(); }

    std::vector<Ref<Layer>>::const_iterator begin() const {
        return m_layers.begin();
    }

    std::vector<Ref<Layer>>::const_iterator end() const {
        return m_layers.end();
    }

    std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const {
        return m_layers.rbegin();
    }

    std::vector<Ref<Layer>>::const_reverse_iterator rend() const {
        return m_layers.rend();
    }

   private:
    std::vector<Ref<Layer>> m_layers;
    std::size_t m_layerInsertId;
};

}  // namespace te

#endif /* LAYER_STACK_HPP */
