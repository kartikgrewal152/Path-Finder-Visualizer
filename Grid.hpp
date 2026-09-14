#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include "Node.hpp"

// Owns the 2D board of Nodes, renders it, and applies user edits
// (walls, weights, start/end placement, clearing).
class Grid {
public:
    Grid();

    void draw(sf::RenderWindow& window) const;

    Node&       at(int r, int c);
    const Node& at(int r, int c) const;
    bool inBounds(int r, int c) const;

    // shiftHeld == true -> place a weighted node (weightToPlace = 5 or 10)
    // shiftHeld == false -> place a wall
    void handleLeftClick(int r, int c, bool shiftHeld, int weightToPlace);
    void handleRightClick(int r, int c); // erase back to Empty (or clears start/end)

    void setStart(int r, int c);
    void setEnd(int r, int c);

    std::pair<int, int> getStart() const { return {startRow, startCol}; }
    std::pair<int, int> getEnd()   const { return {endRow, endCol}; }
    bool hasStart() const { return startRow != -1; }
    bool hasEnd()   const { return endRow != -1; }

    void clearPathVisuals(); // Visited/Frontier/Path -> Empty, keeps walls & weights
    void clearWalls();       // removes walls & weighted nodes only
    void resetBoard();       // full wipe: walls, weights, start, end

    void resetPathData();    // clears algorithm bookkeeping (distance, parent, ...)

private:
    std::vector<std::vector<Node>> cells;
    int startRow = -1, startCol = -1;
    int endRow   = -1, endCol   = -1;

    sf::Color colorFor(const Node& n) const;
};
