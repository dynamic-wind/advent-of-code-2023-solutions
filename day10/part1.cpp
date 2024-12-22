#include <iostream>
#include <cassert>
#include <array>
#include <tuple>
#include <vector>
#include <string>

using namespace std;

using ii = pair<int, int>;

pair<ii, ii> get_adjacents(const vector<string>& lines, int i, int j) {
    switch (lines.at(i).at(j)) {
        case '|': return {{i+1, j}, {i-1, j}};
        case '-': return {{i, j+1}, {i, j-1}};
        case 'L': return {{i-1, j}, {i, j+1}};
        case 'J': return {{i-1, j}, {i, j-1}};
        case '7': return {{i+1, j}, {i, j-1}};
        case 'F': return {{i+1, j}, {i, j+1}};
        case 'S': {
            static const array<ii, 4> offsets = {{{1,0}, {0,1}, {-1,0}, {0,-1}}};
            vector<ii> result;
            for (const auto& [di, dj] : offsets) {
                int ni = i + di;
                int nj = j + dj;
                if (0 <= ni && ni < int(lines.size()) &&
                    0 <= nj && nj < int(lines.at(ni).size()))
                {
                    if (di == 1) {
                        switch (lines.at(ni).at(nj)) {
                            case '|': case 'L': case 'J':
                                result.emplace_back(ni, nj);
                        }
                    } else if (di == -1) {
                        switch (lines.at(ni).at(nj)) {
                            case '|': case '7': case 'F':
                                result.emplace_back(ni, nj);
                        }
                    } else if (dj == 1) {
                        switch (lines.at(ni).at(nj)) {
                            case '-': case 'J': case '7':
                                result.emplace_back(ni, nj);
                        }
                    } else if (dj == -1) {
                        switch (lines.at(ni).at(nj)) {
                            case '-': case 'L': case 'F':
                                result.emplace_back(ni, nj);
                        }
                    }
                }
            }
            assert(result.size() == 2);
            return {result.at(0), result.at(1)};
        }
        default:
            throw "get_adjacents switch";
    }
}

ii get_start(const vector<string>& lines) {
    for (int i = 0; i < int(lines.size()); i++) {
        for (int j = 0; j < int(lines.at(i).size()); j++) {
            if (lines.at(i).at(j) == 'S') {
                return {i, j};
            }
        }
    }
    throw "get_start no S";
}

int main() {
    vector<string> lines;
    while (lines.emplace_back(), getline(cin, lines.back()));
    lines.pop_back();
    ii start = get_start(lines);
    ii prev = start;
    ii curr = get_adjacents(lines, start.first, start.second).first;
    int path_length = 1;
    while (curr != start) {
        auto [p1, p2] = get_adjacents(lines, curr.first, curr.second);
        tie(prev, curr) = tie(curr, prev == p1 ? p2 : p1);
        path_length++;
    }
    cout << (path_length / 2) << endl;
    return 0;
}
