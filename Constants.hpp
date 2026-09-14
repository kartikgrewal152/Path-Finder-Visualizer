#pragma once
#include <SFML/Graphics/Color.hpp>

// All tunable constants live here. Grid size is NOT hardcoded elsewhere -
// change GRID_ROWS / GRID_COLS / CELL_SIZE and everything else adapts.
namespace Config {

    constexpr int GRID_ROWS = 40;
    constexpr int GRID_COLS = 50;
    constexpr int CELL_SIZE = 18;      // pixels per cell
    constexpr int SIDEBAR_WIDTH = 340; // ImGui control panel width

    constexpr int WINDOW_WIDTH  = GRID_COLS * CELL_SIZE + SIDEBAR_WIDTH;
    constexpr int WINDOW_HEIGHT = GRID_ROWS * CELL_SIZE;

    constexpr float DEFAULT_STEP_DELAY = 0.006f; // seconds between animation frames

    enum class NodeState {
        Empty,
        Wall,
        Start,
        End,
        Weight5,
        Weight10,
        Visited,
        Frontier,
        Path
    };

    // ---- Dark theme palette (Google-Maps-routing-simulator inspired) ----
    inline const sf::Color COL_BG(24, 26, 32);
    inline const sf::Color COL_EMPTY(43, 45, 54);
    inline const sf::Color COL_GRID_LINE(58, 61, 72);
    inline const sf::Color COL_WALL(15, 15, 18);
    inline const sf::Color COL_START(46, 204, 113);
    inline const sf::Color COL_END(231, 76, 60);
    inline const sf::Color COL_WEIGHT5(241, 196, 15);
    inline const sf::Color COL_WEIGHT10(230, 126, 34);
    inline const sf::Color COL_VISITED(93, 173, 226);
    inline const sf::Color COL_FRONTIER(133, 193, 233);
    inline const sf::Color COL_PATH(255, 235, 59);

} // namespace Config
