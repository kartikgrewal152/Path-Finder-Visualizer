#include "Algorithms.hpp"
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>

namespace {

struct PQItem {
    double priority;
    int row, col;
    bool operator>(const PQItem& o) const { return priority > o.priority; }
};

inline int idOf(int r, int c) { return r * Config::GRID_COLS + c; }

// Builds a std::vector adjacency list over walkable cells (4-directional).
std::vector<std::vector<int>> buildAdjacency(Grid& grid) {
    std::vector<std::vector<int>> adj(Config::GRID_ROWS * Config::GRID_COLS);
    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};

    for (int r = 0; r < Config::GRID_ROWS; ++r) {
        for (int c = 0; c < Config::GRID_COLS; ++c) {
            if (!grid.at(r, c).isWalkable()) continue;
            int id = idOf(r, c);
            for (int d = 0; d < 4; ++d) {
                int nr = r + dr[d], nc = c + dc[d];
                if (grid.inBounds(nr, nc) && grid.at(nr, nc).isWalkable())
                    adj[id].push_back(idOf(nr, nc));
            }
        }
    }
    return adj;
}

inline double heuristic(int r1, int c1, int r2, int c2) {
    // Manhattan distance - admissible for 4-directional movement.
    return static_cast<double>(std::abs(r1 - r2) + std::abs(c1 - c2));
}

std::vector<std::pair<int, int>> reconstructPath(
    std::unordered_map<int, std::pair<int, int>>& parent,
    int startR, int startC, int endR, int endC) {

    std::vector<std::pair<int, int>> path;
    if (startR == endR && startC == endC) {
        path.push_back({startR, startC});
        return path;
    }
    if (parent.find(idOf(endR, endC)) == parent.end()) return path;

    int r = endR, c = endC;
    path.push_back({r, c});
    while (!(r == startR && c == startC)) {
        auto it = parent.find(idOf(r, c));
        if (it == parent.end()) break;
        r = it->second.first;
        c = it->second.second;
        path.push_back({r, c});
    }
    std::reverse(path.begin(), path.end());
    return path;
}

} // namespace

AlgoResult Algorithms::run(Grid& grid, AlgoType type) {
    return type == AlgoType::Dijkstra ? dijkstra(grid) : aStar(grid);
}

AlgoResult Algorithms::dijkstra(Grid& grid) {
    AlgoResult result;
    if (!grid.hasStart() || !grid.hasEnd()) return result;

    grid.resetPathData();
    auto adj = buildAdjacency(grid);
    auto [sr, sc] = grid.getStart();
    auto [er, ec] = grid.getEnd();

    const int N = Config::GRID_ROWS * Config::GRID_COLS;
    std::vector<double> dist(N, 1e18);
    std::vector<bool> settled(N, false);
    std::unordered_map<int, std::pair<int, int>> parent;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;

    dist[idOf(sr, sc)] = 0.0;
    pq.push({0.0, sr, sc});

    while (!pq.empty()) {
        PQItem cur = pq.top();
        pq.pop();
        int id = idOf(cur.row, cur.col);
        if (settled[id]) continue;
        settled[id] = true;

        AlgoStep step;
        step.visitedNodes.push_back({cur.row, cur.col});
        result.steps.push_back(step);

        if (cur.row == er && cur.col == ec) {
            result.found = true;
            break;
        }

        for (int nid : adj[id]) {
            if (settled[nid]) continue;
            int nr = nid / Config::GRID_COLS;
            int nc = nid % Config::GRID_COLS;
            double w = static_cast<double>(grid.at(nr, nc).weight);
            double nd = dist[id] + w;
            if (nd < dist[nid]) {
                dist[nid] = nd;
                parent[nid] = {cur.row, cur.col};
                pq.push({nd, nr, nc});
            }
        }
    }

    if (result.found)
        result.path = reconstructPath(parent, sr, sc, er, ec);

    return result;
}

AlgoResult Algorithms::aStar(Grid& grid) {
    AlgoResult result;
    if (!grid.hasStart() || !grid.hasEnd()) return result;

    grid.resetPathData();
    auto adj = buildAdjacency(grid);
    auto [sr, sc] = grid.getStart();
    auto [er, ec] = grid.getEnd();

    const int N = Config::GRID_ROWS * Config::GRID_COLS;
    std::vector<double> gScore(N, 1e18);
    std::vector<bool> closed(N, false);
    std::unordered_map<int, std::pair<int, int>> parent;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;

    gScore[idOf(sr, sc)] = 0.0;
    pq.push({heuristic(sr, sc, er, ec), sr, sc});

    while (!pq.empty()) {
        PQItem cur = pq.top();
        pq.pop();
        int id = idOf(cur.row, cur.col);
        if (closed[id]) continue;
        closed[id] = true;

        AlgoStep step;
        step.visitedNodes.push_back({cur.row, cur.col});
        result.steps.push_back(step);

        if (cur.row == er && cur.col == ec) {
            result.found = true;
            break;
        }

        for (int nid : adj[id]) {
            if (closed[nid]) continue;
            int nr = nid / Config::GRID_COLS;
            int nc = nid % Config::GRID_COLS;
            double w = static_cast<double>(grid.at(nr, nc).weight);
            double tentativeG = gScore[id] + w;
            if (tentativeG < gScore[nid]) {
                gScore[nid] = tentativeG;
                parent[nid] = {cur.row, cur.col};
                double f = tentativeG + heuristic(nr, nc, er, ec);
                pq.push({f, nr, nc});
            }
        }
    }

    if (result.found)
        result.path = reconstructPath(parent, sr, sc, er, ec);

    return result;
}
