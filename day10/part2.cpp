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

vector<vector<bool>> get_path_mask(const vector<string>& lines) {
    vector<vector<bool>> result(lines.size(), vector<bool>(lines.at(0).size(), false));
    ii start = get_start(lines);
    result.at(start.first).at(start.second) = true;
    ii prev = start;
    ii curr = get_adjacents(lines, start.first, start.second).first;
    while (curr != start) {
        result.at(curr.first).at(curr.second) = true;
        auto [p1, p2] = get_adjacents(lines, curr.first, curr.second);
        tie(prev, curr) = tie(curr, prev == p1 ? p2 : p1);
    }
    return result;
}

bool is_inside_enclosed_area(const vector<string>& lines, const vector<vector<bool>>& path_mask, int i, int j) {
    if (path_mask.at(i).at(j)) return false;
    int count = 0;
    for (int nj = j+1; nj < int(lines.at(i).size()); nj++) {
        if (path_mask.at(i).at(nj)) { 
            char ch = lines.at(i).at(nj);
            switch (ch) {
                case '|': case 'L': case 'J':
                    count++;
            }
        }
    }
    return count % 2 == 1;
}

void remove_start(vector<string>& lines) {
    ii start = get_start(lines);
    static const array<ii, 4> offsets = {{{1,0}, {0,1}, {-1,0}, {0,-1}}};
    vector<int> v;
    for (int i = 0; i < int(offsets.size()); i++) {
        int ni = start.first  + offsets.at(i).first;
        int nj = start.second + offsets.at(i).second;
        if (0 <= ni && ni < int(lines.size()) &&
            0 <= nj && nj < int(lines.at(ni).size()))
        {
            if (i == 0) {
                switch (lines.at(ni).at(nj)) {
                    case '|': case 'L': case 'J':
                        v.push_back(i);
                }
            } else if (i == 1) {
                switch (lines.at(ni).at(nj)) {
                    case '-': case 'J': case '7':
                        v.push_back(i);
                }
            } else if (i == 2) {
                switch (lines.at(ni).at(nj)) {
                    case '|': case '7': case 'F':
                        v.push_back(i);
                }
            } else if (i == 3) {
                switch (lines.at(ni).at(nj)) {
                    case '-': case 'L': case 'F':
                        v.push_back(i);
                }
            }
        }
    }
    assert(v.size() == 2);
    int i1 = v.at(0);
    int i2 = v.at(1);
    char ch = 0;
    switch (i1) {
        case 0:
            switch (i2) {
                case 1: ch = 'F'; break;
                case 2: ch = '|'; break;
                case 3: ch = '7'; break;
            }
            break;
        case 1:
            switch (i2) {
                case 2: ch = 'L'; break;
                case 3: ch = '-'; break;
            }
            break;
        case 2:
            switch (i2) {
                case 3: ch = 'J'; break;
            }
    }
    assert(ch != 0);
    lines.at(start.first).at(start.second) = ch;
}

int main() {
    vector<string> lines;
    while (lines.emplace_back(), getline(cin, lines.back()));
    lines.pop_back();
    auto mask = get_path_mask(lines);
    remove_start(lines);
    int result = 0;
    for (int i = 0; i < int(lines.size()); i++) {
        for (int j = 0; j < int(lines.at(i).size()); j++) {
            if (is_inside_enclosed_area(lines, mask, i, j)) {
                lines.at(i).at(j) = '#';
                result++;
            }
        }
    }
    cout << result << endl;
    return 0;
}
