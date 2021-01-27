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
#include <game/util/timer.hpp>
#include <game/man/state.hpp>
#include <game/man/menu.hpp>
#include <ecs/man/entitymanager.tpp>
#include <ecs/util/keys.hpp>

struct GameManager_t : StateBase_t {
    explicit GameManager_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const uint32_t scrW, const uint32_t scrH) 
    : SM{sm}, Render{ren}, Input{inp}, kSCRWIDTH{scrW}, kSCRHEIGHT{scrH}
    {
        Input.setObjectFactory(GOFact);
        constexpr uint32_t leftTeamColor  { 0xFF81c784 };
        constexpr uint32_t rightTeamColor { 0xFF56c8d8 };
        
        ECS::Entity_t* player = &GOFact.createPalette(kSCRWIDTH - 10, kSCRHEIGHT/2, InputComponent_t::S_Right, rightTeamColor);
        ECS::Entity_t& Lball = GOFact.createBall(            100, kSCRHEIGHT/2);
        ECS::Entity_t& Rball = GOFact.createBall(kSCRWIDTH - 100, kSCRHEIGHT/2);
        ECS::Entity_t& Lminion = GOFact.createMinionAI(kSCRWIDTH/2 - 100,              30, InputComponent_t::S_Right | InputComponent_t::S_Center, rightTeamColor);
        ECS::Entity_t& Rminion = GOFact.createMinionAI(kSCRWIDTH/2 + 100, kSCRHEIGHT - 30, InputComponent_t::S_Left  | InputComponent_t::S_Center, leftTeamColor);

        if(against_ai) GOFact.createPaletteAI(10, kSCRHEIGHT/2, InputComponent_t::S_Left, leftTeamColor);
        else player = &GOFact.createPalette(10, kSCRHEIGHT/2, InputComponent_t::S_Left, leftTeamColor);

        GOFact.createWalls(kSCRWIDTH, kSCRHEIGHT, 5);
        GOFact.createMiddleLine(kSCRWIDTH, kSCRHEIGHT);

        filename   = findFilename();
        ball_ptr   = Lball.getComponent<PhysicsComponent_t>();
        player_ptr = player->getComponent<PhysicsComponent_t>();
        input_ptr  = player->getComponent<InputComponent_t>();
        ArtificialInteligence.ballPhysics = ball_ptr;
    };

    void dumpCSV() const {
        std::ofstream file(filename.c_str(), std::ios::app);
        if(!file) throw std::runtime_error("Can't open data CSV file for write\n");
        if(!ball_ptr || !player_ptr || !input_ptr) throw std::runtime_error("Missing player or ball pointer"); 
        
        file << player_ptr->y << ";" << player_ptr->vy << ";" << player_ptr->aceleration << ";";
        file << ball_ptr->x << ";" << ball_ptr->y << ";" << ball_ptr->vx << ";" << ball_ptr->vy << ";";
        file << Input.getKeyboard().isKeyPressed(input_ptr->key_UP) << ";" << Input.getKeyboard().isKeyPressed(input_ptr->key_DOWN) << "\n";
    
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
        timer.timedCall("SCO", [&](){ Score.update(EntityMan); } );
        timer.timedCall("WEA", [&](){ Weapons.update(EntityMan); } );
        if(!against_ai) timer.timedCall("CSV", [&](){ dumpCSV(); } );
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";

        m_playing = !Input.isKeyPressed(ECS::Esc);
    }

    bool alive() final { return m_playing; }

    std::string findFilename() const {
        std::stringstream filename;
        int num = 0;
        std::ifstream fi;
        bool fi_is_open = true; 

        // Stop when data%d.csv doesnt exist
        while(fi_is_open) {
            filename.str("");
            filename << "CSVs/data" << num << ".csv";
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
    const uint32_t kSCRWIDTH;
    const uint32_t kSCRHEIGHT;
    static constexpr uint64_t FPS  { 60 };
    static constexpr uint64_t NSPF { 1000000000/FPS };

    // Systems
    const RenderSystem_t<ECS::EntityManager_t>& Render;
    PysicsSystem_t<ECS::EntityManager_t> Physics;
    InputSystem_t<ECS::EntityManager_t>& Input;
    CollisionSystem_t<ECS::EntityManager_t> Collision{kSCRWIDTH, kSCRHEIGHT};
    ArtificialInteligenceSystem_t<ECS::EntityManager_t> ArtificialInteligence;
    ScoreboardSystem_t<ECS::EntityManager_t> Score {kSCRWIDTH};
    WeaponSystem_t<ECS::EntityManager_t> Weapons;

    //Game references to dump to AI
    PhysicsComponent_t* ball_ptr   {nullptr};
    PhysicsComponent_t* player_ptr {nullptr};
    InputComponent_t*   input_ptr  {nullptr};
    std::string         filename;
    bool                against_ai { true };

    ECS::EntityManager_t EntityMan;
    GameObjectFactory_t GOFact { EntityMan };
    StateManager_t& SM;

    bool m_playing { true };
};