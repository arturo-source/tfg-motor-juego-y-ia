#pragma once
#include <ecs/cmp/component.hpp>
#include <fstream>
#include <sstream>

struct PerceptronComponent_t : public ECS::ComponentBase_t<PerceptronComponent_t> {
    explicit PerceptronComponent_t(ECS::EntityID_t eid) 
        : ComponentBase_t(eid)
    {}
    
    using float_array = std::array<float, 8>;
    float_array weightsUp {};
    float_array weightsDown {};

    enum {
        Up   = 0x01,
        Down = 0x02
    };

    constexpr bool isKeyPressed(float_array given_inputs, int8_t side) const {
        float totalSum = 0;

        for(int32_t i = 0; i < given_inputs.size(); i++) {
            if(side & Up)   totalSum += given_inputs[i] * weightsUp[i];
            if(side & Down) totalSum += given_inputs[i] * weightsDown[i];
        }

        return totalSum > 0;
    }
    
    void setWeights() {
        std::ifstream file("weights.csv");
        if(!file) throw std::runtime_error("Can't open weights CSV file for read\n");
        readLine(file, weightsUp);
        readLine(file, weightsDown);
    }
    void readLine(std::ifstream& file, float_array& weights) {
        std::string weight_str;
        uint32_t i = 0;
        std::string line;

        std::getline(file,line);
        std::stringstream lineStream(line);
        while(std::getline(lineStream, weight_str, ';')) {
            weights[i] = atof(weight_str.c_str());
            ++i;
        }
        if(i != weights.size()) throw std::runtime_error("Number of weights of CSV is different from AI number of weights");
    }
};