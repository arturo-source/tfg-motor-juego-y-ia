#pragma once
#include <iostream>
#include <filesystem>
#include <game/man/state.hpp>
#include <game/man/game.hpp>
#include <game/util/aitrainer.hpp>

struct MenuState_t : StateBase_t {
    explicit MenuState_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const uint32_t scrW, const uint32_t scrH) 
    : SM{sm}, Render{ren}, Input{inp}, kSCRWIDTH{scrW}, kSCRHEIGHT{scrH}
    {}
    virtual ~MenuState_t() = default;
    virtual void update()  = 0;

    bool alive() final { return m_Alive; }
protected:
    const uint32_t kSCRWIDTH;
    const uint32_t kSCRHEIGHT;
    static constexpr uint32_t N_times { 1000 };
    static constexpr uint64_t FPS  { 10 };
    static constexpr uint64_t NSPF { 1000000000/FPS };

    uint8_t selected_option { 0 };
    std::vector<std::string> options;

    StateManager_t& SM;
    bool m_Alive { true };

    const RenderSystem_t<ECS::EntityManager_t>& Render;
          InputSystem_t <ECS::EntityManager_t>& Input;
};


struct TrainingMenu_t : MenuState_t {
    explicit TrainingMenu_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const uint32_t scrW, const uint32_t scrH)
    : MenuState_t(sm, ren, inp, scrW, scrH)
    {
        options = loadCSVFiles();
    }
    void update() final {
        GameTimer_t timer;
        if(Input.isKeyPressed(ECS::Down) && selected_option < options.size()-1) ++selected_option;
        if(Input.isKeyPressed(ECS::Up)   && selected_option > 0)                --selected_option;
        if(Input.isKeyPressed(ECS::Esc)) m_Alive = false;
        if(Input.isKeyPressed(ECS::Intro)) {
            AI.train(options[selected_option], N_times);
            m_Alive = false;
        }

        timer.timedCall("REN", [&](){ Render.updateMenu(options, selected_option); });
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";
    }
    
    std::vector<std::string> loadCSVFiles() {
        std::vector<std::string> options {};
        for (const auto & entry : std::filesystem::directory_iterator("CSVs"))
            options.push_back(entry.path().string());
        return options;
    }
private:
    AI_trainer_t AI{};
};


struct MainMenu_t : MenuState_t {
    explicit MainMenu_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const uint32_t scrW, const uint32_t scrH) 
    : MenuState_t(sm, ren, inp, scrW, scrH)
    {
        options = {
            "1. Play",
            "2. Train",
            "3. Play against AI",
            "4. Exit"
        };
    }
    void update() final {
        GameTimer_t timer;

        if(Input.isKeyPressed(ECS::Down) && selected_option < options.size()-1) ++selected_option;
        if(Input.isKeyPressed(ECS::Up)   && selected_option > 0)                --selected_option;
        if(Input.isKeyPressed(ECS::Intro)) {
            switch (selected_option + 1) {
            case 1:
            case 3: SM.pushState<GameManager_t>(SM, Render, Input, kSCRWIDTH, kSCRHEIGHT); break;
            case 2: SM.pushState<TrainingMenu_t>(SM, Render, Input, kSCRWIDTH, kSCRHEIGHT); break;
            case 4: m_Alive = false; break;
            default: std::cout << "Not existig option\n"; break;
            }
        }

        timer.timedCall("REN", [&](){ Render.updateMenu(options, selected_option); });
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";
    }
};
