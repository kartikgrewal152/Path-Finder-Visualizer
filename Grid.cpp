#include "Grid.hpp"

Grid::Grid() {
    cells.assign(Config::GRID_ROWS, std::vector<Node>(Config::GRID_COLS));
    for (int r = 0; r < Config::GRID_ROWS; ++r)
        for (int c = 0; c < Config::GRID_COLS; ++c) {
            cells[r][c].row = r;
            cells[r][c].col = c;
        }
}

bool Grid::inBounds(int r, int c) const {
    return r >= 0 && r < Config::GRID_ROWS && c >= 0 && c < Config::GRID_COLS;
}

Node& Grid::at(int r, int c) { return cells[r][c]; }
const Node& Grid::at(int r, int c) const { return cells[r][c]; }

sf::Color Grid::colorFor(const Node& n) const {
    using namespace Config;
    switch (n.state) {
        case NodeState::Wall:     return COL_WALL;
        case NodeState::Start:    return COL_START;
        case NodeState::End:      return COL_END;
        case NodeState::Weight5:  return COL_WEIGHT5;
        case NodeState::Weight10: return COL_WEIGHT10;
        case NodeState::Visited:  return COL_VISITED;
        case NodeState::Frontier: return COL_FRONTIER;
        case NodeState::Path:     return COL_PATH;
        default:                  return COL_EMPTY;
    }
}

void Grid::draw(sf::RenderWindow& window) const {
    sf::RectangleShape rect(sf::Vector2f(
        static_cast<float>(Config::CELL_SIZE - 1),
        static_cast<float>(Config::CELL_SIZE - 1)));

    for (int r = 0; r < Config::GRID_ROWS; ++r) {
        for (int c = 0; c < Config::GRID_COLS; ++c) {
            rect.setPosition(static_cast<float>(c * Config::CELL_SIZE),
                              static_cast<float>(r * Config::CELL_SIZE));
            rect.setFillColor(colorFor(cells[r][c]));
            window.draw(rect);
        }
    }
}

void Grid::handleLeftClick(int r, int c, bool shiftHeld, int weightToPlace) {
    if (!inBounds(r, c)) return;
    Node& n = cells[r][c];
    if (n.state == Config::NodeState::Start || n.state == Config::NodeState::End) return;

    if (shiftHeld) {
        if (weightToPlace == 5) {
            n.state = Config::NodeState::Weight5;
            n.weight = 5;
        } else {
            n.state = Config::NodeState::Weight10;
            n.weight = 10;
        }
    } else {
        n.state = Config::NodeState::Wall;
        n.weight = 1;
    }
}

void Grid::handleRightClick(int r, int c) {
    if (!inBounds(r, c)) return;
    Node& n = cells[r][c];
    if (n.state == Config::NodeState::Start) { startRow = -1; startCol = -1; }
    if (n.state == Config::NodeState::End)   { endRow = -1;   endCol = -1; }
    n.state = Config::NodeState::Empty;
    n.weight = 1;
}

void Grid::setStart(int r, int c) {
    if (!inBounds(r, c)) return;
    if (cells[r][c].state == Config::NodeState::End) return;
    if (startRow != -1) {
        cells[startRow][startCol].state = Config::NodeState::Empty;
        cells[startRow][startCol].weight = 1;
    }
    cells[r][c].state = Config::NodeState::Start;
    cells[r][c].weight = 1;
    startRow = r; startCol = c;
}

void Grid::setEnd(int r, int c) {
    if (!inBounds(r, c)) return;
    if (cells[r][c].state == Config::NodeState::Start) return;
    if (endRow != -1) {
        cells[endRow][endCol].state = Config::NodeState::Empty;
        cells[endRow][endCol].weight = 1;
    }
    cells[r][c].state = Config::NodeState::End;
    cells[r][c].weight = 1;
    endRow = r; endCol = c;
}

void Grid::clearPathVisuals() {
    for (auto& row : cells)
        for (auto& n : row)
            if (n.state == Config::NodeState::Visited ||
                n.state == Config::NodeState::Frontier ||
                n.state == Config::NodeState::Path)
                n.state = Config::NodeState::Empty;
    resetPathData();
}

void Grid::clearWalls() {
    for (auto& row : cells)
        for (auto& n : row)
            if (n.state == Config::NodeState::Wall ||
                n.state == Config::NodeState::Weight5 ||
                n.state == Config::NodeState::Weight10) {
                n.state = Config::NodeState::Empty;
                n.weight = 1;
            }
}

void Grid::resetBoard() {
    cells.assign(Config::GRID_ROWS, std::vector<Node>(Config::GRID_COLS));
    for (int r = 0; r < Config::GRID_ROWS; ++r)
        for (int c = 0; c < Config::GRID_COLS; ++c) {
            cells[r][c].row = r;
            cells[r][c].col = c;
        }
    startRow = startCol = -1;
    endRow = endCol = -1;
}

void Grid::resetPathData() {
    for (auto& row : cells)
        for (auto& n : row)
            n.resetPathData();
}
