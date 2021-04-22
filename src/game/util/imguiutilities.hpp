#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

#include <game/util/gameconfig.hpp>

//Forward declaration
struct AItrainer_t;

struct ImGuiUtilities {
    explicit ImGuiUtilities(uint32_t w, uint32_t h, uint32_t* framebuffer);
    ~ImGuiUtilities();

    void mainMenu(GameConfig& gConfig, const std::vector<const char*>& files) const noexcept;
    void trainMenu_selectFile(GameConfig& gConfig, const std::vector<const char*>& files) const noexcept;
    void trainMenu_selectFilters(GameConfig& gConfig, const std::vector<float>& values, const AItrainer_t& AI) const noexcept;
    void saveFileMenu(GameConfig& gConfig) const noexcept;
    void prerender() const noexcept;
    void postrender() const noexcept;
    void renderFrameBuffer() const noexcept;
    const bool shouldClose() const noexcept { return glfwWindowShouldClose(m_window); }

private:
    void HelpMarker(const char* desc) const noexcept;
    void reverseYAxis() const noexcept;
    void renderWithoutFlipFrameBuffer() const noexcept;
    
    const uint32_t m_w {0}, m_h{0};
    uint32_t* m_framebuffer {nullptr};
    GLFWwindow* m_window { nullptr };
};
