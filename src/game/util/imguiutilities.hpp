#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <game/util/gameconfig.hpp>

struct ImGuiUtilities {
    explicit ImGuiUtilities(uint32_t w, uint32_t h, uint32_t* framebuffer);
    ~ImGuiUtilities();

    void mainMenu(GameConfig& gConfig) const noexcept;
    void prerender() const noexcept;
    void postrender() const noexcept;
    void renderFrameBuffer() const noexcept;

private:
    void reverseYAxis() const noexcept;
    void renderUI() const noexcept;
    
    const uint32_t m_w {0}, m_h{0};
    uint32_t* m_framebuffer {nullptr};
    GLFWwindow* m_window { nullptr };
};
