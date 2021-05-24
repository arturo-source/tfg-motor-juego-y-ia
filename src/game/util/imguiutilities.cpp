#include <game/util/imguiutilities.hpp>
#include <imgui/src/imgui.h>
#include <imgui/src/imgui_impl_glfw.h>
#include <imgui/src/imgui_impl_opengl3.h>
#include <cstdio>
#include <stdexcept>
#include <game/util/aitrainer.hpp>

ImGuiUtilities::ImGuiUtilities(uint32_t w, uint32_t h, uint32_t* framebuffer)
: m_w{w}, m_h{h}, m_framebuffer{framebuffer}
{
    // Setup window
    glfwSetErrorCallback([](auto error, auto description){
        std::fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        throw std::runtime_error("GLFW Error");
    });
    if (!glfwInit()) throw std::runtime_error("GLFW Error");

    // GL 3.0 + GLSL 130
    constexpr const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    // Create window with graphics context
    m_window = glfwCreateWindow(m_w, m_h, "Pong Game", NULL, NULL);
    if (m_window == nullptr) throw std::runtime_error("GLFW Error creating window");
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1); // Enable vsync

    // Init OpenGL Loader
    if(glewInit() != GLEW_OK) throw std::runtime_error("Failed to initialize OpenGL loader!\n");

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

ImGuiUtilities::~ImGuiUtilities() {
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void ImGuiUtilities::mainMenu(GameConfig& gConfig) const noexcept {
    prerender();

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImVec2(m_w, m_h));
    ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    ImGui::BeginTable("Menu table", 3);

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    gConfig.play = ImGui::Button("Play");
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    gConfig.arena = ImGui::Button("Arena");
    ImGui::SameLine(); HelpMarker(
            "Arena is used to get data assets to be able to train your neural network.");
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    gConfig.train = ImGui::Button("Train");
    ImGui::SameLine(); HelpMarker(
            "Train is where you take dataset to apply backpropagation.");
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    gConfig.editweights = ImGui::Button("Edit neural network");
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    gConfig.exit = ImGui::Button("Exit");

    ImGui::EndTable();
    ImGui::End();

    renderWithoutFlipFrameBuffer();
    postrender();
}

void ImGuiUtilities::editweightsMenu(GameConfig& gConfig, AItrainer_t& ai) const noexcept {
    prerender();

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImVec2(m_w, m_h));
    ImGui::Begin("Editing menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

    static int32_t layer_pos { 0 };
    ImGui::Text("Editing layer %d", layer_pos);
    ImGui::SameLine();
    if( ImGui::Button("Previous") ) {
        layer_pos = (layer_pos - 1)%ai.nn.m_layers.size();
    }
    ImGui::SameLine();
    if( ImGui::Button("Next") ) {
        layer_pos = (layer_pos + 1)%ai.nn.m_layers.size();
    }

    float failed_press_up, failed_press_down, failed_nopress;
    ai.failed_keys(failed_press_up, failed_press_down, failed_nopress);

    Layer_t& layer = ai.nn.m_layers[layer_pos];

    size_t id_num = 0;
    for(size_t i = 0; i < layer.m_neurons.size(); ++i) {
        ImGui::Text("Press up fail %.3f%% | Press down fail %.3f%% | No press fail %.3f%%", failed_press_up*100, failed_press_down*100, failed_nopress*100);
        ImGui::Text("Neuron %d", i);
        for(auto& w: layer.m_neurons[i].m_weights) {
            float w_fake = w;
            std::string id {"##" + std::to_string(id_num++)};
            if( ImGui::SliderFloat(id.c_str(), &w_fake, -2.5f, 2.5f, "%.3f") ) 
                w = w_fake;
        }
    }

    gConfig.saveFile = ImGui::Button("Save");
    ImGui::SameLine();
    gConfig.exit = ImGui::Button("Exit");

    ImGui::End();

    renderWithoutFlipFrameBuffer();
    postrender();
}

