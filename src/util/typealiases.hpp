#pragma once
#include <vector>
#include <memory>
#include <unordered_map>

namespace ECS {
    template<typename T>
    using Vec_t             = std::vector<T>;
    using EntityID_t        = std::size_t;
    using ComponentID_t     = std::size_t;
    using ComponentTypeID_t = std::size_t;
    template<typename KEY_, typename VALUE_t>
    using Hash_t            = std::unordered_map<KEY_, VALUE_t>;
    template<typename T>
    using UPtr              = std::unique_ptr<T>;
} // namespace ECS 