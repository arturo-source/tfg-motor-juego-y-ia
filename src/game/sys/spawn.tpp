#include <game/sys/spawn.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/spawner.hpp>

template<typename GameCTX_t>
void SpawnSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    using clk = std::chrono::steady_clock;
    
    auto now = clk::now();
    for(auto& spw: g.template getComponents<SpawnerComponent_t>()) {
        // auto* phy = getRequieredComponent(g, spw);
        auto* phy = g.template getRequiredComponent<PhysicsComponent_t>(spw);
        if(!phy) continue;

        auto passed { now - spw.last_spawned_time };
        if( spw.to_be_spawned > 0 && passed > spw.spawn_interval ) {
            // spawnDeVerdad(g, phy->x, phy->y);
            spw.spawnerMethod(spw);
            spw.last_spawned_time = now;
            --spw.to_be_spawned;
        }
    }
}
