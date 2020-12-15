#pragma once
#include <ecs/cmp/component.hpp>
#include <fstream>

struct InteligenceComponent_t : public ECS::ComponentBase_t<InteligenceComponent_t> {
    explicit InteligenceComponent_t(ECS::EntityID_t eid) 
        : ComponentBase_t(eid)
    {}
    
    using float_array = std::array<float, 10>;
    float_array inputs {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    float_array weights {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };

    int8_t output {0};
    float threshold = 0;
    inline static const char* filename = "weights.csv";

    bool keyUP_pressed { false };
    bool keyDOWN_pressed { false };

    constexpr int8_t calculateOutput(float_array given_inputs) {
        inputs = given_inputs;
        float totalSum = 0;

        for(int32_t i = 0; i < inputs.size(); i++)
            totalSum += inputs[i] * weights[i];

        if(totalSum > threshold)
            output = 1;
        else
            output = 0;

        return output;
    }
    
    void setWeights() {
        std::ifstream file(filename);
        if(!file) throw std::runtime_error("Can't open weights CSV file for read\n");
        std::string weight_str;
        uint32_t i = 0;

        while(getline(file, weight_str, ';')) {
            weights[i] = atof(weight_str.c_str());
            ++i;
        }
        if(i != weights.size()) throw std::runtime_error("Number of weights of CSV is different from AI number of weights");
    }
};