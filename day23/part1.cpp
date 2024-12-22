#include <iostream>
#include <vector>
#include <string>
#include <array>

using namespace std;

int dfs(const vector<string>& lines, vector<vector<bool>>& visited, int i, int j, int depth) {
    if (pair(i, j) == pair(int(lines.size()-1), int(lines.at(0).size()-2))) {
        return depth;
    } else if (i < 0 || i >= int(lines.size()) ||
               j < 0 || j >= int(lines.at(0).size()) ||
               lines.at(i).at(j) == '#' || 
               visited.at(i).at(j))
    {
        return 0;
    } else {
        visited.at(i).at(j) = true;
        switch (lines.at(i).at(j)) {
            case 'v':
                return dfs(lines, visited, i+1, j, depth+1);
            case '>':
                return dfs(lines, visited, i, j+1, depth+1);
            case '^':
                return dfs(lines, visited, i-1, j, depth+1);
            case '<':
                return dfs(lines, visited, i, j-1, depth+1);
        }
        const array<pair<int, int>, 4> offsets = {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};
        int result = 0;
        for (const auto& [oi, oj] : offsets) {
            result = max(result, dfs(lines, visited, i+oi, j+oj, depth+1));
        }
        visited.at(i).at(j) = false;
        return result;
    }
}

int main() {
    vector<string> lines;
    while (getline(cin, lines.emplace_back()));
    lines.pop_back();
    vector<vector<bool>> visited(lines.size(), vector(lines.at(0).size(), false));
    cout << dfs(lines, visited, 0, 1, 0) << '\n';
    return 0;
}
