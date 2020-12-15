#pragma once
#include <ecs/util/typealiases.hpp>
#include <game/cmp/input.hpp>
#include <game/sys/artificialinteligence.hpp>

template<typename GameCTX_t>
struct PysicsSystem_t {
    explicit PysicsSystem_t(ArtificialInteligenceSystem_t<GameCTX_t>& ai) : 
    AI_system{ai} 
    {}

    bool update(GameCTX_t& g) const;
    void initCSV(std::string fname);
private:
    ArtificialInteligenceSystem_t<GameCTX_t>& AI_system {nullptr};
};