void ImGuiUtilities::playModeMenu(GameConfig& gConfig, const std::vector<const char*>& files, const bool train) const noexcept {
    prerender();

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImVec2(m_w, m_h));
    ImGui::Begin("Choose mode", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    ImGui::BeginTable("Mode table", 3);
    if(!train) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Checkbox("Choose left AI", &gConfig.Lplayer_AI);
        if(gConfig.Lplayer_AI) {
            if(files.size() > 0) {
                static int Lplayer_file = 0;
                ImGui::ListBox("##Left weight files", &Lplayer_file, files.data(), files.size(), 8);
                gConfig.Lplayer_AI_file = files[Lplayer_file];
            } else {
                ImGui::Text("Must train if want choose AI.");
            }
        }

        ImGui::TableSetColumnIndex(2);
        ImGui::Checkbox("Choose right AI", &gConfig.Rplayer_AI);
        if(gConfig.Rplayer_AI) {
            if(files.size() > 0) {
                static int Rplayer_file = 0;
                ImGui::ListBox("##Right weight files", &Rplayer_file, files.data(), files.size(), 8);
                gConfig.Rplayer_AI_file = files[Rplayer_file];
            } else {
                ImGui::Text("Must train if want choose AI.");
            }
        }
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Default AI plays");
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("if you don't choose any weight file.");
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(1);
        gConfig.play = ImGui::Button("Play");
    } else {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(1);
        gConfig.trainLside = ImGui::Button("Train as left player");
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(1);
        gConfig.trainRside = ImGui::Button("Train as right player");
        // ImGui::TableNextRow();
        // ImGui::TableSetColumnIndex(1);
        // gConfig.play = ImGui::Button("Train as left minion");
        // ImGui::TableNextRow();
        // ImGui::TableSetColumnIndex(1);
        // gConfig.play = ImGui::Button("Train as right minion");
    }

    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    gConfig.exit = ImGui::Button("Back");

    ImGui::EndTable();
    ImGui::End();

    renderWithoutFlipFrameBuffer();
    postrender();
}

void ImGuiUtilities::selectFileMenu(GameConfig& gConfig, const std::vector<const char*>& files) const noexcept {
    prerender();

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImVec2(m_w, m_h));
    ImGui::Begin("Select menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    ImGui::BeginTable("Menu table", 3);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    static int item_current = 0;
    if(files.size() > 0)
        ImGui::ListBox("Select a file", &item_current, files.data(), files.size(), 8);
    else
        ImGui::Text("Must play arena first.");
    
    if(files.size() > 0) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(1);
        if( ImGui::Button("Select") ) {
            gConfig.data_filename = files[item_current];
            gConfig.readFile = true;
        }
    }
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    gConfig.exit = ImGui::Button("Exit");

    ImGui::EndTable();
    ImGui::End();

    renderWithoutFlipFrameBuffer();
    postrender();
}

void ImGuiUtilities::selectFileMenu2(GameConfig& gConfig, const std::vector<const char*>& train_files, const std::vector<const char*>& weight_files) const noexcept {
    prerender();

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImVec2(m_w, m_h));
    ImGui::Begin("Select menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    ImGui::BeginTable("Menu table", 3);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    static int train_item_current = 0;
    if(train_files.size() > 0)
        ImGui::ListBox("Select dataset file", &train_item_current, train_files.data(), train_files.size(), 8);
    else
        ImGui::Text("Must play arena first.");
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    static int weight_item_current = 0;
    if(weight_files.size() > 0)
        ImGui::ListBox("Select weights file", &weight_item_current, weight_files.data(), weight_files.size(), 8);
    else
        ImGui::Text("Must train first.");

    if(train_files.size() > 0 && weight_files.size() > 0) {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(1);
        if( ImGui::Button("Select") ) {
            gConfig.data_filename   = train_files[train_item_current];
            gConfig.editweight_filename = weight_files[weight_item_current];
            gConfig.readFile = true;
        }
    }
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    gConfig.exit = ImGui::Button("Exit");

    ImGui::EndTable();
    ImGui::End();

    renderWithoutFlipFrameBuffer();
    postrender();
}

