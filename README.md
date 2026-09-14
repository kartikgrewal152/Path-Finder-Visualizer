# Path Finder Visualizer (C++ / SFML / Dear ImGui)

A Google-Maps-style routing simulator: an interactive grid where you place a
start point, an end point, walls, and weighted "terrain," then watch
**Dijkstra's Algorithm** or **A\* Search** explore the grid wave-by-wave and
draw the shortest path.

![theme](https://img.shields.io/badge/theme-dark-1a1a1f) ![lang](https://img.shields.io/badge/C%2B%2B-17-blue)

## Features

- Dark-themed sidebar (Dear ImGui) with algorithm dropdown, coordinate
  inputs, weight toggle, speed slider, and board controls.
- Left-click to draw walls; **Shift + Left-click** to drop weighted nodes
  (cost 5 or 10, selectable in the sidebar); **Ctrl/Alt + Left-click** to
  place Start/End directly on the grid; right-click to erase a cell.
- Exact row/col text inputs for Start and End, independent of clicking.
- Step-by-step animated exploration (light blue "wave") followed by an
  animated shortest-path highlight (yellow).
- Dijkstra (uniform-cost) and A* (Manhattan-distance heuristic), both built
  on a `std::vector` adjacency list, a `std::priority_queue` min-heap, and a
  `std::unordered_map` for parent/back-pointer tracking.
- Grid size is not hardcoded in logic — change 3 constants in
  `include/Constants.hpp` and the whole app (window size, cell size, layout)
  adapts.

## Project Layout

```
PathFinderVisualizer/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── Constants.hpp   # grid size, colors, node states
│   ├── Node.hpp        # per-cell data (state, weight, algo bookkeeping)
│   ├── Grid.hpp         # board container, editing, rendering
│   ├── Algorithms.hpp   # Dijkstra / A* interface
│   └── UIManager.hpp    # ImGui sidebar interface
└── src/
    ├── main.cpp         # SFML window, event loop, animation driver
    ├── Grid.cpp
    ├── Algorithms.cpp
    └── UIManager.cpp
```

11 source files total — well inside the 50-file limit, while still keeping
each concern (rendering, algorithms, UI, data) in its own module.

## Dependencies (fetched automatically — nothing to install manually)

`CMakeLists.txt` uses `FetchContent` to pull and build, at configure time:

- **SFML 2.6.1** — window/graphics/input
- **Dear ImGui v1.90.4** — immediate-mode UI (compiled as a small static lib)
- **ImGui-SFML v2.6** — the SFML backend for Dear ImGui

You only need a C++17 compiler, CMake ≥ 3.15, Git, and the normal system
libraries SFML depends on for your OS (see below).

## Building

### Linux (Debian/Ubuntu)

```bash
# System libraries SFML needs to build/run (X11, OpenGL, audio, etc.)
sudo apt update
sudo apt install -y build-essential cmake git \
    libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
    libudev-dev libgl1-mesa-dev libfreetype-dev libopenal-dev libflac-dev libvorbis-dev

cd PathFinderVisualizer
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(nproc)
./build/PathFinderVisualizer
```

### Windows (Visual Studio 2022)

```powershell
cd PathFinderVisualizer
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\build\Release\PathFinderVisualizer.exe
```

### macOS

```bash
brew install cmake

cd PathFinderVisualizer
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j$(sysctl -n hw.ncpu)
./build/PathFinderVisualizer
```

> First configure will take a few minutes — it's cloning and building SFML
> and Dear ImGui. Subsequent builds are fast (cached in `build/_deps`).

## Controls

| Action | Input |
|---|---|
| Draw wall | Left-click a cell |
| Place weighted node (cost 5 or 10, chosen in sidebar) | Shift + Left-click |
| Set Start on the grid | Ctrl + Left-click |
| Set End on the grid | Alt + Left-click |
| Erase a cell | Right-click |
| Set Start/End by coordinate | Type row/col in the sidebar, click **Set** |
| Run the algorithm | Choose Dijkstra or A* in the dropdown, click **Start** |
| Clear only the path/exploration overlay | **Clear Path** |
| Clear only walls/weights | **Clear Walls / Weights** |
| Full wipe (also removes Start/End) | **Reset Board** |
| Animation speed | Sidebar slider (seconds per step) |

## Notes on the implementation

- `Grid` stores the board as `std::vector<std::vector<Node>>`; each `Node`
  tracks its visual state, traversal weight, and per-run algorithm fields.
- `Algorithms::dijkstra` / `Algorithms::aStar` first build a
  `std::vector<std::vector<int>>` adjacency list over walkable cells, then
  explore using a `std::priority_queue` (min-heap via `std::greater<>`),
  recording parents in a `std::unordered_map<int, std::pair<int,int>>` keyed
  by `row * COLS + col`. Each settled node is appended as an `AlgoStep`,
  which `main.cpp` replays on a timer to animate the search wave, followed
  by an animated replay of the reconstructed path.
- Weighted terrain (cost 5 / 10) affects both algorithms identically —
  Dijkstra explores in monotonic cost order and A* adds a Manhattan-distance
  heuristic on top of the same cost model.
