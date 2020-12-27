#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include <picoPNG/src/picopng.hpp>
#include <fstream>
#include <cstring>
#include <ecs/cmp/component.hpp>
#include <ecs/util/typealiases.hpp>

struct RenderComponent_t : ECS::ComponentBase_t<RenderComponent_t> {
    explicit RenderComponent_t(ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {}

    void loadFromFile(const std::string_view filename);
    void initSpriteFromABGRData(std::vector<unsigned char> pixels);
    void loadFromWH(uint32_t dw, uint32_t dh, uint32_t color);

    uint32_t w{0}, h{0};
    std::vector<uint32_t> sprite {};
};
