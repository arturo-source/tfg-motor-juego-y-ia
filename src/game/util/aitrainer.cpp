#include <game/util/aitrainer.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <random>

void AI_trainer_t::train(const std::string& filename, const int32_t N) {
    readCSV(filename);
    train(this->weightsUp, this->Up, N);
    train(this->weightsDown, this->Down, N);
    dumpWeights();
}

auto AI_trainer_t::adjust_dataset(const Vec_MS& states, const int8_t usedKey) const {
    Vec_MS correctStates;
    uint32_t keysPressedAdded = 0;
    for(const CurrentMatchState_t& s: states) {
        if(isKeyPressed(s, usedKey)) {
            correctStates.push_back(s);
            ++keysPressedAdded;
        }
    }
    
    if(keysPressedAdded > states.size()/2) throw std::runtime_error("Too much pressed keys in dataset.");

    uint32_t notKeysPressedAdded = 0;
    std::default_random_engine randeng{ 0 }; //Seed is 0 to debug
    while (notKeysPressedAdded != keysPressedAdded) {
        const CurrentMatchState_t& s = states[randeng()%states.size()];
        if(!isKeyPressed(s, usedKey)) {
            correctStates.push_back(s);
            ++notKeysPressedAdded;
        }
    }
    

    return correctStates;
}

auto AI_trainer_t::calculateErrors(const Vec_MS& dataset, float_array& weights, const int8_t usedKey) const {
    std::vector<uint32_t> errors;
    uint32_t index = 0;

    for(const CurrentMatchState_t& ms: dataset) {
        bool isPressed { isKeyPressed(ms, usedKey) };
        float_array given_physics { CurrentMatchState2array(ms) };
        
        int8_t perceptronOutput { calculateOutput(weights, given_physics) };
        int8_t userOutput       { static_cast<int8_t>(isPressed ? 1 : -1) };
        
        if(userOutput != perceptronOutput) errors.push_back(index); 

        ++index;
    }

    return errors;
}

void AI_trainer_t::train(float_array& weights, const int8_t usedKey, const int32_t N) {
    auto dataset { adjust_dataset(matchStates, usedKey) };

    std::size_t currLessErrors { dataset.size() };
    float_array bestWeights {}; //Pocket
    std::default_random_engine randeng{ 0 }; //Seed is 0 to debug

    for(uint32_t i = 0; i<N; ++i) {
        auto errors { calculateErrors(dataset, weights, usedKey) };
        if(errors.size() < currLessErrors) { 
            bestWeights = weights;
            currLessErrors = errors.size();
            if(currLessErrors == 0) break; //Success on all example cases
        }

        const uint32_t randindex = errors[randeng()%errors.size()];
        const float_array given_physics = CurrentMatchState2array(dataset[randindex]);
        const int8_t addOrSubstract { static_cast<int8_t>(isKeyPressed(dataset[randindex], usedKey) ? 1 : -1) };
        updateWeights(weights, given_physics, addOrSubstract);
    }

    std::cout << "Total training fails: " << currLessErrors << "/" << dataset.size() << "\n";
    weights = bestWeights;
}

constexpr bool AI_trainer_t::isKeyPressed(const CurrentMatchState_t& ms, const int8_t usedKey) const {
    return (usedKey == Up) ? ms.upPressed : ms.downPressed;
}

constexpr AI_trainer_t::float_array AI_trainer_t::CurrentMatchState2array(const CurrentMatchState_t& ms) const {
    return float_array {
        1, ms.y, ms.vy, ms.aceleration, ms.ballx, ms.bally, ms.ballVx, ms.ballVy
    }; //1st is threshold
}

constexpr int8_t AI_trainer_t::calculateOutput(const float_array& weights, const float_array& inputs) const {
    float totalSum = 0;
    for(uint32_t i = 0; i < inputs.size(); i++) totalSum += inputs[i] * weights[i];

    if(totalSum < 0) return -1;
    return 1;
}

constexpr void AI_trainer_t::updateWeights(float_array& weights, const float_array& inputs, const int8_t addOrSubstract) const {
    for(uint32_t i = 0; i < weights.size(); i++)
        weights[i] = weights[i] + addOrSubstract*inputs[i]; //Peligroso modificar un atributo de la clase desde una funcion constante con un puntero??
}

void AI_trainer_t::dumpWeights() const {
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

void AI_trainer_t::readCSV(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if(!file) throw std::runtime_error("Can't open physics CSV file for read\n");
    
    std::string line;
    while(std::getline(file,line)) {
        std::stringstream lineStream(line);
        std::string y;
        std::string vy;
        std::string aceleration;
        std::string ballx, bally;
        std::string ballVx, ballVy;
        std::string upPressed;
        std::string downPressed;

        std::getline(lineStream, y, ';');
        std::getline(lineStream, vy, ';');
        std::getline(lineStream, aceleration, ';');
        std::getline(lineStream, ballx, ';');
        std::getline(lineStream, bally, ';');
        std::getline(lineStream, ballVx, ';');
        std::getline(lineStream, ballVy, ';');
        std::getline(lineStream, upPressed, ';');
        std::getline(lineStream, downPressed, ';');

        CurrentMatchState_t ms {
            static_cast<float>(atof(y.c_str())),
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