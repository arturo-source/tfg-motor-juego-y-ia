#include <game/util/aitrainer.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <random>

void AItrainer_t::train(const int n_iter, const GameConfig& gConfig, float& failed_press_up, float& failed_press_down, float& failed_nopress) {
    // nn.print();
    nn.backpropagation(dataInputs, dataOutputs, n_iter, gConfig);
    // nn.average_error(dataInputs, dataOutputs);
    failed_keys(failed_press_up, failed_press_down, failed_nopress);
}

void AItrainer_t::failed_keys(float& failed_press_up, float& failed_press_down, float& failed_nopress) {
    nn.failed_keys(dataInputs, dataOutputs, failed_press_up, failed_press_down, failed_nopress);
}

void AItrainer_t::export_weights_as_csv(const char* filename) const {
    std::string f {"weights_CSVs/"};
    f += filename;
    nn.export_as_csv(f);
}

void AItrainer_t::prepareData(const GameConfig& gConfig) {
    std::vector<uint32_t> neurons;
    neurons.push_back(12);
    for(std::size_t i = 0; i<gConfig.n_layers; ++i) 
        neurons.push_back(gConfig.neuron_per_layer);

    neurons.push_back(2);
    nn.setNeurons(neurons);
}


void AItrainer_t::read_data_csv(const std::string& filename) {
    std::ifstream file(filename.c_str());
    if(!file) throw std::runtime_error("Can't open physics CSV file for read\n");
    
    std::string line;
    while(std::getline(file,line)) {
        std::stringstream lineStream(line);
        std::string x, y;
        std::string vy;
        std::string aceleration;
        std::string my_ballx, my_bally;
        std::string my_ballVx, my_ballVy;
        std::string other_ballx, other_bally;
        std::string other_ballVx, other_ballVy;
        std::string minion_x, minion_y;
        std::string minion_vy;
        std::string minion_aceleration;
        std::string upPressed;
        std::string downPressed;

        std::getline(lineStream, x, ';');
        std::getline(lineStream, y, ';');
        std::getline(lineStream, vy, ';');
        std::getline(lineStream, aceleration, ';');
        std::getline(lineStream, my_ballx, ';');
        std::getline(lineStream, my_bally, ';');
        std::getline(lineStream, my_ballVx, ';');
        std::getline(lineStream, my_ballVy, ';');
        std::getline(lineStream, other_ballx, ';');
        std::getline(lineStream, other_bally, ';');
        std::getline(lineStream, other_ballVx, ';');
        std::getline(lineStream, other_ballVy, ';');
        std::getline(lineStream, minion_x, ';');
        std::getline(lineStream, minion_y, ';');
        std::getline(lineStream, minion_vy, ';');
        std::getline(lineStream, minion_aceleration, ';');
        std::getline(lineStream, upPressed, ';');
        std::getline(lineStream, downPressed, ';');

        VecDouble_t matchStateInput {
            atof(x.c_str()),
            atof(y.c_str()),
            // atof(vy.c_str()),
            // atof(aceleration.c_str()),
            atof(my_ballx.c_str()),
            atof(my_bally.c_str()),
            atof(my_ballVx.c_str()),
            atof(my_ballVy.c_str()),
            atof(other_ballx.c_str()),
            atof(other_bally.c_str()),
            atof(other_ballVx.c_str()),
            atof(other_ballVy.c_str()),
            atof(minion_x.c_str()),
            atof(minion_y.c_str()),
            // atof(minion_vy.c_str()),
            // atof(minion_aceleration.c_str())
        };
        dataInputs.push_back(matchStateInput);

        VecDouble_t matchStateOutput {
            atoi(upPressed.c_str()) ? 1.0 : 0.0,
            atoi(downPressed.c_str()) ? 1.0 : 0.0
        };
        dataOutputs.push_back(matchStateOutput);
    }
    file.close();
}