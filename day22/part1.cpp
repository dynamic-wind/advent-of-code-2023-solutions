#include <iostream>
#include <cassert>
#include <vector>
#include <string>
#include <tuple>

using namespace std;

using iii = tuple<int, int, int>;

struct Grid {
    const int width;
    const int depth;
    const int height;
    vector<int> v;
    
    Grid(int w, int d, int h) : width(w), depth(d), height(h), v(w*d*h, 0) {}

    int& at(int x, int y, int z) {
        return v.at((x*depth + y)*height + z);
    }

    int at(int x, int y, int z) const {
        return v.at((x*depth + y)*height + z);
    }
};

struct Brick {
    const int id;
    iii p1;
    iii p2;
    
    Brick(int id, const iii& p1, const iii& p2, Grid& grid) :
        id(id),
        p1(min(p1, p2)),
        p2(max(p1, p2))
    {
        const auto& [x1, y1, z1] = p1;
        const auto& [x2, y2, z2] = p2;
        assert(int(x1==x2) + int(y1==y2) + int(z1==z2) >= 2);
        assert(z1 >= 1 && z2 >= 1);
        for (int i = x1; i <= x2; i++) {
            for (int j = y1; j <= y2; j++) {
                for (int k = z1; k <= z2; k++) {
                    grid.at(i, j, k) = id;
                }
            }
        }
    }

    bool can_be_lowered(const Grid& grid) const {
        const auto& [x1, y1, z1] = p1;
        const auto& [x2, y2, z2] = p2;
        for (int x = x1; x <= x2; x++) {
            for (int y = y1; y <= y2; y++) {
                if (z1 == 1) {
                    return false;
                }
                int below = grid.at(x, y, z1-1);
                if (below != 0 && below != id) {
                    return false;
                }
            }
        }
        return true;
    }

    void lower(Grid& grid) {
        auto& [x1, y1, z1] = p1;
        auto& [x2, y2, z2] = p2;
        for (int x = x1; x <= x2; x++) {
            for (int y = y1; y <= y2; y++) {
                for (int z = z1; z <= z2; z++) {
                    int& here = grid.at(x, y, z);
                    int& below = grid.at(x, y, z-1);
                    assert(z != 1 && below == 0);
                    below = here;
                    here = 0;
                }
            }
        }
        z1--;
        z2--;
    }

    bool can_be_lowered_without(const Grid& grid, int removed_id) const {
        const auto& [x1, y1, z1] = p1;
        const auto& [x2, y2, z2] = p2;
        for (int x = x1; x <= x2; x++) {
            for (int y = y1; y <= y2; y++) {
                if (z1 == 1) {
                    return false;
                }
                int below = grid.at(x, y, z1-1);
                if (below != 0 && below != id && below != removed_id) {
                    return false;
                }
            }
        }
        return true;
    }
};

pair<Grid, vector<Brick>> read_input() {
    vector<pair<iii, iii>> brick_positions;
    char ch;
    int x1, y1, z1, x2, y2, z2;
    while (cin >> x1 >> ch >> y1 >> ch >> z1 >> ch >> x2 >> ch >> y2 >> ch >> z2) {
        iii p1 = {x1, y1, z1};
        iii p2 = {x2, y2, z2};
        if (p1 > p2) {
            swap(p1, p2);
        }
        brick_positions.emplace_back(p1, p2);
    }
    int w = 0;
    int d = 0;
    int h = 0;
    for (const auto& [p1, p2] : brick_positions) {
        w = max(w, get<0>(p2));
        d = max(d, get<1>(p2));
        h = max(h, get<2>(p2));
    }
    Grid grid(w+1, d+1, h+1);
    vector<Brick> bricks;
    bricks.reserve(brick_positions.size());
    int id = 1;
    for (const auto& [p1, p2] : brick_positions) {
        bricks.emplace_back(id, p1, p2, grid);
        id++;
    }
    return {grid, bricks};
}

int main() {
    auto [grid, bricks] = read_input();
    bool all_lowered = false;
    while (!all_lowered) {
        all_lowered = true;
        for (Brick& brick : bricks) {
            if (brick.can_be_lowered(grid)) {
                all_lowered = false;
                do brick.lower(grid); while (brick.can_be_lowered(grid));
            }
        }
    }
    int result = 0;
    for (const Brick& brick : bricks) {
        bool can_be_disintegrated = true;
        for (const Brick& other : bricks) {
            if (brick.id != other.id && other.can_be_lowered_without(grid, brick.id)) {
                can_be_disintegrated = false;
                break;
            }
        }
        result += int(can_be_disintegrated);
    }
    cout << result << endl;
    return 0;
}
