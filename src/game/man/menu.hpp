#pragma once
#include <iostream>
#include <filesystem>
#include <game/man/state.hpp>
#include <game/util/aitrainer.hpp>

struct MenuState_t : StateBase_t {
    explicit MenuState_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, const InputSystem_t<ECS::EntityManager_t>& inp, bool& playing) 
    : SM{sm}, Render{ren} , Input{inp}, continue_playing { playing }
    {}
    void update() final {
        GameTimer_t timer;
        timer.timedCall("REN", [&](){ Render.updateMenu(); } );
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << " Numpres: " << Input.numberPressed() << "\n";
        switch (Input.numberPressed()) {
            case 1: m_Alive = false; break;
            case 2:
                // filename = showOptions();
                std::cout << "Times of training: ";
                // std::cin >> N;
                // AI.train(filename, N);
                break;
            case 3: m_Alive = false; break;
            case 4: m_Alive = false; continue_playing = false; break;
            default: break;
        }
    }

    std::string showOptions() {
        int opt;
        std::vector<std::string> options;
        
        do {
            std::cout << "\nSelect one of the following:\n";
            options = showFiles();
            std::cout << " >> ";
            if(options.size() == 0) {
                std::cout << "No files stored yet, first play a game\n";
                break;
            } else {
                std::cin >> opt;
                if(opt >= options.size()) std::cout << "BAD ANSWER\n";
            }
        } while(opt >= options.size()); //Select one inside the range
        
        return options[opt];
    }
    
    std::vector<std::string> showFiles() {
        std::vector<std::string> options {};
        unsigned int pos = 0;
        for (const auto & entry : std::filesystem::directory_iterator("CSVs")) {
            std::cout << pos++ << ": " << entry.path() << "\n";
            options.push_back(entry.path().string());
        }
        return options;
    }

    bool alive() final { return m_Alive; }
private:
    static constexpr uint32_t kSCRWIDTH  {900};
    static constexpr uint32_t kSCRHEIGHT {500};
    static constexpr uint64_t FPS  { 30 };
    static constexpr uint64_t NSPF { 1000000000/FPS };

    bool m_Alive { true };
    bool& continue_playing;
    StateManager_t& SM;

    const RenderSystem_t<ECS::EntityManager_t>& Render;
    const InputSystem_t <ECS::EntityManager_t>& Input;
};
