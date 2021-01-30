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
        constexpr uint32_t leftTeamColor   { 0xFF81c784 };
        constexpr uint32_t leftTeamColor2  { 0xFF005b4f };
        constexpr uint32_t rightTeamColor  { 0xFF56c8d8 };
        constexpr uint32_t rightTeamColor2 { 0xFF006978 };
        
        ECS::Entity_t& Lplayer = GOFact.createPaletteAI(          10, kSCRHEIGHT/2, InputComponent_t::S_Left , leftTeamColor);
        ECS::Entity_t& Rplayer = GOFact.createPalette(kSCRWIDTH - 10, kSCRHEIGHT/2, InputComponent_t::S_Right, rightTeamColor);
        ECS::Entity_t& Lball   = GOFact.createBall(            200, kSCRHEIGHT/2, leftTeamColor2);
        ECS::Entity_t& Rball   = GOFact.createBall(kSCRWIDTH - 200, kSCRHEIGHT/2, rightTeamColor2);
        ECS::Entity_t& Lminion = GOFact.createMinionAI(kSCRWIDTH/2 - 100,              30, InputComponent_t::S_Right | InputComponent_t::S_Center, rightTeamColor);
        ECS::Entity_t& Rminion = GOFact.createMinionAI(kSCRWIDTH/2 + 100, kSCRHEIGHT - 30, InputComponent_t::S_Left  | InputComponent_t::S_Center, leftTeamColor);

        GOFact.createWalls(kSCRWIDTH, kSCRHEIGHT, 5, leftTeamColor2, rightTeamColor2);
        GOFact.createMiddleLine(kSCRWIDTH, kSCRHEIGHT);

        filename   = findFilename();
        Lballphy   = Lball.getComponent<PhysicsComponent_t>();
        Rballphy   = Rball.getComponent<PhysicsComponent_t>();
        Lplayerphy = Lplayer.getComponent<PhysicsComponent_t>();
        Rplayerphy = Rplayer.getComponent<PhysicsComponent_t>();
        Lminionphy = Lminion.getComponent<PhysicsComponent_t>();
        Rminionphy = Rminion.getComponent<PhysicsComponent_t>();
        input_ptr  = Lplayer.getComponent<InputComponent_t>();
        ArtificialInteligence.ballPhysics = Lballphy;

        reset();
    };

    void dumpCSV() const {
        // std::ofstream file(filename.c_str(), std::ios::app);
        // if(!file) throw std::runtime_error("Can't open data CSV file for write\n");
        // if(!ball_ptr || !player_ptr || !input_ptr) throw std::runtime_error("Missing player or ball pointer"); 
        
        // file << player_ptr->y << ";" << player_ptr->vy << ";" << player_ptr->aceleration << ";";
        // file << ball_ptr->x << ";" << ball_ptr->y << ";" << ball_ptr->vx << ";" << ball_ptr->vy << ";";
        // file << Input.getKeyboard().isKeyPressed(input_ptr->key_UP) << ";" << Input.getKeyboard().isKeyPressed(input_ptr->key_DOWN) << "\n";
    
        // file.close();
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

        m_playing = !Input.isKeyPressed(ECS::Esc) && !ScoreboardSystem_t<ECS::EntityManager_t>::end_game;
        if(ScoreboardSystem_t<ECS::EntityManager_t>::scored && m_playing) {
            ScoreboardSystem_t<ECS::EntityManager_t>::scored = false;
            reset();
        }
    }

    void reset() {
        const uint32_t avgheight { kSCRHEIGHT/2 };
        Lballphy->vx = -3; Lballphy->vy = 0; Lballphy->x =             200; Lballphy->y = avgheight; Lballphy->aceleration = 0;
        Rballphy->vx =  3; Rballphy->vy = 0; Rballphy->x = kSCRWIDTH - 200; Rballphy->y = avgheight; Rballphy->aceleration = 0;
        Lplayerphy->vx = 0; Lplayerphy->vy = 0; Lplayerphy->y = avgheight; Lplayerphy->aceleration = 0;
        Rplayerphy->vx = 0; Rplayerphy->vy = 0; Rplayerphy->y = avgheight; Rplayerphy->aceleration = 0;
        Lminionphy->vx = 0; Lminionphy->vy = 0; Lminionphy->y = avgheight; Lminionphy->aceleration = 0;
        Rminionphy->vx = 0; Rminionphy->vy = 0; Rminionphy->y = avgheight; Rminionphy->aceleration = 0;
        Render.countdown(EntityMan);
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
    PhysicsComponent_t* Lballphy   {nullptr};
    PhysicsComponent_t* Rballphy   {nullptr};
    PhysicsComponent_t* Lplayerphy {nullptr};
    PhysicsComponent_t* Rplayerphy {nullptr};
    PhysicsComponent_t* Lminionphy {nullptr};
    PhysicsComponent_t* Rminionphy {nullptr};
    InputComponent_t*   input_ptr  {nullptr};
    std::string         filename;
    bool                against_ai { true };

    ECS::EntityManager_t EntityMan;
    GameObjectFactory_t GOFact { EntityMan };
    StateManager_t& SM;

    bool m_playing { true };
};