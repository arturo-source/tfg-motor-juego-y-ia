#pragma once
#include <ecs/cmp/component.hpp>
#include <game/util/gameconfig.hpp>

#include <vector>
#include <cstdint>
// #include <initializer_list>
#include <iostream>

using VecDouble_t = std::vector<double>;

struct Neuron_t {
    explicit Neuron_t(uint32_t number_of_imputs, uint32_t position_in_layer, std::vector<Neuron_t>* output_neurons_pointer);
    explicit Neuron_t(const VecDouble_t& weights, uint32_t position_in_layer, std::vector<Neuron_t>* output_neurons_pointer);

    double feedforward(const VecDouble_t& inputs);
    void backpropagation(double expected_output, double learning_rate);
    void update_weights();
    void export_as_csv(std::ofstream& file) const;
    void print() const {
        for(const auto& w : m_weights)
            std::cout << w << " ";
    }

    VecDouble_t m_weights;
private:
    void fillVectorRandom(VecDouble_t& vec, double min, double max);
    double randDouble(double min, double max);
    double sigmoid(double x);

    VecDouble_t m_inputs;
    // VecDouble_t m_weights;
    VecDouble_t m_weights_after_backpropagation;

    double m_output { 0.0 };
    double m_delta  { 0.0 };
    double m_derivedSigmoid { 0.0 };

    uint32_t m_position_in_layer { 0 };

    std::vector<Neuron_t>* m_output_neurons { nullptr };
};

struct Layer_t {
    explicit Layer_t(uint32_t number_of_neurons, uint32_t inputs_per_neuron, Layer_t* output_layer);
    explicit Layer_t(std::vector<VecDouble_t> neurons_w, Layer_t* output_layer);

    VecDouble_t feedforward(const VecDouble_t& inputs);
    void backpropagation(const VecDouble_t& expected_output, double learning_rate);
    void update_weights();
    void export_as_csv(std::ofstream& file) const;
    void print() const {
        for(const auto& n : m_neurons) {
            n.print();
            std::cout << std::endl;
        }
    }
// private:
    std::vector<Neuron_t> m_neurons {};
};

struct NeuralNetwork_t : public ECS::ComponentBase_t<NeuralNetwork_t> {
    using VecUInt_t = std::vector<uint32_t>;

    explicit NeuralNetwork_t(ECS::EntityID_t eid) : ComponentBase_t(eid) 
    {}

    VecDouble_t feedforward(const VecDouble_t& inputs);
    void backpropagation(const std::vector<VecDouble_t>& X, const std::vector<VecDouble_t>& y, uint32_t num_iterations, const GameConfig& gConfig);
    void export_as_csv(const std::string& filename) const;
    void setNeurons(const std::vector<uint32_t>& layers);
    void setNeurons(const std::string& filename);
    double average_error(const std::vector<VecDouble_t>& X, const std::vector<VecDouble_t>& y);
    void failed_keys(const std::vector<VecDouble_t>& X, const std::vector<VecDouble_t>& y, float& failed_press_up, float& failed_press_down, float& failed_nopress);

    void print() const {
        for(int i=0; i<m_layers.size(); ++i){
            std::cout << "Layer[" << i << "] ";
            m_layers[i].print();
        }
    }

    std::vector<Layer_t> m_layers;
private:
    std::vector<std::vector<VecDouble_t>> read_from_csv(const std::string& filename);
    VecUInt_t adjust_data(const VecUInt_t& no_touch_indexes, const VecUInt_t& up_touch_indexes, const VecUInt_t& down_touch_indexes, const size_t size, const GameConfig& gConfig);
    void prepare_data(const std::vector<VecDouble_t>& y, VecUInt_t& no_touch_indexes, VecUInt_t& up_touch_indexes, VecUInt_t& down_touch_indexes);
    int randInt(int min, int max);
    float randFloat(float min, float max);
};
