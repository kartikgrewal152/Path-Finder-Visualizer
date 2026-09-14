#pragma once
#include "Constants.hpp"

// Shared UI state, read/written each frame by main.cpp and UIManager::draw.
struct UIState {
    int algoIndex = 0; // 0 = Dijkstra, 1 = A*

    int startRowInput = 5, startColInput = 5;
    int endRowInput   = 20, endColInput  = 40;

    int weightMode = 5; // 5 or 10, chosen via sidebar, applied with Shift+Click

    float stepDelay = Config::DEFAULT_STEP_DELAY;

    bool requestRun         = false;
    bool requestClearPath   = false;
    bool requestClearWalls  = false;
    bool requestReset       = false;
    bool requestSetStart    = false;
    bool requestSetEnd      = false;
};

namespace UIManager {
    // Draws the dark-themed sidebar; sets the request* flags in `state`
    // based on button presses for main.cpp to act on.
    void draw(UIState& state, bool isRunning, bool pathFound, bool searchAttempted);
    void applyDarkTheme();
}
