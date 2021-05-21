#pragma once
#include <cstdint>
#include <memory>
#include <iostream>
#include <sstream>
#include <thread>
#include <game/sys/render.tpp>
#include <game/sys/physics.tpp>
#include <game/sys/input.tpp>
#include <game/sys/collision.tpp>
#include <game/sys/health.tpp>
#include <game/sys/spawn.tpp>
#include <game/sys/scoreboard.tpp>
#include <game/sys/weapon.tpp>
#include <game/sys/artificialinteligence.tpp>
#include <game/cmp/collider.hpp>
#include <game/util/gameobjectfactory.hpp>
#include <game/util/gameconfig.hpp>
#include <game/util/gamereferences.hpp>
#include <game/util/timer.hpp>
#include <game/man/state.hpp>
#include <game/man/menu.hpp>
#include <ecs/man/entitymanager.tpp>
#include <ecs/util/keys.hpp>

struct GameManager_t : StateBase_t {
    explicit GameManager_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, GameConfig& gConfig, const uint8_t gameMode, const uint32_t scrW, const uint32_t scrH) 
    : SM{sm}, Render{ren}, Input{inp}, kSCRWIDTH{scrW}, kSCRHEIGHT{scrH}
    , gameReferences{GOFact, gConfig, gameMode, scrW, scrH}, ArtificialInteligence{gameReferences}
    {
        Input.setObjectFactory(GOFact);
        GOFact.createMiddleLine(kSCRWIDTH, kSCRHEIGHT);
    };

    void update() final {
        // Main Loop
        GameTimer_t timer;
        timer.timedCall("REN", [&](){ Render.update(EntityMan); } );
        timer.timedCall("AIN", [&](){ ArtificialInteligence.update(EntityMan); } );
        timer.timedCall("INP", [&](){ Input.update(EntityMan); } );
        timer.timedCall("PHY", [&](){ Physics.update(EntityMan); } );
        timer.timedCall("COL", [&](){ Collision.update(EntityMan); } );
        timer.timedCall("SCO", [&](){ Score.update(EntityMan); } );
        timer.timedCall("WEA", [&](){ Weapons.update(EntityMan); } );
        timer.timedCall("CSV", [&](){ gameReferences.dumpCSV(); } );
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";

        m_playing = !Input.isKeyPressed(ECS::Esc) && !ScoreboardSystem_t<ECS::EntityManager_t>::end_game;
        if(ScoreboardSystem_t<ECS::EntityManager_t>::scored) {
            ScoreboardSystem_t<ECS::EntityManager_t>::scored = false;
            gameReferences.resetGame();
        }
    }

    bool alive() final { return m_playing && !Render.shouldClose(); }

    ECS::Keyboard_t& getKeyboard() { return Input.getKeyboard(); }
private:
    //Game consts
    const uint32_t kSCRWIDTH;
    const uint32_t kSCRHEIGHT;
    static constexpr uint64_t FPS  { 60 };
    static constexpr uint64_t NSPF { 1000000000/FPS };

    // Systems
    const RenderSystem_t<ECS::EntityManager_t>& Render;
    PysicsSystem_t<ECS::EntityManager_t> Physics;
    InputSystem_t<ECS::EntityManager_t>& Input;
    CollisionSystem_t<ECS::EntityManager_t> Collision{kSCRWIDTH, kSCRHEIGHT};
    ScoreboardSystem_t<ECS::EntityManager_t> Score {kSCRWIDTH};
    WeaponSystem_t<ECS::EntityManager_t> Weapons;

    ECS::EntityManager_t EntityMan;
    GameObjectFactory_t GOFact { EntityMan };
    StateManager_t& SM;
    GameReferences gameReferences;

    ArtificialInteligenceSystem_t<ECS::EntityManager_t> ArtificialInteligence;

    bool m_playing { true };
};