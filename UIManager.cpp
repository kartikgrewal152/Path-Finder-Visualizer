#include "UIManager.hpp"
#include "imgui.h"

void UIManager::applyDarkTheme() {
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 6.0f;
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.WindowBorderSize = 0.0f;

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg]        = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_ChildBg]         = ImVec4(0.10f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_FrameBg]         = ImVec4(0.18f, 0.19f, 0.22f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]  = ImVec4(0.24f, 0.25f, 0.29f, 1.00f);
    colors[ImGuiCol_FrameBgActive]   = ImVec4(0.28f, 0.29f, 0.34f, 1.00f);
    colors[ImGuiCol_Button]          = ImVec4(0.20f, 0.45f, 0.85f, 1.00f);
    colors[ImGuiCol_ButtonHovered]   = ImVec4(0.26f, 0.53f, 0.95f, 1.00f);
    colors[ImGuiCol_ButtonActive]    = ImVec4(0.16f, 0.38f, 0.72f, 1.00f);
    colors[ImGuiCol_Header]          = ImVec4(0.20f, 0.45f, 0.85f, 0.6f);
    colors[ImGuiCol_Separator]       = ImVec4(0.30f, 0.31f, 0.35f, 1.00f);
    colors[ImGuiCol_SliderGrab]      = ImVec4(0.26f, 0.53f, 0.95f, 1.00f);
    colors[ImGuiCol_CheckMark]       = ImVec4(0.26f, 0.53f, 0.95f, 1.00f);
    colors[ImGuiCol_Text]            = ImVec4(0.90f, 0.90f, 0.93f, 1.00f);
}

void UIManager::draw(UIState& state, bool isRunning, bool pathFound, bool searchAttempted) {
    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(Config::GRID_COLS * Config::CELL_SIZE), 0.0f));
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(Config::SIDEBAR_WIDTH), static_cast<float>(Config::WINDOW_HEIGHT)));

    ImGui::Begin("Controls", nullptr,
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

    ImGui::Dummy(ImVec2(0, 4));
    ImGui::TextColored(ImVec4(0.35f, 0.65f, 1.0f, 1.0f), "PATH FINDER");
    ImGui::TextColored(ImVec4(0.6f, 0.6f, 0.65f, 1.0f), "Routing Simulator - C++/SFML/ImGui");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));

    // --- Algorithm selection ---
    ImGui::Text("Algorithm");
    const char* algos[] = {"Dijkstra's Algorithm", "A* Search (Manhattan)"};
    ImGui::SetNextItemWidth(-1);
    ImGui::Combo("##algo", &state.algoIndex, algos, IM_ARRAYSIZE(algos));

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));

    // --- Coordinate input ---
    ImGui::Text("Start Node  (row / col)");
    ImGui::SetNextItemWidth(90);
    ImGui::InputInt("##sr", &state.startRowInput);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(90);
    ImGui::InputInt("##sc", &state.startColInput);
    ImGui::SameLine();
    if (ImGui::Button("Set##start")) state.requestSetStart = true;

    ImGui::Dummy(ImVec2(0, 4));

    ImGui::Text("End Node  (row / col)");
    ImGui::SetNextItemWidth(90);
    ImGui::InputInt("##er", &state.endRowInput);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(90);
    ImGui::InputInt("##ec", &state.endColInput);
    ImGui::SameLine();
    if (ImGui::Button("Set##end")) state.requestSetEnd = true;

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));

    // --- Weight toggle ---
    ImGui::Text("Toggle Weight  (hold Shift + Click)");
    ImGui::RadioButton("Cost 5", &state.weightMode, 5);
    ImGui::SameLine();
    ImGui::RadioButton("Cost 10", &state.weightMode, 10);

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));

    ImGui::Text("Animation Speed");
    ImGui::SetNextItemWidth(-1);
    ImGui::SliderFloat("##speed", &state.stepDelay, 0.0005f, 0.03f, "%.4f s/step");

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));

    // --- Action buttons ---
    ImGui::BeginDisabled(isRunning);
    if (ImGui::Button(state.algoIndex == 0 ? "Start Dijkstra" : "Start A* Search", ImVec2(-1, 42)))
        state.requestRun = true;
    ImGui::EndDisabled();

    ImGui::Dummy(ImVec2(0, 6));
    if (ImGui::Button("Clear Path", ImVec2(-1, 0))) state.requestClearPath = true;
    if (ImGui::Button("Clear Walls / Weights", ImVec2(-1, 0))) state.requestClearWalls = true;
    if (ImGui::Button("Reset Board", ImVec2(-1, 0))) state.requestReset = true;

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Separator();

    if (searchAttempted) {
        ImGui::Dummy(ImVec2(0, 6));
        if (pathFound)
            ImGui::TextColored(ImVec4(0.35f, 0.85f, 0.45f, 1.0f), "Path found!");
        else
            ImGui::TextColored(ImVec4(0.9f, 0.35f, 0.3f, 1.0f), "No path exists.");
    }

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));
    ImGui::Text("Legend");

    auto swatch = [](const char* label, ImVec4 col) {
        ImGui::ColorButton(label, col, ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoBorder, ImVec2(14, 14));
        ImGui::SameLine();
        ImGui::Text("%s", label);
    };
    swatch("Start", ImVec4(0.18f, 0.80f, 0.44f, 1));
    swatch("End", ImVec4(0.90f, 0.30f, 0.24f, 1));
    swatch("Wall", ImVec4(0.08f, 0.08f, 0.10f, 1));
    swatch("Weight 5", ImVec4(0.95f, 0.77f, 0.06f, 1));
    swatch("Weight 10", ImVec4(0.90f, 0.49f, 0.13f, 1));
    swatch("Exploring", ImVec4(0.36f, 0.68f, 0.90f, 1));
    swatch("Shortest Path", ImVec4(1.0f, 0.92f, 0.23f, 1));

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 6));
    ImGui::TextWrapped("Left Click: Wall  |  Shift+Left Click: Weight");
    ImGui::TextWrapped("Ctrl+Left Click: Set Start  |  Alt+Left Click: Set End");
    ImGui::TextWrapped("Right Click: Erase Cell");

    ImGui::End();
}
