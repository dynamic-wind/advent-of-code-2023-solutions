#include <iostream>
#include <vector>
#include <string>
#include <array>

using namespace std;

const array<pair<int, int>, 4> offsets = {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};

vector<vector<pair<int, int>>> parse_graph(const vector<string>& lines) {
    const int rows = int(lines.size());
    const int cols = int(lines.at(0).size());
    vector<vector<pair<int, int>>> result(rows*cols, vector<pair<int, int>>{});
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (lines.at(i).at(j) != '#') {
                const int idx = i*cols + j;
                for (const auto& [oi, oj] : offsets) {
                    int ni = i + oi;
                    int nj = j + oj;
                    if (0 <= ni && ni < rows &&
                        0 <= nj && nj < cols &&
                        lines.at(ni).at(nj) != '#')
                    {
                        result.at(idx).emplace_back(1, ni*cols + nj);
                    }
                }
            }
        }
    }
    return result;
}

void compress_graph(vector<vector<pair<int, int>>>& graph, int position) {
    auto& adjacents = graph.at(position);
    for (int i = 0; i < int(adjacents.size())-1; i++) {
        for (int j = i+1; j < int(adjacents.size()); j++) {
            const auto& p1 = adjacents.at(i);
            const auto& p2 = adjacents.at(j);
            if (p1.second == p2.second) {
                if (p1.first < p2.first) {
                    adjacents.erase(adjacents.begin()+i);
                    j = i;
                } else {
                    adjacents.erase(adjacents.begin()+j);
                    j--;
                }
            }
        }
    }
    if (adjacents.size() == 2) {
        const auto [w1, n1] = adjacents.front();
        const auto [w2, n2] = adjacents.back();
        for (auto& [w, n] : graph.at(n1)) {
            if (n == position) {
                n = n2;
                w = w1+w2;
            }
        }
        for (auto& [w, n] : graph.at(n2)) {
            if (n == position) {
                n = n1;
                w = w1+w2;
            }
        }
        adjacents.clear();
        compress_graph(graph, n1);
        compress_graph(graph, n2);
    }
}

int dfs(const vector<vector<pair<int, int>>>& graph, vector<bool>& visited, int index, int depth) {
    if (index == int(graph.size())-2) {
        return depth;
    } else if (visited.at(index)) {
        return 0;
    } else {
        visited.at(index) = true;
        int result = 0;
        for (const auto& [w, n] : graph.at(index)) {
            result = max(result, dfs(graph, visited, n, depth+w));
        }
        visited.at(index) = false;
        return result;
    }
}

int main() {
    vector<string> lines;
    while (getline(cin, lines.emplace_back()));
    lines.pop_back();
    auto graph = parse_graph(lines);
    for (int i = 0; i < int(graph.size()); i++) {
        compress_graph(graph, i);
    }
    vector<bool> visited(graph.size(), false);
    cout << dfs(graph, visited, 1, 0) << '\n';
    return 0;
}
