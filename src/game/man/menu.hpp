#pragma once
#include <iostream>
#include <filesystem>
#include <game/man/state.hpp>
#include <game/util/aitrainer.hpp>

struct MenuState_t : StateBase_t {
    explicit MenuState_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, const InputSystem_t<ECS::EntityManager_t>& inp, bool& playing) 
    : SM{sm}, Render{ren} , Input{inp}, continue_playing { playing }
    {
        Render.updateMenu(menu_options, selected_option);
    }
    void update() final {
        GameTimer_t timer;

        if(Input.isKeyPressed(XK_Down)) {
            ++selected_option;
            timer.timedCall("REN", [&](){ Render.updateMenu(options, selected_option); } );
        }
        if(Input.isKeyPressed(XK_Up)) {
            --selected_option;
            timer.timedCall("REN", [&](){ Render.updateMenu(options, selected_option); } );
        }
        if(Input.isKeyPressed(XK_Return)) {
            switch (selected_option + 1) {
            case 3:
            case 1: m_Alive = false; break;
            case 2:
                options = loadCSVFiles();
                timer.timedCall("REN", [&](){ Render.updateMenu(options, selected_option); } );
                break;
            case 4: m_Alive = false; continue_playing = false; break;
            default: std::cout << "Not existig option\n"; break;
            }
        }

        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";
    }

    // std::string showOptions() {
    //     int opt;
    //     std::vector<std::string> options;
        
    //     do {
    //         std::cout << "\nSelect one of the following:\n";
    //         options = showFiles();
    //         std::cout << " >> ";
    //         if(options.size() == 0) {
    //             std::cout << "No files stored yet, first play a game\n";
    //             break;
    //         } else {
    //             std::cin >> opt;
    //             if(opt >= options.size()) std::cout << "BAD ANSWER\n";
    //         }
    //     } while(opt >= options.size()); //Select one inside the range
        
    //     return options[opt];
    // }
    
    std::vector<std::string> loadCSVFiles() {
        std::vector<std::string> options {};
        for (const auto & entry : std::filesystem::directory_iterator("CSVs"))
            options.push_back(entry.path().string());
        return options;
    }

    bool alive() final { return m_Alive; }

private:
    static constexpr uint32_t kSCRWIDTH  {900};
    static constexpr uint32_t kSCRHEIGHT {500};
    static constexpr uint64_t FPS  { 10 };
    static constexpr uint64_t NSPF { 1000000000/FPS };

    uint8_t selected_option { 0 };
    std::vector<std::string> menu_options {
        "1. Play",
        "2. Train",
        "3. Play against AI",
        "4. Exit"
    };
    std::vector<std::string> options { menu_options };

    bool m_Alive { true };
    bool& continue_playing;
    StateManager_t& SM;

    const RenderSystem_t<ECS::EntityManager_t>& Render;
    const InputSystem_t <ECS::EntityManager_t>& Input;
};
