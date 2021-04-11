#include <game/cmp/neuralnetwork.hpp>
#include <random>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cassert>

//-------------Neuron-------------
Neuron_t::Neuron_t(uint32_t number_of_imputs, uint32_t position_in_layer, std::vector<Neuron_t>* output_neurons_pointer)
: m_inputs(number_of_imputs + 1), m_weights(number_of_imputs + 1), m_weights_after_backpropagation(number_of_imputs + 1),
m_position_in_layer(position_in_layer), m_output_neurons(output_neurons_pointer)
{ fillVectorRandom(m_weights, -1, 1); }

Neuron_t::Neuron_t(const VecDouble_t& weights, uint32_t position_in_layer, std::vector<Neuron_t>* output_neurons_pointer)
: m_inputs(weights.size()), m_weights(weights), m_weights_after_backpropagation(weights.size()),
m_position_in_layer(position_in_layer), m_output_neurons(output_neurons_pointer)
{}
double Neuron_t::randDouble(double min, double max) {
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_real_distribution<double> dist(min, max);

    return dist(rng);
}
void Neuron_t::fillVectorRandom(VecDouble_t& vec, double min, double max) {
    for(auto& v: vec)
        v = randDouble(min, max);
}
double Neuron_t::sigmoid(double x) {
    return 1 / (1 + std::exp(-x));
}
double Neuron_t::feedforward(const VecDouble_t& inputs) {
    assert(inputs.size() == m_inputs.size());
    
    double activation { 0 };

    for(std::size_t i=0; i < inputs.size(); ++i) {
        m_inputs[i] = inputs[i];
        activation += m_weights[i]*inputs[i];
    }
    m_output = sigmoid(activation);
    m_derivedSigmoid = m_output*(1 - m_output);

    return m_output;
}
void Neuron_t::backpropagation(double expected_output, double learning_rate) {
    if(m_output_neurons == nullptr) {
        m_delta = (m_output - expected_output) * m_derivedSigmoid;
    } else {
        double delta_sum { 0 };
        for(const auto& output_neuron: *m_output_neurons)
            delta_sum += output_neuron.m_delta * output_neuron.m_weights[m_position_in_layer];
        
        m_delta = delta_sum*m_derivedSigmoid;
    }

    for(std::size_t i=0; i < m_weights_after_backpropagation.size(); ++i)
        m_weights_after_backpropagation[i] = m_weights[i] - m_delta*m_inputs[i]*learning_rate;
}
void Neuron_t::update_weights() {
    for(std::size_t i=0; i < m_weights_after_backpropagation.size(); ++i)
        m_weights[i] = m_weights_after_backpropagation[i];
}
void Neuron_t::export_as_csv(std::ofstream& file) const {
    std::string weight_separator = "";
    for(const auto& weight: m_weights) {
        file << weight_separator << weight;
        weight_separator = ":";
    }
}

//-------------Layer-------------
Layer_t::Layer_t(uint32_t number_of_neurons, uint32_t inputs_per_neuron, Layer_t* output_layer) {
    for(std::size_t i=0; i < number_of_neurons; ++i) {
        if(output_layer)
            m_neurons.push_back(Neuron_t(inputs_per_neuron, i, &output_layer->m_neurons));
        else 
            m_neurons.push_back(Neuron_t(inputs_per_neuron, i, nullptr));
    }
}

Layer_t::Layer_t(std::vector<VecDouble_t> neurons_w, Layer_t* output_layer) {
    for(std::size_t i=0; i < neurons_w.size(); ++i) {
        if(output_layer)
            m_neurons.push_back(Neuron_t(neurons_w[i], i, &output_layer->m_neurons));
        else 
            m_neurons.push_back(Neuron_t(neurons_w[i], i, nullptr));
    }
}
VecDouble_t Layer_t::feedforward(const VecDouble_t& inputs) {
    VecDouble_t outputs(m_neurons.size());

    for(std::size_t i=0; i < m_neurons.size(); ++i)
        outputs[i] = m_neurons[i].feedforward(inputs);

    return outputs;
}
void Layer_t::backpropagation(const VecDouble_t& expected_output, double learning_rate) {
    for(std::size_t i=0; i < m_neurons.size(); ++i) 
        m_neurons[i].backpropagation(expected_output[i], learning_rate);
}
void Layer_t::update_weights() {
    for(auto& neuron: m_neurons)
        neuron.update_weights();
}
void Layer_t::export_as_csv(std::ofstream& file) const {
    std::string neuron_separator = "";
    for(const auto& neuron: m_neurons) {
        file << neuron_separator;
        neuron.export_as_csv(file);
        neuron_separator = "|";            
    }
}

