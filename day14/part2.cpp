#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <map>

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

void roll_rocks_west(vector<string>& lines) {
    for (string& line : lines) {
        int empty_space = -1;
        for (int j = 0; j < int(line.size()); j++) {
            if (line.at(j) == '.') {
                if (empty_space == -1) {
                    empty_space = j;
                }
            } else if (line.at(j) == '#') {
                empty_space = -1;
            } else if (line.at(j) == 'O') {
                if (empty_space != -1) {
                    swap(line.at(j), line.at(empty_space));
                    empty_space++;
                }
            } else {
                assert(!"not '#', 'O' or '.'");
            }
        }
    }
}

void roll_rocks_south(vector<string>& lines) {
    for (int j = 0; j < int(lines.at(0).size()); j++) {
        int empty_space = -1;
        for (int i = int(lines.size())-1; i >= 0; i--) {
            if (lines.at(i).at(j) == '.') {
                if (empty_space == -1) {
                    empty_space = i;
                }
            } else if (lines.at(i).at(j) == '#') {
                empty_space = -1;
            } else if (lines.at(i).at(j) == 'O') {
                if (empty_space != -1) {
                    swap(lines.at(i).at(j), lines.at(empty_space).at(j));
                    empty_space--;
                }
            } else {
                assert(!"not '#', 'O' or '.'");
            }
        }
    }
}

void roll_rocks_east(vector<string>& lines) {
    for (string& line : lines) {
        int empty_space = -1;
        for (int j = int(line.size())-1; j >= 0; j--) {
            if (line.at(j) == '.') {
                if (empty_space == -1) {
                    empty_space = j;
                }
            } else if (line.at(j) == '#') {
                empty_space = -1;
            } else if (line.at(j) == 'O') {
                if (empty_space != -1) {
                    swap(line.at(j), line.at(empty_space));
                    empty_space--;
                }
            } else {
                assert(!"not '#', 'O' or '.'");
            }
        }
    }
}

void perform_cycle(vector<string>& lines) {
    roll_rocks_north(lines);
    roll_rocks_west(lines);
    roll_rocks_south(lines);
    roll_rocks_east(lines);
}

void perform_n_cycles(vector<string>& lines, int n) {
    map<vector<string>, int> seen_before;
    for (int i = 0; i < n; i++) {
        const auto it = seen_before.find(lines);
        if (it == seen_before.end()) {
            seen_before.emplace(lines, i);
            perform_cycle(lines);
        } else {
            const int start = it->second;
            const int remainder = (n-start) % (i-start);
            for (int j = 0; j < remainder; j++) {
                perform_cycle(lines);
            }
            break;
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
    perform_n_cycles(lines, 1'000'000'000);
    const int load = compute_load(lines);
    cout << load << endl;
    return 0;
}
