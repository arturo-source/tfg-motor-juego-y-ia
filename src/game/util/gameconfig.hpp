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
    bool editweights      = false;
    bool trainLside       = false;
    bool trainRside       = false;
    int  n_iter           = 10000;
    int  n_layers         = 3;
    int  neuron_per_layer = 6;
    float learning_rate         = 0.001;
    float no_touch_importance   = 0.33;
    float up_touch_importance   = 0.33;
    float down_touch_importance = 0.33;
    const char* data_filename   = nullptr;
    const char* editweight_filename = nullptr;
    const char* Lplayer_AI_file = nullptr;
    const char* Rplayer_AI_file = nullptr;
    char weight_filename[128]   = "weights.csv";
};