//-------------Neural network-------------
VecDouble_t NeuralNetwork_t::feedforward(const VecDouble_t& inputs) {
    VecDouble_t inputs_copy(inputs);

    for(auto& layer: m_layers) {
        inputs_copy.insert(inputs_copy.begin(), 1.0); //Add bias to the inputs
        inputs_copy = layer.feedforward(inputs_copy); //Layer feedforward returns the output of the neurons
    }
    
    return inputs_copy; 
}
void NeuralNetwork_t::backpropagation(const std::vector<VecDouble_t>& X, const std::vector<VecDouble_t>& y, uint32_t num_iterations, double learning_rate) {
    if(X.size() != y.size()) throw std::out_of_range("Given total inputs sample is different from given total outputs sample"); 

    for(std::size_t i=0; i < num_iterations; ++i) {
        int index = randInt(0,X.size()-1);
        // for(std::size_t j=0; j < X.size(); ++j) {
            this->feedforward(X[index]);

            for(auto it = m_layers.rbegin(); it != m_layers.rend(); ++it)
                it->backpropagation(y[index], learning_rate);

            for(auto& layer: m_layers) 
                layer.update_weights();
        // }

        // if(i%10000==0) {
        //     double avg_error { average_error(X, y) };
        //     std::cout<< "Iteración " << i << " - error " << avg_error << "\n";
        //     // if(avg_error > 0.2) {
        //     //     if(learning_rate > 1) learning_rate /= 2;
        //     //     else learning_rate *= 2;
        //     // }
        // }
    }
}
int NeuralNetwork_t::randInt(int min, int max) {
    static std::random_device dev;
    static std::mt19937 rng(dev());
    static std::uniform_int_distribution<int> dist(min, max);

    return dist(rng);
}
double NeuralNetwork_t::average_error(const std::vector<VecDouble_t>& X, const std::vector<VecDouble_t>& y) {
    double avg_error { 0.0 };
    for(std::size_t i=0; i < X.size(); ++i) {
        auto output = this->feedforward(X[i]);
        if(output.size() != y[i].size()) throw std::out_of_range("Number of neural network outputs is diffetent than given sample outputs.");

        for(std::size_t j=0; j < output.size(); ++j) {
            avg_error += (output[j] - y[i][j])*(output[j] - y[i][j]);
        }
        // avg_error /= output.size();
    }

    return avg_error/X.size();
}
void NeuralNetwork_t::export_as_csv(const std::string& filename) const {
    std::ofstream file(filename, std::ios::trunc);
    if(!file) throw std::runtime_error("Can't open weights CSV file for write\n");

    std::string layer_separator = "";
    for(const auto& layer: m_layers) {
        file << layer_separator;
        layer.export_as_csv(file);
        layer_separator = ";";
    }
    
    file.close();
}
std::vector<std::vector<VecDouble_t>> NeuralNetwork_t::read_from_csv(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if(!file) throw std::runtime_error("Can't open weights CSV file for read\n");

    std::vector<std::vector<VecDouble_t>> weights;

    std::string layer_str;
    std::string line;
    std::getline(file,line);
    std::stringstream lineStream(line);
    while (std::getline(lineStream,layer_str,';')) {
        std::string neuron_str;
        std::stringstream layerStream(layer_str);
        std::vector<VecDouble_t> layer_w;
        while (std::getline(layerStream,neuron_str,'|')) {
            std::string weight_str;
            std::stringstream neuronStream(neuron_str);
            VecDouble_t neuron_w;
            while (std::getline(neuronStream,weight_str,':')) {
                neuron_w.push_back(atof(weight_str.c_str()));
            }
            layer_w.push_back(neuron_w);
        }
        weights.push_back(layer_w);
    }

    return weights;
}
void NeuralNetwork_t::setNeurons(const std::string& filename) {
    m_layers = {};
    auto weights { read_from_csv(filename) };
    
    Layer_t* output_layer { nullptr };
    m_layers.reserve(weights.size());

    for(auto layer_w = weights.rbegin(); layer_w != weights.rend(); ++layer_w) {
        m_layers.insert(m_layers.begin(), Layer_t(*layer_w, output_layer));
        output_layer = &(m_layers[0]);
    }
}
void NeuralNetwork_t::setNeurons(const std::vector<uint32_t>& layers) {
    if(layers.size() < 2) throw std::out_of_range("Not enought layers.");
    m_layers = {};

    Layer_t* output_layer { nullptr };
    m_layers.reserve(layers.size()-1);

    auto input_size_it = layers.end() - 2;
    for(auto it = layers.end() - 1; it != layers.begin(); --it) {
        m_layers.insert(m_layers.begin(), Layer_t(*it, *input_size_it, output_layer));
        output_layer = &(m_layers[0]);
        --input_size_it;
    }
}