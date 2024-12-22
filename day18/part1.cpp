#include <iostream>
#include <utility>
#include <limits>
#include <vector>
#include <string>

using namespace std;

pair<int, int> direction_to_offset(char ch) {
    switch (ch) {
        case 'U': return {-1, 0};
        case 'D': return {1, 0};
        case 'L': return {0, -1};
        case 'R': return {0, 1};
    }
    cerr << "Character not a direction: '" << ch << '\'' << endl;
    terminate();
}

pair<int, int> find_a_top_left_corner(const vector<vector<bool>>& grid) {
    for (int i = 0; i < int(grid.size()); i++) {
        for (int j = 0; j < int(grid.at(0).size()); j++) {
            if (grid.at(i).at(j)) {
                return {i, j};
            }
        }
    }
    cerr << "No top left corner" << endl;
    terminate();
}

void flood_fill(vector<vector<bool>>& grid, int i, int j) {
    if (grid.at(i).at(j)) {
        return;
    }
    grid.at(i).at(j) = true;
    for (char ch : "UDLR"s) {
        auto [oi, oj] = direction_to_offset(ch);
        flood_fill(grid, i+oi, j+oj);
    }
}

int main() {
    vector<pair<char, int>> input;
    while (true) {
        char ch;
        int n;
        if (!(cin >> ch >> n)) {
            break;
        }
        input.emplace_back(ch, n);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    int left = 0;
    int right = 0;
    int up = 0;
    int down = 0;
    int i = 0;
    int j = 0;
    for (const auto& [ch, n] : input) {
        auto [oi, oj] = direction_to_offset(ch);
        for (int k = 0; k < n; k++) {
            left = min(left, j);
            right = max(right, j);
            up = min(up, i);
            down = max(down, i);
            i += oi;
            j += oj;
        }
    }
    vector<vector<bool>> grid(down-up+1, vector(right-left+1, false));
    i = 0;
    j = 0;
    for (const auto& [ch, n] : input) {
        auto [oi, oj] = direction_to_offset(ch);
        for (int k = 0; k < n; k++) {
            grid.at(i-up).at(j-left) = true;
            i += oi;
            j += oj;
        }
    }
    
    auto [top_left_i, top_left_j] = find_a_top_left_corner(grid);
    flood_fill(grid, top_left_i+1, top_left_j+1);

    int result = 0;
    for (const auto& row : grid) {
        for (bool b : row) result += int(b);
    }
    cout << result << endl;
    
    return 0;
}
