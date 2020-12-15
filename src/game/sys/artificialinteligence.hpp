#pragma once
#include <ecs/util/typealiases.hpp>
#include <game/cmp/inteligence.hpp>
#include <game/cmp/physics.hpp>
#include <game/cmp/input.hpp>
#include <game/cmp/collider.hpp>

struct CurrentPhysics_t {
    uint8_t side;
    float x, y;
    float vx, vy;
    float aceleration;
};
struct KeysPressed_t {
    uint8_t player_side;
    bool    up;
    bool    down;
};

//Forward declaration
namespace ECS {
    struct Keyboard_t;
}

template<typename GameCTX_t>
struct ArtificialInteligenceSystem_t {
    explicit ArtificialInteligenceSystem_t() = default;

    void update(GameCTX_t& g) const;
    void train(std::string keysFile, std::string physicsFile);

    std::string keysfilename;
    std::string physicsfilename;

    void dumpCSV(const InputComponent_t& inp, ECS::Keyboard_t& ms_Keyboard) const;
    void dumpCSV(const PhysicsComponent_t& phy, const uint8_t side) const;

private:
    PhysicsComponent_t CurrentPhysics2PhysicsComponent(const CurrentPhysics_t& cp) const {
        PhysicsComponent_t phy {0};
        phy.aceleration = cp.aceleration;
        phy.x = cp.x; phy.y = cp.y;
        phy.vx = cp.vx; phy.vy = cp.vy;
        return phy;
    };
    constexpr void propagation(InteligenceComponent_t &ic, const PhysicsComponent_t& playerPhysics, const PhysicsComponent_t& ballPhysics) const;
    constexpr void updateWeights(InteligenceComponent_t &ic, float alpha, KeysPressed_t& kp) const;

    void dumpWeights(InteligenceComponent_t& ic) const;
    void readKeysCSV(std::string& filename);
    void readPhysicsCSV(std::string& filename);

    std::vector<KeysPressed_t>  inputKeys {};
    std::vector<CurrentPhysics_t> physics {};
};