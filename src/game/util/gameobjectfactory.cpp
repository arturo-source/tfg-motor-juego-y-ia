#include <game/util/gameobjectfactory.hpp>
#include <game/cmp/neuralnetwork.hpp>
#include <game/cmp/collider.hpp>
#include <game/cmp/input.hpp>
#include <game/cmp/render.hpp>
#include <game/cmp/spawner.hpp>
#include <game/cmp/health.hpp>
#include <game/cmp/score.hpp>
#include <game/cmp/weapon.hpp>


ECS::Entity_t& GameObjectFactory_t::createEntity(uint32_t w, uint32_t h, uint32_t color) const {
    auto& e = m_EntMan.createEntity();
    auto& rn = m_EntMan.addComponent<RenderComponent_t>(e);
    auto& ph = m_EntMan.addComponent<PhysicsComponent_t>(e);
    auto& cl = m_EntMan.addComponent<ColliderComponent_t>(e);
    rn.loadFromWH(w, h, color);
    // ph.x  = x; ph.y  = y;
    // ph.vx = 0; ph.vy = 0;
    cl.box.xLeft  = 0;
    cl.box.xRight = rn.w;
    cl.box.yUp    = 0;
    cl.box.yDown  = rn.h;
    cl.color = color;

    return e;
}

void GameObjectFactory_t::addInteligence(ECS::Entity_t& entity, const char* filename) const {
    auto& nn = m_EntMan.addComponent<NeuralNetwork_t>(entity);
    nn.setNeurons(filename);
}


ECS::Entity_t& GameObjectFactory_t::createPalette(uint8_t side, uint32_t color) const {
    auto& e = createPalette(paletteW, paletteH, color, side);
    auto& sco = m_EntMan.addComponent<ScoreComponent_t>(e);
    auto& weap = m_EntMan.addComponent<WeaponComponent_t>(e);

    auto& inp = m_EntMan.addComponent<InputComponent_t>(e);
    if(side & InputComponent_t::S_Right) {
        inp.key_UP    = ECS::o;
        inp.key_DOWN  = ECS::l;
        inp.key_shoot = ECS::Intro;
    }
    inp.side = side;

    return e;
}

ECS::Entity_t& GameObjectFactory_t::createMinion(uint8_t side, uint32_t color) const {
    constexpr uint32_t w { paletteW }, h { paletteH/2 };
    auto& e = createPalette(w, h, color, side);
    auto* phy = e.getComponent<PhysicsComponent_t>();
    if(phy) {
        if(side & InputComponent_t::S_Right) phy->x -= w/2;
        if(side & InputComponent_t::S_Left)  phy->x += w/2;
    }
    auto& inp = m_EntMan.addComponent<InputComponent_t>(e);
    if(side & InputComponent_t::S_Right) {
        inp.key_UP    = 0;
        inp.key_DOWN  = 0;
        inp.key_shoot = 0;
    }
    inp.side = side;
    addInteligence(e, "weights_CSVs/weights.csv");

    return e;
}

ECS::Entity_t& GameObjectFactory_t::createPalette(uint32_t w, uint32_t h, uint32_t color, uint8_t side) const {
    auto& e = createEntity(w, h, color);
    auto* phy = e.getComponent<PhysicsComponent_t>();
    if(phy) {
        phy->friction = 0.97;
    }
    auto* col = e.getComponent<ColliderComponent_t>();
    if(col) {
        col->properties = ColliderComponent_t::P_IsPlayer | ColliderComponent_t::P_Bounces;
    }
    
    return e;
}

ECS::Entity_t& GameObjectFactory_t::createBullet(const PhysicsComponent_t& palette_phy, uint8_t side) const {
    constexpr uint32_t w { 15 }, h { 5 };
    constexpr float vx { 5 };
    constexpr uint32_t color { 0xFFf44336 };
    
    auto& e = createEntity(w, h, color);
    auto* phy = e.getComponent<PhysicsComponent_t>();
    if(phy) {
        phy->x = palette_phy.x;
        phy->y = palette_phy.y + paletteH/2;
        if( side & InputComponent_t::S_Left ) {
            phy->vx = vx;
            phy->x += paletteW;
        }
        if( side & InputComponent_t::S_Right ) {
            phy->vx = -vx;
            phy->x -= w;
        }
    }
    auto* col = e.getComponent<ColliderComponent_t>();
    if(col) {
        col->properties = ColliderComponent_t::P_IsBullet;
    }
    return e;
}

ECS::Entity_t& GameObjectFactory_t::createBall(const uint32_t color) const {
    constexpr uint32_t w { 10 }, h { 10 };
    auto& e = createEntity(w, h, color);

    auto* phy = e.getComponent<PhysicsComponent_t>();
    if(phy) {
        phy->vx = 3;
    }
    auto* col = e.getComponent<ColliderComponent_t>();
    if(col) {
        col->properties = ColliderComponent_t::P_IsBall | ColliderComponent_t::P_Bounces;
    }

    return e;
}

void GameObjectFactory_t::createWalls(const uint32_t scrWidth, const uint32_t scrHeight, const uint32_t columns, const uint32_t leftTeamColor, const uint32_t rightTeamColor) const {
    constexpr uint32_t wallWidth { 10 }, wallHeight { 50 };
    const uint32_t wallsPerColumn { scrHeight / (wallHeight+1) };
    const uint32_t middleScrWidth { scrWidth/2 };

    for(uint32_t i = 0; i < columns; ++i) {
        uint32_t x { middleScrWidth + (i+1) * (wallWidth+3)};
        createWallsColumn(x, wallsPerColumn, wallWidth, wallHeight, leftTeamColor);
    }
    for(uint32_t i = 0; i < columns; ++i) {
        uint32_t x { middleScrWidth - (i+2) * (wallWidth+3) };
        createWallsColumn(x, wallsPerColumn, wallWidth, wallHeight, rightTeamColor);
    }
}

void GameObjectFactory_t::createWallsColumn(const uint32_t x, const uint32_t wallsPerColumn, const uint32_t wallWidth, const uint32_t wallHeight, const uint32_t color) const {
    for(uint32_t j = 0; j < wallsPerColumn; ++j) {
        uint32_t y { 10 + j * (wallHeight+3) };
        auto& e = createEntity(wallWidth, wallHeight, color);
        
        auto* col = e.getComponent<ColliderComponent_t>();
        if(col) col->properties = ColliderComponent_t::P_IsWall | ColliderComponent_t::P_Bounces;
        
        auto* phy = e.getComponent<PhysicsComponent_t>();
        if(phy) {
            phy->x = x;
            phy->y = y;
        }
    }
}

void GameObjectFactory_t::createMiddleLine(const uint32_t scrWidth, const uint32_t scrHeight) const {
    auto& e = m_EntMan.createEntity();
    auto& rn = m_EntMan.addComponent<RenderComponent_t>(e);
    auto& ph = m_EntMan.addComponent<PhysicsComponent_t>(e);
    ph.x = scrWidth/2 - 1;
    ph.y = 0;
    rn.loadFromWH(2, scrHeight, 0xff002233);
}