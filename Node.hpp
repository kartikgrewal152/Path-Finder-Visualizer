#pragma once
#include "Constants.hpp"

// A single grid cell. Holds both visual state and the bookkeeping fields
// used by the pathfinding algorithms (distance / heuristic score).
struct Node {
    int row = 0;
    int col = 0;

    Config::NodeState state = Config::NodeState::Empty;
    int weight = 1; // traversal cost: 1 = normal, 5 / 10 = weighted terrain

    // Algorithm bookkeeping (reset before every run)
    double distance = 1e18; // g-score (Dijkstra) / g-score (A*)
    double fScore    = 1e18; // f = g + h, used by A*
    bool   visited   = false;
    bool   inQueue   = false;
    int    parentRow = -1;
    int    parentCol = -1;

    bool isWalkable() const { return state != Config::NodeState::Wall; }

    void resetPathData() {
        distance  = 1e18;
        fScore    = 1e18;
        visited   = false;
        inQueue   = false;
        parentRow = -1;
        parentCol = -1;
    }
};
