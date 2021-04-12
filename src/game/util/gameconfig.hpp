#pragma once

struct GameConfig {
    bool play             = false;
    bool train            = false;
    bool arena            = false;
    bool Lplayer_AI       = false;
    bool Rplayer_AI       = false;
    bool exit             = false;
    bool readFile         = false;
    bool setData          = false;
    bool saveFile         = false;
    int  n_iter           = 20000;
    int  n_layers         = 4;
    int  neuron_per_layer = 10;
    float learning_rate   = 1.0;
    const char* data_filename = nullptr;
    const char* Lplayer_AI_file = nullptr;
    const char* Rplayer_AI_file = nullptr;
    char weight_filename[128] = "weights.csv";
};
