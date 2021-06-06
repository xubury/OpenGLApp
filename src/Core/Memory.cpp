#include <Core/Memory.hpp>

namespace te {

std::size_t VPool::size() const { return m_memory.size(); }

void VPool::resize(std::size_t size) { m_memory.resize(size, nullptr); }

bool VPool::isSet(std::size_t index) const {
    return m_memory.at(index) != nullptr;
}

}  // namespace te
