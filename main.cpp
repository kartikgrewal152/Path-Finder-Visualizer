#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include "Constants.hpp"
#include "Grid.hpp"
#include "Algorithms.hpp"
#include "UIManager.hpp"

int main() {
    sf::RenderWindow window(
        sf::VideoMode(Config::WINDOW_WIDTH, Config::WINDOW_HEIGHT),
        "Path Finder Visualizer  -  Dijkstra / A*  (C++ / SFML / ImGui)",
        sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window)) return -1;
    UIManager::applyDarkTheme();

    Grid grid;
    UIState uiState;

    AlgoResult currentResult;
    size_t animIndex = 0;
    size_t pathIndex = 0;
    bool isRunning = false;      // an algorithm animation is currently playing
    bool showingPath = false;    // exploration finished, now animating the path
    bool searchAttempted = false;

    sf::Clock deltaClock;
    sf::Clock stepClock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(window, event);

            if (event.type == sf::Event::Closed)
                window.close();

            const bool mouseOverUI = ImGui::GetIO().WantCaptureMouse;

            if (!mouseOverUI && !isRunning && event.type == sf::Event::MouseButtonPressed) {
                const int mx = event.mouseButton.x;
                const int my = event.mouseButton.y;
                const int c = mx / Config::CELL_SIZE;
                const int r = my / Config::CELL_SIZE;

                if (grid.inBounds(r, c)) {
                    const bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                                        sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
                    const bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                                       sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);
                    const bool alt = sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) ||
                                      sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt);

                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (ctrl) {
                            grid.setStart(r, c);
                            uiState.startRowInput = r;
                            uiState.startColInput = c;
                        } else if (alt) {
                            grid.setEnd(r, c);
                            uiState.endRowInput = r;
                            uiState.endColInput = c;
                        } else {
                            grid.handleLeftClick(r, c, shift, uiState.weightMode);
                        }
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        grid.handleRightClick(r, c);
                    }
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        UIManager::draw(uiState, isRunning, currentResult.found, searchAttempted);

        // ---- React to sidebar requests ----
        if (uiState.requestSetStart) {
            grid.setStart(uiState.startRowInput, uiState.startColInput);
            uiState.requestSetStart = false;
        }
        if (uiState.requestSetEnd) {
            grid.setEnd(uiState.endRowInput, uiState.endColInput);
            uiState.requestSetEnd = false;
        }
        if (uiState.requestClearPath) {
            grid.clearPathVisuals();
            isRunning = false;
            showingPath = false;
            searchAttempted = false;
            uiState.requestClearPath = false;
        }
        if (uiState.requestClearWalls) {
            grid.clearWalls();
            uiState.requestClearWalls = false;
        }
        if (uiState.requestReset) {
            grid.resetBoard();
            isRunning = false;
            showingPath = false;
            searchAttempted = false;
            currentResult = AlgoResult{};
            uiState.requestReset = false;
        }
        if (uiState.requestRun && !isRunning) {
            grid.clearPathVisuals();
            const AlgoType type = (uiState.algoIndex == 0) ? AlgoType::Dijkstra : AlgoType::AStar;
            currentResult = Algorithms::run(grid, type);
            animIndex = 0;
            pathIndex = 0;
            showingPath = false;
            searchAttempted = true;
            isRunning = !currentResult.steps.empty();
            stepClock.restart();
            uiState.requestRun = false;
        }

        // ---- Step-by-step animation ----
        if (isRunning && !showingPath) {
            if (stepClock.getElapsedTime().asSeconds() >= uiState.stepDelay) {
                stepClock.restart();
                if (animIndex < currentResult.steps.size()) {
                    for (auto& [r, c] : currentResult.steps[animIndex].visitedNodes) {
                        Node& n = grid.at(r, c);
                        if (n.state != Config::NodeState::Start && n.state != Config::NodeState::End)
                            n.state = Config::NodeState::Visited;
                    }
                    ++animIndex;
                } else {
                    showingPath = true;
                    pathIndex = 0;
                }
            }
        } else if (isRunning && showingPath) {
            if (stepClock.getElapsedTime().asSeconds() >= uiState.stepDelay) {
                stepClock.restart();
                if (pathIndex < currentResult.path.size()) {
                    auto [r, c] = currentResult.path[pathIndex];
                    Node& n = grid.at(r, c);
                    if (n.state != Config::NodeState::Start && n.state != Config::NodeState::End)
                        n.state = Config::NodeState::Path;
                    ++pathIndex;
                } else {
                    isRunning = false;
                }
            }
        }

        window.clear(Config::COL_BG);
        grid.draw(window);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}
