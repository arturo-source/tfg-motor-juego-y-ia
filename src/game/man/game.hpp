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
#include <game/sys/artificialinteligence.tpp>
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
    explicit GameManager_t(StateManager_t& sm, bool ai) 
    : SM{sm}, keyboard{Input.getKeyboard()}
    {
        against_ai = ai;
        ECS::Entity_t* player = &GOFact.createPalette(kSCRWIDTH - 10, kSCRHEIGHT/2, InputComponent_t::S_Right);
        ECS::Entity_t& ball = GOFact.createBall(kSCRWIDTH/2, kSCRHEIGHT/2);

        if(against_ai) GOFact.createPaletteAI(10, kSCRHEIGHT/2, InputComponent_t::S_Left);
        else player = &GOFact.createPalette(10, kSCRHEIGHT/2, InputComponent_t::S_Left);

        filename   = findFilename();
        ball_ptr   = ball.getComponent<PhysicsComponent_t>();
        player_ptr = player->getComponent<PhysicsComponent_t>();
        input_ptr  = player->getComponent<InputComponent_t>();

    };

    void dumpCSV() const {
        std::ofstream file(filename.c_str(), std::ios::app);
        if(!file) throw std::runtime_error("Can't open data CSV file for write\n");
        if(!ball_ptr || !player_ptr || !input_ptr) throw std::runtime_error("Missing player or ball pointer"); 
        
        file << player_ptr->y << ";" << player_ptr->vy << ";" << player_ptr->aceleration << ";";
        file << ball_ptr->x << ";" << ball_ptr->y << ";" << ball_ptr->vx << ";" << ball_ptr->vy << ";";
        file << keyboard.isKeyPressed(input_ptr->key_UP) << ";" << keyboard.isKeyPressed(input_ptr->key_DOWN) << "\n";
    
        file.close();
    }

    void update() final {
        // Main Loop
        GameTimer_t timer;
        timer.timedCall("REN", [&](){ Render.update(EntityMan); } );
        timer.timedCall("AIN", [&](){ ArtificialInteligence.update(EntityMan); } );
        timer.timedCall("INP", [&](){ Input.update(EntityMan); } );
        timer.timedCall("PHY", [&](){ Physics.update(EntityMan); } );
        timer.timedCall("COL", [&](){ Collision.update(EntityMan); } );
        // timer.timedCall("HEA", [&](){ Health.update(EntityMan); } );
        timer.timedCall("SCO", [&](){ Score.update(EntityMan); } );
        if(!against_ai) timer.timedCall("CSV", [&](){ dumpCSV(); } );
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";

        m_playing = !Input.isEscPressed();
        if(Input.isPausePressed())
            SM.pushState<PauseState_t>();

        if(ScoreComponent_t::scored) {
            ScoreComponent_t::scored = false;
            Score.show(EntityMan);
        }
    }

    bool alive() final { return m_playing; }

    std::string findFilename() const {
        std::stringstream filename;
        int num = 0;
        std::ifstream fi;
        bool fi_is_open = true; 

        // Stop when data%s%d.bin doesnt exist
        while(fi_is_open) {
            filename.str("");
            filename << "CSVs/data" << num << ".csv";
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
    PysicsSystem_t<ECS::EntityManager_t> Physics;
    InputSystem_t<ECS::EntityManager_t> Input;
    CollisionSystem_t<ECS::EntityManager_t> Collision{kSCRWIDTH, kSCRHEIGHT};
    ArtificialInteligenceSystem_t<ECS::EntityManager_t> ArtificialInteligence;
    inline static ScoreboardSystem_t<ECS::EntityManager_t> Score {kSCRWIDTH};
    // const HealthSystem_t<ECS::EntityManager_t> Health {};

    //Game references to dump to AI
    PhysicsComponent_t* ball_ptr   {nullptr};
    PhysicsComponent_t* player_ptr {nullptr};
    InputComponent_t*   input_ptr  {nullptr};
    ECS::Keyboard_t&    keyboard;
    std::string         filename;
    bool                against_ai;

    ECS::EntityManager_t EntityMan;
    GameObjectFactory_t GOFact { EntityMan };
    StateManager_t& SM;

    bool m_playing { true };
};