#include <game/sys/artificialinteligence.hpp>

template<typename GameCTX_t>
void ArtificialInteligenceSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    for(auto& nn: g.template getComponents<NeuralNetwork_t>()) {
        auto* phy = g.template getRequiredComponent<PhysicsComponent_t>(nn);
        if(!phy) throw std::runtime_error("The neural network does not have phisics.");
        auto* inp = g.template getRequiredComponent<InputComponent_t>(nn);
        if(!inp) throw std::runtime_error("The neural network does not have input.");

        bool keyUP_pressed = false,  keyDOWN_pressed = false;
        keysPressed(nn, *phy, *inp, keyUP_pressed, keyDOWN_pressed);

        if( keyDOWN_pressed && !keyUP_pressed) phy->aceleration =  0.44;
        if(!keyDOWN_pressed &&  keyUP_pressed) phy->aceleration = -0.44;
    }
}

template<typename GameCTX_t>
constexpr void ArtificialInteligenceSystem_t<GameCTX_t>::keysPressed(NeuralNetwork_t& nn, const PhysicsComponent_t& phy, const InputComponent_t& inp, bool& keyUp, bool& keyDown) const noexcept {
    VecDouble_t input_data {
        phy.x, phy.y, phy.vy
    };
    if(inp.side & InputComponent_t::S_Left) {
        input_data.push_back(gameReferences.Lball->x);
        input_data.push_back(gameReferences.Lball->y);
        input_data.push_back(gameReferences.Lball->vx);
        input_data.push_back(gameReferences.Lball->vy);
        input_data.push_back(gameReferences.Rball->x);
        input_data.push_back(gameReferences.Rball->y);
        input_data.push_back(gameReferences.Rball->vx);
        input_data.push_back(gameReferences.Rball->vy);
        input_data.push_back(gameReferences.Lminion->x);
        input_data.push_back(gameReferences.Lminion->y);
        input_data.push_back(gameReferences.Lminion->vy);
    } else if(inp.side & InputComponent_t::S_Right) {
        input_data.push_back(gameReferences.Rball->x);
        input_data.push_back(gameReferences.Rball->y);
        input_data.push_back(gameReferences.Rball->vx);
        input_data.push_back(gameReferences.Rball->vy);
        input_data.push_back(gameReferences.Lball->x);
        input_data.push_back(gameReferences.Lball->y);
        input_data.push_back(gameReferences.Lball->vx);
        input_data.push_back(gameReferences.Lball->vy);
        input_data.push_back(gameReferences.Rminion->x);
        input_data.push_back(gameReferences.Rminion->y);
        input_data.push_back(gameReferences.Rminion->vy);
    }
    VecDouble_t result = nn.feedforward(input_data);
    keyUp   = result[0] > 0.5;
    keyDown = result[1] > 0.5;
}