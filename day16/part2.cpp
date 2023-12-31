#include <iostream>
#include <array>
#include <cassert>
#include <utility>
#include <vector>
#include <string>

using namespace std;

enum Direction {
    Down = 0,
    Right = 1,
    Up = 2,
    Left = 3,
};

pair<int, int> move_forward(int i, int j, Direction d) {
    switch (d) {
        case Down: return {i+1, j};
        case Right: return {i, j+1};
        case Up: return {i-1, j};
        case Left: return {i, j-1};
    }
    terminate();
}

void find_energized(
    const vector<string>& map,
    vector<vector<array<bool, 4>>>& visited,
    vector<vector<bool>>& energized,
    int i, int j, Direction d)
{
    if (i < 0 || i >= int(visited.size()) ||
        j < 0 || j >= int(visited.at(i).size()) ||
        visited.at(i).at(j).at(static_cast<int>(d)))
    {
        return;
    }
    energized.at(i).at(j) = true;
    visited.at(i).at(j).at(static_cast<int>(d)) = true;
    switch (map.at(i).at(j)) {
        case '.': {
            auto [ni, nj] = move_forward(i, j, d);
            find_energized(map, visited, energized, ni, nj, d);
            break;
        }
        case '/': {
            switch (d) {
                case Down: d = Left; break;
                case Right: d = Up; break;
                case Up: d = Right; break;
                case Left: d = Down; break;
            }
            auto [ni, nj] = move_forward(i, j, d);
            find_energized(map, visited, energized, ni, nj, d);
            break;
        }
        case '\\': {
            switch (d) {
                case Up: d = Left; break;
                case Down: d = Right; break;
                case Left: d = Up; break;                    
                case Right: d = Down; break;
            }
            auto [ni, nj] = move_forward(i, j, d);
            find_energized(map, visited, energized, ni, nj, d);
            break;
        }
        case '|': {
            switch (d) {
                case Up: case Down: {
                    auto [ni, nj] = move_forward(i, j, d);
                    find_energized(map, visited, energized, ni, nj, d);
                    break;
                }
                case Left: case Right: {
                    auto [i2, j2] = move_forward(i, j, Up);
                    find_energized(map, visited, energized, i2, j2, Up);
                    auto [i3, j3] = move_forward(i, j, Down);
                    find_energized(map, visited, energized, i3, j3, Down);
                    break;
                }
            }
            break;
        }
        case '-': {
            switch (d) {
                case Left: case Right: {
                    auto [ni, nj] = move_forward(i, j, d);
                    find_energized(map, visited, energized, ni, nj, d);
                    break;
                }
                case Up: case Down: {
                    auto [i2, j2] = move_forward(i, j, Left);
                    find_energized(map, visited, energized, i2, j2, Left);
                    auto [i3, j3] = move_forward(i, j, Right);
                    find_energized(map, visited, energized, i3, j3, Right);
                    break;
                }
            }
            break;
        }
        default: cerr << map.at(i).at(j) << endl; terminate();
    }
}

int solve(const vector<string>& lines, int i, int j, Direction d) {
    vector<vector<array<bool, 4>>> visited(lines.size(), vector(lines.at(0).size(), array<bool, 4>{}));
    vector<vector<bool>> energized(lines.size(), vector(lines.at(0).size(), false));
    find_energized(lines, visited, energized, i, j, d);
    int result = 0;
    for (const auto& v : energized) {
        for (bool b : v) {
            result += int(b);
        }
    }
    return result;
}

int main() {
    vector<string> lines;
    while (lines.emplace_back(), getline(cin, lines.back()));
    lines.pop_back();
    int result = 0;
    for (int i = 0; i < int(lines.size()); i++) {
        result = max(result, solve(lines, i, 0, Right));
        result = max(result, solve(lines, i, int(lines.size()-1), Left));        
    }
    for (int j = 0; j < int(lines.at(0).size()); j++) {
        result = max(result, solve(lines, 0, j, Down));
        result = max(result, solve(lines, int(lines.at(0).size())-1, j, Up));
    }
    cout << result << endl;
    return 0;
}
