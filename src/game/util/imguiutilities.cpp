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

void ImGuiUtilities::mainMenu(GameConfig& gConfig, const std::vector<const char*>& files) const noexcept {
    prerender();

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImVec2(m_w, m_h));
    ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    ImGui::BeginTable("Menu table", 3);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Checkbox("Left player with AI", &gConfig.Lplayer_AI);
    if(gConfig.Lplayer_AI) {
        if(files.size() > 0) {
            static int Lplayer_file = 0;
            ImGui::ListBox("##Left weight files", &Lplayer_file, files.data(), files.size(), 4);
            gConfig.Lplayer_AI_file = files[Lplayer_file];
        } else {
            ImGui::Text("Must train first.");
        }
    }

    ImGui::TableSetColumnIndex(2);
    ImGui::Checkbox("Right player with AI", &gConfig.Rplayer_AI);
    if(gConfig.Rplayer_AI) {
        if(files.size() > 0) {
            static int Rplayer_file = 0;
            ImGui::ListBox("##Right weight files", &Rplayer_file, files.data(), files.size(), 4);
            gConfig.Rplayer_AI_file = files[Rplayer_file];
        } else {
            ImGui::Text("Must train first.");
        }
    }

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
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    gConfig.exit = ImGui::Button("Exit");

    ImGui::EndTable();
    ImGui::End();

    renderWithoutFlipFrameBuffer();
    postrender();
}

void ImGuiUtilities::trainMenu_selectFile(GameConfig& gConfig, const std::vector<const char*>& files) const noexcept {
    prerender();

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImVec2(m_w, m_h));
    ImGui::Begin("Training menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    ImGui::BeginTable("Menu table", 3);
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    static int item_current = 0;
    ImGui::ListBox("Training files", &item_current, files.data(), files.size(), 4);
    
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(1);
    if( ImGui::Button("Train with this file") ) {
        gConfig.data_filename = files[item_current];
        gConfig.readFile = true;
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

void ImGuiUtilities::trainMenu_selectFilters(GameConfig& gConfig, const std::vector<float>& values, const AItrainer_t& AI) const noexcept {
    prerender();

    ImGui::SetNextWindowPos(ImVec2(0.0, 0.0));
    ImGui::SetNextWindowSize(ImVec2(m_w, m_h));
    ImGui::Begin("Training menu", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    
    static bool change_importance = false;
    ImGui::Checkbox("Change importance of data", &change_importance);
    ImGui::SameLine(); HelpMarker(
        "The dataset is usually full of not pressing key examples. You may want to change the importance of pressing a key, doing it can cause bigger error but better results in game.");

    {
        uint32_t data_without_touch, data_with_up, data_with_down, total;
        AI.infoFromData(data_without_touch, data_with_up, data_with_down, total);
        ImGui::Text("File selected: %s, with %u lines of example.", gConfig.data_filename, total);

        ImGui::BeginTable("Dataset table", 3);

        ImGui::TableSetupColumn("Examples with no touch");
        ImGui::TableSetupColumn("Examples with up touch");
        ImGui::TableSetupColumn("Examples with down touch");
        ImGui::TableHeadersRow();

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("%u(%.2f%)", data_without_touch, (float)data_without_touch/(float)total*100);
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%u(%.2f%)", data_with_up, (float)data_with_up/(float)total*100);
        ImGui::TableSetColumnIndex(2);
        ImGui::Text("%u(%.2f%)", data_with_down, (float)data_with_down/(float)total*100);

        if( change_importance ) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::VSliderFloat("##no touch", ImVec2(30, 120), &gConfig.no_touch_importance, 0.01f, 1.0f, "");
            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                ImGui::SetTooltip("%.2f", gConfig.no_touch_importance);

            ImGui::TableSetColumnIndex(1);
            ImGui::VSliderFloat("##up touch", ImVec2(30, 120), &gConfig.up_touch_importance, 0.01f, 1.0f, "");
            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                ImGui::SetTooltip("%.2f", gConfig.up_touch_importance);

            ImGui::TableSetColumnIndex(2);
            ImGui::VSliderFloat("##down touch", ImVec2(30, 120), &gConfig.down_touch_importance, 0.01f, 1.0f, "");
            if (ImGui::IsItemActive() || ImGui::IsItemHovered())
                ImGui::SetTooltip("%.2f", gConfig.down_touch_importance);
        }
        ImGui::EndTable();
    }
    {
        ImGui::InputInt("Number of iterations", &gConfig.n_iter);
        ImGui::InputInt("Number of hidden layers", &gConfig.n_layers);
        ImGui::InputInt("Number of neurons per layer", &gConfig.neuron_per_layer);
        ImGui::SliderFloat("Learning rate", &gConfig.learning_rate, 0.0f, 2.0f, "learning rate -> %.3f");
        ImGui::PlotLines("% of error", values.data(), values.size(), 0, NULL, 0.0f, 1.0f, ImVec2(0, 100.0f));
    }
    {
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