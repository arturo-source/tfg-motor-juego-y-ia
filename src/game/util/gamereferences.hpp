#pragma once
#include <game/cmp/physics.hpp>
#include <game/cmp/input.hpp>
#include <game/sys/input.hpp>
#include <game/util/gameobjectfactory.hpp>
#include <game/util/gameconfig.hpp>

struct GameReferences {
    explicit GameReferences(GameObjectFactory_t& GOFactory, GameConfig& gConfig, const uint32_t scrW, const uint32_t scrH)
    : kSCRWIDTH{scrW}, kSCRHEIGHT{scrH}, GOFact{GOFactory}, gameConfig{gConfig}
    {
        filename = findFilename();
        constexpr uint32_t leftTeamColor   { 0xFF81c784 };
        constexpr uint32_t leftTeamColor2  { 0xFF005b4f };
        constexpr uint32_t rightTeamColor  { 0xFF56c8d8 };
        constexpr uint32_t rightTeamColor2 { 0xFF006978 };
        
        ECS::Entity_t& Lplayer_entity = GOFact.createPalette(InputComponent_t::S_Left , leftTeamColor);
        ECS::Entity_t& Rplayer_entity = GOFact.createPalette(InputComponent_t::S_Right, rightTeamColor);
        ECS::Entity_t& Lball_entity   = GOFact.createBall(leftTeamColor2);
        ECS::Entity_t& Rball_entity   = GOFact.createBall(rightTeamColor2);
        ECS::Entity_t& Lminion_entity = GOFact.createMinion(InputComponent_t::S_Right | InputComponent_t::S_Center, rightTeamColor);
        ECS::Entity_t& Rminion_entity = GOFact.createMinion(InputComponent_t::S_Left  | InputComponent_t::S_Center, leftTeamColor);

        if(gameConfig.Lplayer_AI) GOFact.addInteligence(Lplayer_entity); 
        if(gameConfig.Rplayer_AI) GOFact.addInteligence(Rplayer_entity); 

        GOFact.createWalls(kSCRWIDTH, kSCRHEIGHT, 1, leftTeamColor2, rightTeamColor2);

        Lball   = Lball_entity.getComponent<PhysicsComponent_t>();
        Rball   = Rball_entity.getComponent<PhysicsComponent_t>();
        Lplayer = Lplayer_entity.getComponent<PhysicsComponent_t>();
        Rplayer = Rplayer_entity.getComponent<PhysicsComponent_t>();
        Lminion = Lminion_entity.getComponent<PhysicsComponent_t>();
        Rminion = Rminion_entity.getComponent<PhysicsComponent_t>();
        Linputs = Lplayer_entity.getComponent<InputComponent_t>();
        Rinputs = Rplayer_entity.getComponent<InputComponent_t>();

        resetGame();
    }

    void resetGame() {
        const uint32_t avgheight { kSCRHEIGHT/2 };
        const uint32_t avgwidth { kSCRWIDTH/2 };
        Lball->vx = -3; Lball->vy = 0; Lball->x =             200; Lball->y = avgheight; Lball->aceleration = 0;
        Rball->vx =  3; Rball->vy = 0; Rball->x = kSCRWIDTH - 200; Rball->y = avgheight; Rball->aceleration = 0;
        Lplayer->vx = 0; Lplayer->vy = 0; Lplayer->y = avgheight; Lplayer->x =             10; Lplayer->aceleration = 0;
        Rplayer->vx = 0; Rplayer->vy = 0; Rplayer->y = avgheight; Rplayer->x = kSCRWIDTH - 20; Rplayer->aceleration = 0;
        Lminion->vx = 0; Lminion->vy = 0; Lminion->y = avgheight; Lminion->x = avgwidth - 60; Lminion->aceleration = 0;
        Rminion->vx = 0; Rminion->vy = 0; Rminion->y = avgheight; Rminion->x = avgwidth + 50; Rminion->aceleration = 0;
    }

    void dumpCSV() const {
        std::ofstream file(filename.c_str(), std::ios::app);
        if(!file) throw std::runtime_error("Can't open data CSV file for write\n");
        if( !(Lball && Rball && Lplayer && Rplayer && Lminion && Rminion && Linputs && Rinputs) ) 
                  throw std::runtime_error("Missing player or ball or minion pointer"); 
        
        if(!gameConfig.Lplayer_AI) {
            file << Lplayer->x << ";" << Lplayer->y << ";" << Lplayer->vy << ";" << Lplayer->aceleration << ";";
            file << Lball->x << ";" << Lball->y << ";" << Lball->vx << ";" << Lball->vy << ";";
            file << Rball->x << ";" << Rball->y << ";" << Rball->vx << ";" << Rball->vy << ";";
            file << Lminion->x << ";" << Lminion->y << ";" << Lminion->vy << ";" << Lminion->aceleration << ";";
            file << InputSystem_t<ECS::EntityManager_t>::getKeyboard().isKeyPressed(Linputs->key_UP) << ";" << InputSystem_t<ECS::EntityManager_t>::getKeyboard().isKeyPressed(Linputs->key_DOWN) << "\n";
        }
        if(!gameConfig.Rplayer_AI) {
            file << Rplayer->x << ";" << Rplayer->y << ";" << Rplayer->vy << ";" << Rplayer->aceleration << ";";
            file << Rball->x << ";" << Rball->y << ";" << Rball->vx << ";" << Rball->vy << ";";
            file << Lball->x << ";" << Lball->y << ";" << Lball->vx << ";" << Lball->vy << ";";
            file << Rminion->x << ";" << Rminion->y << ";" << Rminion->vy << ";" << Rminion->aceleration << ";";
            file << InputSystem_t<ECS::EntityManager_t>::getKeyboard().isKeyPressed(Rinputs->key_UP) << ";" << InputSystem_t<ECS::EntityManager_t>::getKeyboard().isKeyPressed(Rinputs->key_DOWN) << "\n";
        }
    
        file.close();
    }

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

    PhysicsComponent_t* Lball   {nullptr};
    PhysicsComponent_t* Rball   {nullptr};
    PhysicsComponent_t* Lplayer {nullptr};
    PhysicsComponent_t* Rplayer {nullptr};
    PhysicsComponent_t* Lminion {nullptr};
    PhysicsComponent_t* Rminion {nullptr};
    InputComponent_t*   Linputs {nullptr};
    InputComponent_t*   Rinputs {nullptr};

private:
    const uint32_t kSCRWIDTH;
    const uint32_t kSCRHEIGHT;
    GameObjectFactory_t& GOFact;
    GameConfig& gameConfig;
    std::string filename;
};
