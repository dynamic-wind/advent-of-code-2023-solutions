#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <utility>

using namespace std;

using ii = pair<int, int>;

vector<string> read_lines() {
    vector<string> lines;
    while (lines.emplace_back(), getline(cin, lines.back()));
    lines.pop_back();
    return lines;
}

void expand_rows(vector<string>& lines) {
    vector<string> copy = move(lines);
    lines.clear();
    for (string& line : copy) {
        if (all_of(line.begin(), line.end(), [](char c) { return c == '.'; })) {
            lines.push_back(line);
        }
        lines.push_back(move(line));
    }
}

void expand_columns(vector<string>& lines) {
    vector<string> copy = move(lines);
    lines.assign(copy.size(), "");
    for (int j = 0; j < int(copy.at(0).size()); j++) {
        bool empty_column = true;
        for (const string& line : copy) {
            if (line.at(j) != '.') {
                empty_column = false;
                break;
            }
        }
        if (empty_column) {
            for (string& line : lines) {
                line.push_back('.');
            }
        }
        for (int i = 0; i < int(copy.size()); i++) {
            lines.at(i).push_back(copy.at(i).at(j));
        }
    }
}

void expand_space(vector<string>& lines) {
    expand_rows(lines);
    expand_columns(lines);
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
    expand_space(lines);
    auto points = get_points(lines);
    int result = 0;
    for (int i = 0; i < int(points.size()); i++) {
        for (int j = i+1; j < int(points.size()); j++) {
            const auto& [x1, y1] = points.at(i);
            const auto& [x2, y2] = points.at(j);
            result += abs(x2-x1) + abs(y2-y1);
        }
    }
    cout << result << endl;
    return 0;
}
