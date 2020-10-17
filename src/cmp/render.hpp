#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <picoPNG/src/picopng.hpp>
#include <fstream>
#include <iostream>
#include <cstring>
#include <cmp/component.hpp>
#include <util/typealiases.hpp>

namespace ECS {

struct RenderComponent_t : Component_t {
    explicit RenderComponent_t(EntityID_t eid)
    : Component_t(eid)
    {}

    static ComponentTypeID_t getComponentTypeID() noexcept {
        return static_cast<ComponentTypeID_t>(3);
    }

    void loadFromFile(const std::string_view filename);
    void initSpriteFromABGRData(std::vector<unsigned char> pixels);

    uint32_t w{0}, h{0};
    std::vector<uint32_t> sprite {};
};
} // namespace ECS
