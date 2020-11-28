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
#include <game/cmp/collider.hpp>
#include <game/util/gameobjectfactory.hpp>
#include <game/util/timer.hpp>
#include <game/man/state.hpp>
#include <ecs/man/entitymanager.tpp>

struct PauseState_t : StateBase_t {
    explicit PauseState_t() = default;
    void update() final {
        std::cout << R"(
GAME PAUSED
[[ TYPE IN SOMETHING ]])";
        int opt;
        std::cin >> opt;
        std::cout << "Has pulsado " << opt << "\n";
        m_alive = false;
    }
    bool alive() final { return m_alive; }
private:
    bool m_alive { true };
};

struct GameManager_t : StateBase_t {
    explicit GameManager_t(StateManager_t& sm) : SM{sm} {
        Input.initCSV(   findFilename("input", "csv") );
        Physics.initCSV( findFilename("physics", "csv") );
        GOFact.createPalette(10, kSCRHEIGHT/2, InputComponent_t::S_Left);
        GOFact.createPalette(kSCRWIDTH - 10, kSCRHEIGHT/2, InputComponent_t::S_Right);
        GOFact.createBall(kSCRWIDTH/2, kSCRHEIGHT/2);
    };

    void update() final {
        // Main Loop
        GameTimer_t timer;
        timer.timedCall("REN", [&](){ Render.update(EntityMan); } );
        timer.timedCall("INP", [&](){ Input.update(EntityMan); } );
        timer.timedCall("PHY", [&](){ Physics.update(EntityMan); } );
        timer.timedCall("COL", [&](){ Collision.update(EntityMan); } );
        // timer.timedCall("HEA", [&](){ Health.update(EntityMan); } );
        timer.timedCall("SCO", [&](){ Score.update(EntityMan); } );
        // timer.waitUntil_ns(NSPF);
        std::cout << "[EXT]" << timer.waitUntil_ns(NSPF) << "\n";

        m_playing = !Input.isEscPressed();
        if(Input.isPausePressed())
            SM.pushState<PauseState_t>();

        if(ScoreComponent_t::scored) {
            ScoreComponent_t::scored = false;
            Score.show(EntityMan);
        }
    }

    bool alive() final { return m_playing; }

    std::string findFilename(const std::string_view toSystem, const std::string_view type) const {
        std::stringstream filename;
        int num = 0;
        std::ifstream fi;
        bool fi_is_open = true; 

        // Stop when data%s%d.bin doesnt exist
        while(fi_is_open) {
            filename.str("");
            filename << "CSVs/data" << toSystem << num << "." << type;
            // sprintf(filename.data(), "data%s%d.bin", toSystem.data(), num);
            fi.open(filename.str());
            fi_is_open = fi.is_open();
            num++;
            fi.close();
        }

        return filename.str();
    }
    ECS::Keyboard_t& getKeyboard() { return Input.getKeyboard(); }
private:
    //Game consts
    static constexpr uint32_t kSCRWIDTH  {700};
    static constexpr uint32_t kSCRHEIGHT {400};
    static constexpr uint64_t FPS  { 60 };
    static constexpr uint64_t NSPF { 1000000000/FPS };

    // Systems
    const RenderSystem_t<ECS::EntityManager_t> Render{kSCRWIDTH, kSCRHEIGHT};
    PysicsSystem_t<ECS::EntityManager_t> Physics {};
    InputSystem_t<ECS::EntityManager_t> Input {};
    CollisionSystem_t<ECS::EntityManager_t> Collision{kSCRWIDTH, kSCRHEIGHT};
    inline static ScoreboardSystem_t<ECS::EntityManager_t> Score {kSCRWIDTH};
    // const HealthSystem_t<ECS::EntityManager_t> Health {};

    ECS::EntityManager_t EntityMan;
    GameObjectFactory_t GOFact { EntityMan };
    StateManager_t& SM;

    bool m_playing { true };
};