#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <string>

struct CurrentMatchState_t {
    float x, y;
    float vx, vy;
    float aceleration;
    float ballx, bally;
    float ballVx, ballVy;
    bool  upPressed;
    bool  downPressed;
};

struct ArtificialInteligence_t {
    explicit ArtificialInteligence_t() = default;
    using float_array = std::array<float, 10>;

    void train(const std::string& filename);
private:
    enum {
        Up   = 0x01,
        Down = 0x02
    };

    void train(int8_t key);
    void dumpWeights() const;
    void readCSV(const std::string& filename);
    constexpr float_array CurrentMatchState2array(CurrentMatchState_t& ms) const;
    constexpr int8_t calculatedOutput(float_array& weights, float_array& inputs);
    constexpr void updateWeights(float_array& weights, float_array& inputs, int8_t output, bool keyPressed) const;

    float_array weightsUp {};
    float_array weightsDown {};
    std::vector<CurrentMatchState_t>  matchStates {};
};
