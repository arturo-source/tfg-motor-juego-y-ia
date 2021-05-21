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

    bool alive() final { return m_Alive && !Render.shouldClose();; }

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
            float f_press_up, f_press_down, f_nopress;
            AI.train(train_offset, gConfig, f_press_up, f_press_down, f_nopress);

            failed_press_up.push_back(f_press_up);
            failed_press_down.push_back(f_press_down);
            failed_nopress.push_back(f_nopress);
        } else {
            gConfig.train = false;
        }

        if(gConfig.setData) {
            failed_press_up = {};
            failed_press_down = {};
            failed_nopress = {};
            times_trained = 0;
            train_offset = gConfig.n_iter/200;
            gConfig.train = true;
            gConfig.setData = false;
            if(train_offset == 0) train_offset = 1;

            AI.prepareData(gConfig);
        }

        if(gConfig.saveFile) {
            SM.pushState<SaveFileMenu_t>(SM, Render, Input, AI, kSCRWIDTH, kSCRHEIGHT);
            m_Alive = false;
        }

        timer.timedCall("REN", [&](){ Render.getMenu().trainMenu_selectFilters(gConfig, failed_press_up, failed_press_down, failed_nopress, AI); });
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
    std::vector<float> failed_press_up {};
    std::vector<float> failed_press_down {};
    std::vector<float> failed_nopress {};
};

struct EditWeightsMenu_t : MenuState_t {
    explicit EditWeightsMenu_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const char* train_file, const char* weights_file, const uint32_t scrW, const uint32_t scrH)
    : MenuState_t(sm, ren, inp, scrW, scrH)
    {
        AI.nn.setNeurons(weights_file);
        AI.read_data_csv(train_file);
    }
    void update() final {
        GameTimer_t timer;

        if(gConfig.saveFile) {
            SM.pushState<SaveFileMenu_t>(SM, Render, Input, AI, kSCRWIDTH, kSCRHEIGHT);
            m_Alive = false;
        }

        if(gConfig.exit) {
            m_Alive = false;
        }

        timer.timedCall("REN", [&](){ Render.getMenu().editweightsMenu(gConfig, AI); });
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";
    }
private:
    AItrainer_t AI;
};

struct SelectFileMenu2_t : MenuState_t {
    explicit SelectFileMenu2_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const uint32_t scrW, const uint32_t scrH)
    : MenuState_t(sm, ren, inp, scrW, scrH)
    {
        static std::vector<std::string> train_files_str;
        train_files_str = loadCSVFiles("dataset_CSVs");
        for(auto& f: train_files_str)
            train_files.push_back(f.c_str());
        
        static std::vector<std::string> weight_files_str;
        weight_files_str = loadCSVFiles("weights_CSVs");
        for(auto& f: weight_files_str)
            weight_files.push_back(f.c_str());
    }
    void update() final {
        GameTimer_t timer;

        if(gConfig.readFile && gConfig.data_filename != nullptr) {
            gConfig.readFile = false;
            SM.pushState<EditWeightsMenu_t>(SM, Render, Input, gConfig.data_filename, gConfig.editweight_filename, kSCRWIDTH, kSCRHEIGHT);
            m_Alive = false;
        }

        if(gConfig.exit) {
            m_Alive = false;
        }

        timer.timedCall("REN", [&](){ Render.getMenu().selectFileMenu2(gConfig, train_files, weight_files); });
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";
    }
private:
    std::vector<const char*> train_files;
    std::vector<const char*> weight_files;
};

struct SelectFileMenu_t : MenuState_t {
    explicit SelectFileMenu_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const uint32_t scrW, const uint32_t scrH)
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

        timer.timedCall("REN", [&](){ Render.getMenu().selectFileMenu(gConfig, files); });
        timer.timedCall("EXT", [&](){ timer.waitUntil_ns(NSPF); } );
        std::cout << "\n";
    }
private:
    std::vector<const char*> files;
};

struct MainMenu_t : MenuState_t {
    using GR = GameReferences;

    explicit MainMenu_t(StateManager_t& sm, const RenderSystem_t<ECS::EntityManager_t>& ren, InputSystem_t<ECS::EntityManager_t>& inp, const uint32_t scrW, const uint32_t scrH) 
    : MenuState_t(sm, ren, inp, scrW, scrH)
    {
        resetFilesPointers();
    }
    void update() final {
        GameTimer_t timer;

        if(gConfig.play) {
            ScoreboardSystem_t<ECS::EntityManager_t>::end_game = false;
            SM.pushState<GameManager_t>(SM, Render, Input, gConfig, GR::G_Playing|GR::G_WithWall, kSCRWIDTH, kSCRHEIGHT);
        }
        if(gConfig.arena) SM.pushState<GameManager_t>(SM, Render, Input, gConfig, GR::G_TrainLeft, kSCRWIDTH, kSCRHEIGHT);
        if(gConfig.train) SM.pushState<SelectFileMenu_t>(SM, Render, Input, kSCRWIDTH, kSCRHEIGHT);
        if(gConfig.editweights) SM.pushState<SelectFileMenu2_t>(SM, Render, Input, kSCRWIDTH, kSCRHEIGHT);
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
