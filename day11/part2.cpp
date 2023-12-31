#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>
#include <set>

using namespace std;

constexpr int times_larger = 1'000'000;

using ii = pair<int, int>;

vector<string> read_lines() {
    vector<string> lines;
    while (lines.emplace_back(), getline(cin, lines.back()));
    lines.pop_back();
    return lines;
}

set<int> get_empty_rows(const vector<string>& lines) {
    set<int> result;
    for (int i = 0; i < int(lines.size()); i++) {
        const string& line = lines.at(i);
        if (all_of(line.begin(), line.end(), [](char c) { return c == '.'; })) {
            result.insert(i);
        }
    }
    return result;
}

set<int> get_empty_columns(const vector<string>& lines) {
    set<int> result;
    for (int j = 0; j < int(lines.at(0).size()); j++) {
        bool empty_column = true;
        for (int i = 0; i < int(lines.size()); i++) {
            if (lines.at(i).at(j) != '.') {
                empty_column = false;
                break;
            }
        }
        if (empty_column) {
            result.insert(j);
        }
    }
    return result;
}

vector<ii> get_points(const vector<string>& lines) {
    vector<ii> result;
    for (int i = 0; i < int(lines.size()); i++) {
        for (int j = 0; j < int(lines.at(i).size()); j++) {
            if (lines.at(i).at(j) == '#') {
                result.emplace_back(i, j);
            }
        }
    }
    return result;
}

int main() {
    auto lines = read_lines();
    auto empty_rows = get_empty_rows(lines);
    auto empty_columns = get_empty_columns(lines);
    auto points = get_points(lines);
    long long result = 0;
    for (int i = 0; i < int(points.size()); i++) {
        for (int j = i+1; j < int(points.size()); j++) {
            const auto& [x1, y1] = points.at(i);
            const auto& [x2, y2] = points.at(j);
            result += abs(x2-x1) + abs(y2-y1);
            for (int k = min(x1, x2)+1; k <= max(x1, x2)-1; k++) {
                if (empty_rows.find(k) != empty_rows.end()) {
                    result += times_larger-1;
                }
            }
            for (int k = min(y1, y2)+1; k <= max(y1, y2)-1; k++) {
                if (empty_columns.find(k) != empty_columns.end()) {
                    result += times_larger-1;
                }
            }
        }
    }
    cout << result << endl;
    return 0;
}