void ImGuiUtilities::saveFileMenu(GameConfig& gConfig) const noexcept {
    prerender();

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImVec2(m_w, m_h));
    ImGui::Begin("Save file", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    ImGui::BeginTable("Save file table", 3);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    ImGui::InputText("Name of weights file", gConfig.weight_filename, IM_ARRAYSIZE(gConfig.weight_filename));
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    gConfig.saveFile = ImGui::Button("Save file");
    
    ImGui::EndTable();
    ImGui::End();

    renderWithoutFlipFrameBuffer();
    postrender();
}

void ImGuiUtilities::trainMenu_selectFilters(GameConfig& gConfig, const std::vector<float>& failed_press_up, const std::vector<float>& failed_press_down, const std::vector<float>& failed_nopress, const AItrainer_t& AI) const noexcept {
    prerender();

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImVec2(m_w, m_h));
    ImGui::Begin("Training menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    static bool change_importance = false;
    static bool show_graphics = false;
    ImGui::Checkbox("Change importance of data", &change_importance);
    ImGui::SameLine(); HelpMarker(
        "The dataset is usually full of not pressing key examples. You may want to change the importance of pressing a key, doing it can cause bigger error but better results in game.");

    {
        if(gConfig.n_iter <= 0) gConfig.n_iter = 1;
        if(gConfig.n_layers <= 0) gConfig.n_layers = 0;
        if(gConfig.neuron_per_layer <= 0) gConfig.neuron_per_layer = 0;
        
        ImGui::InputInt("Number of iterations", &gConfig.n_iter);
        ImGui::SameLine(); HelpMarker(
            "Arround 10000 iterations work ok.");
        ImGui::SliderInt("Number of hidden layers", &gConfig.n_layers, 1, 10);
        ImGui::SameLine(); HelpMarker(
            "Arround 3 layers work ok.");
        ImGui::SliderInt("Number of neurons per layer", &gConfig.neuron_per_layer, 1, 10);
        ImGui::SameLine(); HelpMarker(
            "Arround 6 neurons per layer work ok.");
        ImGui::SliderFloat("Learning rate", &gConfig.learning_rate, 0.0001f, 1.0f, "learning rate -> %.5f", ImGuiSliderFlags_Logarithmic);
        ImGui::SameLine(); HelpMarker(
            "Learning rate lower than 0.001 work better.");
    }
    {
        constexpr uint32_t input_neurons {14};
        constexpr uint32_t output_neurons {2};
        uint32_t total_weights { 0 };
        total_weights += input_neurons*gConfig.neuron_per_layer;
        for(size_t i = 0; i < gConfig.n_layers-1; ++i) {
            total_weights += gConfig.neuron_per_layer*gConfig.neuron_per_layer;
        }
        total_weights += gConfig.neuron_per_layer*output_neurons;
        ImGui::Text("Actual weights are %d input neuron, %d hidden layer, %d neuron per each hidden layer, and %d output neuron. Total %d weights.", input_neurons, gConfig.n_layers, gConfig.neuron_per_layer, output_neurons, total_weights);
        ImGui::Text("You'll need %d examples from each learnable action (up, down and no press) to be able to make the neural network learn.", total_weights*10);
    }
    {
        uint32_t data_without_touch, data_with_up, data_with_down, total;
        AI.infoFromData(data_without_touch, data_with_up, data_with_down, total);
        ImGui::Text("File selected: %s, with %u lines of example.", gConfig.data_filename, total);

        ImGui::BeginTable("Dataset table", 3);

        ImGui::TableSetupColumn("Up touch");
        ImGui::TableSetupColumn("Down touch");
        ImGui::TableSetupColumn("No touch");
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%u(%.2f%%)", data_with_up, (float)data_with_up/(float)total*100);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u(%.2f%%)", data_with_down, (float)data_with_down/(float)total*100);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%u(%.2f%%)", data_without_touch, (float)data_without_touch/(float)total*100);

        if( change_importance ) {
            float total_percentage = gConfig.no_touch_importance + gConfig.up_touch_importance + gConfig.down_touch_importance;
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::VSliderFloat("##up touch", ImVec2(40, 110), &gConfig.up_touch_importance, 0.01f, 1.0f, "");
            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                ImGui::SetTooltip("%.2f%% of train dataset is up touch", gConfig.up_touch_importance/total_percentage*100);

            ImGui::TableSetColumnIndex(1);
            ImGui::VSliderFloat("##down touch", ImVec2(40, 110), &gConfig.down_touch_importance, 0.01f, 1.0f, "");
            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                ImGui::SetTooltip("%.2f%% of train dataset is down touch", gConfig.down_touch_importance/total_percentage*100);
            
            ImGui::TableSetColumnIndex(2);
            ImGui::VSliderFloat("##no touch", ImVec2(40, 110), &gConfig.no_touch_importance, 0.01f, 1.0f, "");
            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                ImGui::SetTooltip("%.2f%% of train dataset is no touch", gConfig.no_touch_importance/total_percentage*100);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%.0f samples used in train", gConfig.up_touch_importance/total_percentage*total);
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.0f samples used in train", gConfig.down_touch_importance/total_percentage*total);
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%.0f samples used in train", gConfig.no_touch_importance/total_percentage*total);
        }
        ImGui::EndTable();
    }
    {
        ImGui::PlotLines("% of error pressing up key", failed_press_up.data(), failed_press_up.size(), 0, NULL, 0.0f, 1.0f, ImVec2(0, 100.0f));
        ImGui::PlotLines("% of error pressing down key", failed_press_down.data(), failed_press_down.size(), 0, NULL, 0.0f, 1.0f, ImVec2(0, 100.0f));
        ImGui::PlotLines("% of error no pressing key", failed_nopress.data(), failed_nopress.size(), 0, NULL, 0.0f, 1.0f, ImVec2(0, 100.0f));

        ImGui::BeginTable("Menu table", 3);
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(1);
        gConfig.setData = ImGui::Button("Train with this parameters");
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(1);
        gConfig.saveFile = ImGui::Button("Save data of neural network");
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(1);
        gConfig.exit = ImGui::Button("Exit");

        ImGui::EndTable();
    }

    ImGui::End();

    renderWithoutFlipFrameBuffer();
    postrender();
}

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see docs/FONTS.md)
void ImGuiUtilities::HelpMarker(const char* desc) const noexcept {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void ImGuiUtilities::renderScoreboard(const uint32_t left_score, const uint32_t right_score) const noexcept {
    constexpr float window_width  = 250;
    constexpr float window_height = 100;
    ImGui::SetNextWindowPos(ImVec2((m_w-window_width)/2, 0.0));
    ImGui::SetNextWindowSize(ImVec2(window_width, window_height));
    ImGui::Begin("Scoreboard", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
    
    ImGui::BeginTable("Score table", 2);

    ImGui::TableSetupColumn("Left team score");
    ImGui::TableSetupColumn("Right team score");
    ImGui::TableHeadersRow();

    ImGui::Indent(100);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("%d", left_score);
    ImGui::TableSetColumnIndex(1);
    ImGui::Text("%d", right_score);

    ImGui::EndTable();
    ImGui::End();
}

void ImGuiUtilities::prerender() const noexcept {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiUtilities::postrender() const noexcept {
    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(m_window);
}

void ImGuiUtilities::reverseYAxis() const noexcept {
    uint32_t buffer_size = m_w*m_h;
    uint32_t* buffer { m_framebuffer };
    for(std::size_t i = 0; i<m_h/2; ++i) {
        for(std::size_t j = 0; j<m_w; ++j) {
            std::swap(buffer[i*m_w + j], buffer[buffer_size - i*m_w + j]);
        }
    }
}

void ImGuiUtilities::renderWithoutFlipFrameBuffer() const noexcept {
    glWindowPos2i(0,0); 
    glDrawPixels(m_w, m_h, GL_BGRA, GL_UNSIGNED_BYTE, m_framebuffer);
    /*int display_w, display_h;
    glfwGetFramebufferSize(m_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);*/
}

void ImGuiUtilities::renderFrameBuffer() const noexcept {
    reverseYAxis();
    renderWithoutFlipFrameBuffer();
}