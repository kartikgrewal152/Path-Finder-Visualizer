#pragma once
#include "Grid.hpp"
#include <vector>
#include <utility>

enum class AlgoType { Dijkstra, AStar };

// One "wave" of the animation - the set of node(s) settled in a given step.
struct AlgoStep {
    std::vector<std::pair<int, int>> visitedNodes;
};

struct AlgoResult {
    std::vector<AlgoStep> steps;            // exploration animation, in order
    std::vector<std::pair<int, int>> path;  // shortest path start -> end (empty if none)
    bool found = false;
};

// Both algorithms build an adjacency list (std::vector<std::vector<int>>),
// explore with a std::priority_queue min-heap, and track parents with a
// std::unordered_map keyed by node id (row * COLS + col).
namespace Algorithms {
    AlgoResult run(Grid& grid, AlgoType type);
    AlgoResult dijkstra(Grid& grid);
    AlgoResult aStar(Grid& grid);
}
