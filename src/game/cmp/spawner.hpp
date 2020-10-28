#pragma once
#include <cstdint>
#include <chrono>
#include <functional>
#include <ecs/cmp/component.hpp>

using namespace std::chrono_literals; //FEO!!!

struct SpawnerComponent_t : public ECS::ComponentBase_t<SpawnerComponent_t> {
    using clk = std::chrono::steady_clock;

    explicit SpawnerComponent_t(ECS::EntityID_t eid)
    : ComponentBase_t(eid)
    {}
    
    std::function<void(const SpawnerComponent_t&)> spawnerMethod {};
    clk::time_point last_spawned_time { clk::now() };
    std::chrono::duration<double> spawn_interval { 3s };
    std::size_t to_be_spawned { 2 };
};
