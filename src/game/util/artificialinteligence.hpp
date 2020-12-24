#pragma once
#include <cstdint>
#include <vector>
#include <array>
#include <string>

struct CurrentMatchState_t {
    float y;
    float vy;
    float aceleration;
    float ballx, bally;
    float ballVx, ballVy;
    bool  upPressed;
    bool  downPressed;
};

struct ArtificialInteligence_t {
    explicit ArtificialInteligence_t() = default;
    using float_array = std::array<float, 8>;
    using Vec_MS      = std::vector<CurrentMatchState_t>;

    void train(const std::string& filename, const int32_t N);
private:
    enum {
        Up   = 0x01,
        Down = 0x02
    };

    void train(float_array& weights, const int8_t usedKey, const int32_t N);
    auto adjust_dataset(const Vec_MS& states, const int8_t usedKey) const;
    void dumpWeights() const;
    void readCSV(const std::string& filename);
    constexpr float_array CurrentMatchState2array(const CurrentMatchState_t& ms) const;
    constexpr int8_t calculateOutput(const float_array& weights, const float_array& inputs) const;
    auto calculateErrors(const Vec_MS& dataset, float_array& weights, const int8_t usedKey) const;
    constexpr void updateWeights(float_array& weights, const float_array& inputs, const int8_t addOrSubstract) const;
    constexpr bool isKeyPressed(const CurrentMatchState_t& ms, const int8_t usedKey) const;

    float_array weightsUp {};
    float_array weightsDown {};
    Vec_MS matchStates {};
};
