#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>

using namespace std;

void roll_rocks_north(vector<string>& lines) {
    for (int j = 0; j < int(lines.at(0).size()); j++) {
        int empty_space = -1;
        for (int i = 0; i < int(lines.size()); i++) {
            if (lines.at(i).at(j) == '.') {
                if (empty_space == -1) {
                    empty_space = i;
                }
            } else if (lines.at(i).at(j) == '#') {
                empty_space = -1;
            } else if (lines.at(i).at(j) == 'O') {
                if (empty_space != -1) {
                    swap(lines.at(i).at(j), lines.at(empty_space).at(j));
                    empty_space++;
                }
            } else {
                assert(!"not '#', 'O' or '.'");
            }
        }
    }
}

int compute_load(const vector<string>& lines) {
    int result = 0;
    for (int i = 0; i < int(lines.size()); i++) {
        for (int j = 0; j < int(lines.at(i).size()); j++) {
            if (lines.at(i).at(j) == 'O') {
                result += int(lines.size()) - i;
            }
        }
    }
    return result;
}

int main() {
    vector<string> lines;
    while (lines.emplace_back(), getline(cin, lines.back()));
    lines.pop_back();
    roll_rocks_north(lines);
    int load = compute_load(lines);
    cout << load << endl;
    return 0;
}
