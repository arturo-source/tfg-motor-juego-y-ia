#pragma once
#include <ecs/util/typealiases.hpp>


template<typename GameCTX_t>
struct ArtificialInteligenceSystem_t {
    explicit ArtificialInteligenceSystem_t();

    bool update(GameCTX_t& g) const;
};