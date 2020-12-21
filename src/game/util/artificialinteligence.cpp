#include <game/util/artificialinteligence.hpp>
#include <fstream>
#include <sstream>

void ArtificialInteligence_t::train(const std::string& filename) {
    readCSV(filename);
    train(Up);
    train(Down);
    dumpWeights();
}
void ArtificialInteligence_t::train(int8_t key) {
    if(key & Up) {
        for(CurrentMatchState_t& ms: matchStates) {
            float_array given_physics = CurrentMatchState2array(ms);
            int8_t output { calculatedOutput(weightsUp, given_physics) };
            updateWeights(weightsUp, given_physics, output, ms.upPressed);
        }
    } else if(key & Down) {
        for(CurrentMatchState_t& ms: matchStates) {
            float_array given_physics = CurrentMatchState2array(ms);
            int8_t output { calculatedOutput(weightsDown, given_physics) };
            updateWeights(weightsDown, given_physics, output, ms.downPressed);
        }
    }
}

constexpr ArtificialInteligence_t::float_array ArtificialInteligence_t::CurrentMatchState2array(CurrentMatchState_t& ms) const {
    return float_array {
        1, ms.x, ms.y, ms.aceleration, ms.ballx, ms.bally, ms.ballVx, ms.ballVy
    }; //1st is threshold
}

constexpr int8_t ArtificialInteligence_t::calculatedOutput(float_array& weights, float_array& inputs) {
    float totalSum = 0;
    for(uint32_t i = 0; i < inputs.size(); i++) totalSum += inputs[i] * weights[i];

    if(totalSum < 0) return -1;
    return 1;
}

constexpr void ArtificialInteligence_t::updateWeights(float_array& weights, float_array& inputs, int8_t output, bool keyPressed) const {
    int8_t desiredOutput { static_cast<int8_t>(keyPressed ? 1 : -1) };

    for(int32_t i = 0; i < weights.size(); i++)
        weights[i] = weights[i] + (desiredOutput-output)/2 * inputs[i]; //Peligroso modificar un atributo de la clase desde una funcion constante con un puntero??
}

void ArtificialInteligence_t::dumpWeights() const {
    std::ofstream file("weights.csv", std::ios::trunc);
    if(!file) throw std::runtime_error("Can't open weights CSV file for write\n");
    
    std::string separator = "";
    for(float weight : weightsUp) {
        file << separator << weight;
        separator = ";";
    }
    file << "\n";
    separator = "";
    for(float weight : weightsDown) {
        file << separator << weight;
        separator = ";";
    }
    file.close();
}

void ArtificialInteligence_t::readCSV(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if(!file) throw std::runtime_error("Can't open physics CSV file for read\n");
    
    std::string line;
    while(std::getline(file,line)) {
        std::stringstream lineStream(line);
        std::string x, y;
        std::string vx, vy;
        std::string aceleration;
        std::string ballx, bally;
        std::string ballVx, ballVy;
        std::string upPressed;
        std::string downPressed;

        std::getline(lineStream, x, ';');
        std::getline(lineStream, y, ';');
        std::getline(lineStream, vx, ';');
        std::getline(lineStream, vy, ';');
        std::getline(lineStream, aceleration, ';');
        std::getline(lineStream, ballx, ';');
        std::getline(lineStream, bally, ';');
        std::getline(lineStream, ballVx, ';');
        std::getline(lineStream, ballVy, ';');
        std::getline(lineStream, upPressed, ';');
        std::getline(lineStream, downPressed, ';');

        CurrentMatchState_t ms {
            static_cast<float>(atof(x.c_str())),
            static_cast<float>(atof(y.c_str())),
            static_cast<float>(atof(vx.c_str())),
            static_cast<float>(atof(vy.c_str())),
            static_cast<float>(atof(aceleration.c_str())),
            static_cast<float>(atof(ballx.c_str())),
            static_cast<float>(atof(bally.c_str())),
            static_cast<float>(atof(ballVx.c_str())),
            static_cast<float>(atof(ballVy.c_str())),
            static_cast<bool>(atoi(upPressed.c_str())),
            static_cast<bool>(atoi(downPressed.c_str()))
        };
        matchStates.push_back(ms);
    }
    file.close();
}