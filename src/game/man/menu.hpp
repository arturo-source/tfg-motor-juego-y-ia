#pragma once
#include <filesystem>
#include <game/man/state.hpp>
#include <game/man/game.hpp>
#include <game/util/aitrainer.hpp>
#include <game/util/gameconfig.hpp>

struct MenuState_t : StateBase_t {
    explicit MenuState_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const uint32_t scrW, const uint32_t scrH) 
    : SM{sm}, Render{ren}, Input{inp}, kSCRWIDTH{scrW}, kSCRHEIGHT{scrH}
    {}
    virtual ~MenuState_t() = default;
    virtual void update()  = 0;

    bool alive() final { return m_Alive; }

    std::vector<std::string> loadCSVFiles(const std::string& dir) {
        std::vector<std::string> options {};
        for (const auto & entry : std::filesystem::directory_iterator(dir))
            options.push_back(entry.path().string());
        
        return options;
    }
protected:
    const uint32_t kSCRWIDTH;
    const uint32_t kSCRHEIGHT;
    static constexpr uint32_t N_times { 1000 };
    static constexpr uint64_t FPS  { 60 };
    static constexpr uint64_t NSPF { 1000000000/FPS };

    uint8_t selected_option { 0 };
    GameConfig gConfig;

    StateManager_t& SM;
    bool m_Alive { true };

    const RenderSystem_t<ECS::EntityManager_t>& Render;
          InputSystem_t <ECS::EntityManager_t>& Input;
};

struct SaveFileMenu_t : MenuState_t {
    explicit SaveFileMenu_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const AItrainer_t& ai, const uint32_t scrW, const uint32_t scrH)
    : MenuState_t(sm, ren, inp, scrW, scrH), AI{ai}
    {}
    void update() final {
        GameTimer_t timer;

        if(gConfig.saveFile) {
            AI.export_weights_as_csv(gConfig.weight_filename);
            m_Alive = false;
        }

        timer.timedCall("REN", [&](){ Render.getMenu().saveFileMenu(gConfig); });
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";
    }

private:
    const AItrainer_t& AI;
};

struct ResultOfTrainingMenu_t : MenuState_t {
    explicit ResultOfTrainingMenu_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const char* filename, const uint32_t scrW, const uint32_t scrH)
    : MenuState_t(sm, ren, inp, scrW, scrH)
    {
        gConfig.data_filename = filename;
        AI.read_data_csv(filename);
    }
    void update() final {
        GameTimer_t timer;

        if(gConfig.train && times_trained < gConfig.n_iter) {
            times_trained += train_offset;
            results.push_back(AI.train(train_offset, gConfig));
        } else {
            gConfig.train = false;
        }

        if(gConfig.setData) {
            results = {};
            times_trained = 0;
            train_offset = gConfig.n_iter/1000;
            gConfig.train = true;
            gConfig.setData = false;
            if(train_offset == 0) train_offset = 1;

            AI.prepareData(gConfig);
        }

        if(gConfig.saveFile) {
            SM.pushState<SaveFileMenu_t>(SM, Render, Input, AI, kSCRWIDTH, kSCRHEIGHT);
            m_Alive = false;
        }

        timer.timedCall("REN", [&](){ Render.getMenu().trainMenu_selectFilters(gConfig, results, AI); });
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";

        if(gConfig.exit) {
            m_Alive = false;
        }
    }
private:
    AItrainer_t AI;
    
    uint32_t train_offset;
    uint32_t times_trained;
    std::vector<float> results {};
};

struct TrainingMenu_t : MenuState_t {
    explicit TrainingMenu_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const uint32_t scrW, const uint32_t scrH)
    : MenuState_t(sm, ren, inp, scrW, scrH)
    {
        static std::vector<std::string> files_str;
        files_str = loadCSVFiles("dataset_CSVs");
        for(auto& f: files_str)
            files.push_back(f.c_str());
    }
    void update() final {
        GameTimer_t timer;

        if(gConfig.readFile && gConfig.data_filename != nullptr) {
            gConfig.readFile = false;
            SM.pushState<ResultOfTrainingMenu_t>(SM, Render, Input, gConfig.data_filename, kSCRWIDTH, kSCRHEIGHT);
            m_Alive = false;
        }

        if(gConfig.exit) {
            m_Alive = false;
        }

        timer.timedCall("REN", [&](){ Render.getMenu().trainMenu_selectFile(gConfig, files); });
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";
    }
private:
    std::vector<const char*> files;
};


struct MainMenu_t : MenuState_t {
    explicit MainMenu_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const uint32_t scrW, const uint32_t scrH) 
    : MenuState_t(sm, ren, inp, scrW, scrH)
    {
        resetFilesPointers();
    }
    void update() final {
        GameTimer_t timer;

        if(gConfig.play) SM.pushState<GameManager_t>(SM, Render, Input, gConfig, kSCRWIDTH, kSCRHEIGHT);
        if(gConfig.train) SM.pushState<TrainingMenu_t>(SM, Render, Input, kSCRWIDTH, kSCRHEIGHT);
        if(gConfig.Lplayer_AI || gConfig.Rplayer_AI) resetFilesPointers();
        if(gConfig.exit) m_Alive = false;

        timer.timedCall("REN", [&](){ Render.getMenu().mainMenu(gConfig, files); });
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";
    }
    void resetFilesPointers() {
        static std::vector<std::string> files_str;
        files_str = loadCSVFiles("weights_CSVs");
        files = {};
        for(auto& f: files_str)
            files.push_back(f.c_str());
    }
private:
    std::vector<const char*> files;
};
