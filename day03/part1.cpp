#include <iostream>
#include <cctype>
#include <vector>
#include <string>

using namespace std;

vector<string> read_input() {
    vector<string> result;
    while (getline(cin, result.emplace_back()));
    result.pop_back();
    return result;
}

pair<int, int> find_number_indexes_in_schematic(const vector<string>& a, int i, int j) {
    int l = j;
    while (l > 0 && isdigit(a.at(i).at(l-1))) l--;
    int r = j;
    while (r < int(a.at(i).size())-1 && isdigit(a.at(i).at(r+1))) r++;
    return {l, r};
}

int parse_number_in_schematic(const vector<string>& a, int i, int j) {
    const auto [l, r] = find_number_indexes_in_schematic(a, i, j);
    int num = 0;
    for (int k = l; k <= r; k++) {
        num = 10*num + (a.at(i).at(k) - '0');
    }
    return num;
}

void erase_number_in_schematic(vector<string>& a, int i, int j) {
    const auto [l, r] = find_number_indexes_in_schematic(a, i, j);
    for (int k = l; k <= r; k++) {
        a.at(i).at(k) = '.';
    }
}

vector<pair<int, int>> neighboring_positions(int i, int j, int rows, int cols) {
    vector<pair<int, int>> result;
    for (int ni = i-1; ni <= i+1; ni++) {
        for (int nj = j-1; nj <= j+1; nj++) {
            if (pair(i, j) != pair(ni, nj) &&
                0 <= ni && ni < rows &&
                0 <= nj && nj < cols)
            {
                result.emplace_back(ni, nj);
            }
        }
    }
    return result;
}

bool is_symbol(char ch) {
    return !isdigit(ch) && ch != '.';
}

int main() {
    auto lines = read_input();
    
    int result = 0;
    
    const int rows = int(lines.size());
    const int cols = int(lines.at(0).size());
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            const char ch = lines.at(i).at(j);
            if (is_symbol(ch)) {
                for (const auto& [ni, nj] : neighboring_positions(i, j, rows, cols)) {
                    if (isdigit(lines.at(ni).at(nj))) {
                        result += parse_number_in_schematic(lines, ni, nj);
                        erase_number_in_schematic(lines, ni, nj);
                    }
                }
            }
        }
    }
    
    cout << result << '\n';
    return 0;
}
