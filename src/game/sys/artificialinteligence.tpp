#include <game/sys/artificialinteligence.hpp>

template<typename GameCTX_t>
void ArtificialInteligenceSystem_t<GameCTX_t>::update(GameCTX_t& g) const {
    PhysicsComponent_t* ballPhy { nullptr };
    for(auto& col: g.template getComponents<ColliderComponent_t>()) {
        if(col.properties & ColliderComponent_t::P_IsBall) {
            ballPhy = g.template getRequiredComponent<PhysicsComponent_t>(col);
            break;
        }
    }
    if(!ballPhy) throw std::runtime_error("Ball physics missing in game.");

    for(auto& ic: g.template getComponents<InteligenceComponent_t>()) {
        auto* phy = g.template getRequiredComponent<PhysicsComponent_t>(ic);
        if(!phy) continue;

        propagation(ic, *phy, *ballPhy);

        ic.keyUP_pressed = false;
        ic.keyDOWN_pressed = false;
        
        if(ic.output == 1)
            ic.keyUP_pressed   = true;
        if(ic.output == 0)
            ic.keyDOWN_pressed = true;
    }
}

template<typename GameCTX_t>
void ArtificialInteligenceSystem_t<GameCTX_t>::train(std::string keysFile, std::string physicsFile) {
    InteligenceComponent_t ic { 0 };
    readKeysCSV(keysFile);
    readPhysicsCSV(physicsFile);
    for(uint32_t i = 0; i < inputKeys.size()/2; i++) {

        KeysPressed_t* leftPlayerInput { nullptr };
        KeysPressed_t* rightPlayerInput { nullptr };
        for (uint32_t j = 0; j < 2; j++) { //Locate left and right player
            if(inputKeys[i+j].player_side & InputComponent_t::S_Left) {
                leftPlayerInput = &inputKeys[i+j];
            }
            if(inputKeys[i+j].player_side & InputComponent_t::S_Right) {
                rightPlayerInput = &inputKeys[i+j];
            }
        }
        if(!leftPlayerInput || !rightPlayerInput) throw std::runtime_error("Some input missing on CSV.");

        CurrentPhysics_t* leftPlayerPhysics { nullptr };
        CurrentPhysics_t* rightPlayerPhysics { nullptr };
        CurrentPhysics_t* ballPhysics { nullptr };
        for (uint32_t j = 0; j < 3; j++) { //Locate ball and left and right player
            if (physics[i+j].side & InputComponent_t::S_Left) {
                leftPlayerPhysics = &physics[i+j];
            } else if (physics[i+j].side & InputComponent_t::S_Right) {
                rightPlayerPhysics = &physics[i+j];
            } else if(physics[i+j].side == InputComponent_t::S_NoSide) {
                ballPhysics = &physics[i+j];                 
            }
        }
        if(!ballPhysics || !leftPlayerPhysics || !rightPlayerPhysics) throw std::runtime_error("Some phisic missing on CSV.");

        propagation(ic, CurrentPhysics2PhysicsComponent(*leftPlayerPhysics), CurrentPhysics2PhysicsComponent(*ballPhysics));
        updateWeights(ic, 0.1, *leftPlayerInput);
        propagation(ic, CurrentPhysics2PhysicsComponent(*rightPlayerPhysics), CurrentPhysics2PhysicsComponent(*ballPhysics));
        updateWeights(ic, 0.1, *rightPlayerInput);
    }
    dumpWeights(ic);
}

template<typename GameCTX_t>
void ArtificialInteligenceSystem_t<GameCTX_t>::dumpWeights(InteligenceComponent_t& ic) const {
    std::ofstream file(ic.filename, std::ios::trunc);
    if(!file) throw std::runtime_error("Can't open weights CSV file for write\n");
    
    std::string separator = "";
    for(float weight : ic.weights) {
        file << separator << weight;
        separator = ";";
    }
    file.close();
}

