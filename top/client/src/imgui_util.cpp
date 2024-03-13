#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>

#include "imgui_util.hpp"

void text_horizontally_center(std::string text) {
    float window_width = ImGui::GetWindowSize().x;
    float text_width = ImGui::CalcTextSize(text.c_str()).x;

    ImGui::SetCursorPosX((window_width - text_width) * 0.5f);
}

void text_vertically_center(std::string text) {
    float window_height = ImGui::GetWindowSize().y;
    float text_height = ImGui::CalcTextSize(text.c_str()).y;

    ImGui::SetCursorPosX((window_height - text_height) * 0.5f);
}

bool button_horizontally_center(std::string text, ImVec2 size)
{
    float window_width = ImGui::GetWindowSize().x;
    ImGui::SetCursorPosX((window_width - size.x) * 0.5f);

    return ImGui::Button(text.c_str(), size);
}