#include <iostream>
#include <vector>
#include <string>
#include <optional>

using namespace std;

optional<vector<string>> read_pattern() {
    vector<string> result;
    while (true) {
        result.emplace_back();
        if (!getline(cin, result.back())) {
            result.pop_back();
            if (result.empty()) {
                return nullopt;
            } else {
                return result;
            }
        }
        if (result.back() == "") {
            result.pop_back();
            break;
        }
    }
    return result;
}

int find_symmetry_row(const vector<string>& pattern) {
    for (int i = 0; i < int(pattern.size())-1; i++) {
        bool answer = true;
        for (int j = 0; i-j >= 0 && i+j+1 < int(pattern.size()); j++) {
            if (pattern.at(i-j) != pattern.at(i+j+1)) {
                answer = false;
                break;
            }
        }
        if (answer) {
            return i+1;
        }
    }
    return -1;
}

int find_symmetry_column(const vector<string>& pattern) {
    for (int i = 0; i < int(pattern.at(0).size())-1; i++) {
        bool answer = true;
        for (int j = 0; i-j >= 0 && i+j+1 < int(pattern.at(0).size()); j++) {
            for (int k = 0; k < int(pattern.size()); k++) {
                if (pattern.at(k).at(i-j) != pattern.at(k).at(i+j+1)) {
                    answer = false;
                    break;
                }
            }
            if (!answer) break;
        }
        if (answer) {
            return i+1;
        }
    }
    return -1;
}

int main() {
    optional<vector<string>> opt;
    int result = 0;
    while ((opt = read_pattern()) != nullopt) {
        auto& pattern = opt.value();
        int row = find_symmetry_row(pattern);
        if (row != -1) {
            result += 100*row;
        } else {
            int column = find_symmetry_column(pattern);
            result += column;
        }
    }
    cout << result << endl;
    return 0;
}