template<typename GameCTX_t>
void ArtificialInteligenceSystem_t<GameCTX_t>::readPhysicsCSV(std::string& filename) {
    std::ifstream file(filename.c_str());
    if(!file) throw std::runtime_error("Can't open physics CSV file for read\n");
    
    std::string line;
    std::getline(file,line); //Erase the first line of csv
    std::cout << line;
    while(std::getline(file,line)) {
        std::stringstream lineStream(line);
        std::string player_ID;
        std::string x, y;
        std::string vx, vy;
        std::string aceleration;
        std::getline(lineStream, player_ID, ';');
        std::getline(lineStream, x, ';');
        std::getline(lineStream, y, ';');
        std::getline(lineStream, vx, ';');
        std::getline(lineStream, vy, ';');
        std::getline(lineStream, aceleration, ';');

        CurrentPhysics_t cp {
            static_cast<uint8_t>(atoi(player_ID.c_str())),
            static_cast<float>(atof(x.c_str())),
            static_cast<float>(atof(y.c_str())),
            static_cast<float>(atof(vx.c_str())),
            static_cast<float>(atof(vy.c_str())),
            static_cast<float>(atof(aceleration.c_str()))
        };
        physics.push_back(cp);
    }
    file.close();
}

template<typename GameCTX_t>
void ArtificialInteligenceSystem_t<GameCTX_t>::readKeysCSV(std::string& filename) {
    std::ifstream file(filename.c_str());
    if(!file) throw std::runtime_error("Can't open keys CSV file for read\n");
    
    std::string line;
    std::getline(file,line); //Erase the first line of csv
    while(std::getline(file,line)) {
        std::stringstream lineStream(line);
        std::string side;
        std::string kUP;
        std::string kDOWN;
        std::getline(lineStream,side, ';');
        std::getline(lineStream,kUP, ';');
        std::getline(lineStream,kDOWN, ';');

        KeysPressed_t kp {
            static_cast<uint8_t>( atoi(side.c_str()) ),
            static_cast<bool>( atoi(kUP.c_str()) ),
            static_cast<bool>( atoi(kDOWN.c_str()) )
        };
        inputKeys.push_back(kp);
    }
    file.close();
}

template<typename GameCTX_t>
constexpr void ArtificialInteligenceSystem_t<GameCTX_t>::propagation(InteligenceComponent_t &ic, const PhysicsComponent_t& playerPhysics, const PhysicsComponent_t& ballPhysics) const {
    std::array<float, 10> dataInputs {
        playerPhysics.x, playerPhysics.y, playerPhysics.vx, playerPhysics.vy, playerPhysics.aceleration,
        ballPhysics.x, ballPhysics.y, ballPhysics.vx, ballPhysics.vy, ballPhysics.aceleration
    };

    ic.calculateOutput(dataInputs);
}

template<typename GameCTX_t>
constexpr void ArtificialInteligenceSystem_t<GameCTX_t>::updateWeights(InteligenceComponent_t &ic, float alpha, KeysPressed_t& kp) const {
    int8_t desiredOutput { 0 };
    if(kp.up) 
        desiredOutput = 1;
    if(kp.down)
        desiredOutput = 0;

    for(int32_t i = 0; i < ic.weights.size(); i++)
        ic.weights[i] = ic.weights[i] + alpha*(desiredOutput-ic.output) * ic.inputs[i];
}

template<typename GameCTX_t>
void ArtificialInteligenceSystem_t<GameCTX_t>::dumpCSV(const PhysicsComponent_t& phy, const uint8_t side) const {
    std::ofstream file(physicsfilename.c_str(), std::ios::app);
    file << static_cast<int>(side) << ";" << phy.x << ";" << phy.y << ";" << phy.vx << ";" << phy.vy << ";" << phy.aceleration << "\n";
    file.close();
}

template<typename GameCTX_t>
void ArtificialInteligenceSystem_t<GameCTX_t>::dumpCSV(const InputComponent_t& inp, ECS::Keyboard_t& ms_Keyboard) const {
    std::ofstream file(keysfilename.c_str(), std::ios::app);
    file << static_cast<int>(inp.side) << ";" << ms_Keyboard.isKeyPressed(inp.key_UP) << ";" << ms_Keyboard.isKeyPressed(inp.key_DOWN) << "\n";
    file.close();
}