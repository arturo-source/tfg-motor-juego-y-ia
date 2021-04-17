#pragma once
#include <cstdint>
#include <vector>
#include <game/cmp/neuralnetwork.hpp>
#include <game/util/gameconfig.hpp>

struct AItrainer_t {
    explicit AItrainer_t() = default;
    using Vec_MatchStates = std::vector<VecDouble_t>;

    void read_data_csv(const std::string& filename);
    void prepareData(const GameConfig& gConfig);
    double train(const int n_iter, const GameConfig& gConfig);
    void export_weights_as_csv(const char* filename) const;

    void infoFromData(uint32_t& data_without_touch, uint32_t& data_with_up, uint32_t& data_with_down, uint32_t& total) const noexcept {
        data_without_touch = 0; data_with_up = 0; data_with_down = 0;
        for(const auto& output: dataOutputs) {
            if(output[0] == 1.0) ++data_with_up;
            if(output[1] == 1.0) ++data_with_down;
            if(output[0] == 0.0 && output[1] == 0.0) ++data_without_touch;
        }
        total = dataOutputs.size();
    }
    
private:
    Vec_MatchStates dataInputs {};
    Vec_MatchStates dataOutputs {};
    NeuralNetwork_t nn{0};
};
