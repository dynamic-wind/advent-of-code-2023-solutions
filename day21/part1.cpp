#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <set>
#include <utility>

using namespace std;

pair<int, int> find_start(const vector<string>& lines) {
    for (int i = 0; i < int(lines.size()); i++) {
        for (int j = 0; j < int(lines.at(i).size()); j++) {
            if (lines.at(i).at(j) == 'S') {
                return {i, j};
            }
        }
    }
    cerr << "No start" << endl; terminate();
}

int main() {
    const array<pair<int, int>, 4> offsets = {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};
    vector<string> lines;
    while (lines.emplace_back(), getline(cin, lines.back()));
    lines.pop_back();
    auto [si, sj] = find_start(lines);
    set<pair<int, int>> current;
    current.emplace(si, sj);
    for (int i = 0; i < 64; i++) {
        set<pair<int, int>> next;
        for (const auto& [ni, nj] : current) {
            for (const auto& [oi, oj] : offsets) {
                int ai = ni + oi;
                int aj = nj + oj;
                if (0 <= ai && ai < int(lines.size()) &&
                    0 <= aj && aj < int(lines.at(ai).size()) &&
                    lines.at(ai).at(aj) != '#')
                {
                    next.emplace(ai, aj);
                }
            }
        }
        current = move(next);
    }
    cout << current.size() << endl;
    return 0;
}
