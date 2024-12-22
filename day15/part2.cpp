#include <iostream>
#include <algorithm>
#include <array>
#include <vector>
#include <charconv>
#include <cstdint>
#include <string>
#include <cassert>
#include <string_view>
#include <unordered_map>

using namespace std;

vector<pair<string_view, int>> parse_line(const string& line) {
    vector<pair<string_view, int>> result;
    int pos = -1;
    while (pos < int(line.size())) {
        int prev = pos+1;
        pos = int(line.find(',', prev));
        if (pos == int(line.npos)) pos = int(line.size());
        auto view = string_view(line).substr(prev, pos-prev);
        if (view.back() == '-') {
            result.emplace_back(view.substr(0, view.size()-1), -1);
        } else {
            int equals_pos = int(view.find('='));
            assert(equals_pos != int(view.npos));
            int count;
            auto [ptr, ec] = from_chars(view.substr(equals_pos+1).data(), view.substr(view.size()).data(), count);
            assert(ec == errc());
            result.emplace_back(view.substr(0, equals_pos), count);
        }
    }
    return result;
}

uint8_t hash_label(string_view view) {
    uint8_t result = 0;
    for (char c : view) {
        result += uint8_t(c);
        result *= 17;
    }
    return result;
}

int main() {
    string line;
    assert(getline(cin, line));
    auto input = parse_line(line);
    
    array<vector<pair<string, int>>, 256> boxes;
    for (int i = 0; i < int(input.size()); i++) {
        const auto& [label, focal_length] = input.at(i);
        string label_string(label);
        uint8_t label_hash = hash_label(label);
        auto& box = boxes.at(label_hash);
        auto it = find_if(
            box.begin(), box.end(), 
            [&](const auto& p) { return p.first == label_string; }
        );
        if (focal_length == -1) {
            if (it != box.end()) {
                box.erase(it);
            }
        } else {
            if (it == box.end()) {
                box.emplace_back(move(label_string), focal_length);
            } else {
                it->second = focal_length;
            }
        }
    }
    
    int result = 0;
    for (int i = 0; i < int(boxes.size()); i++) {
        const auto& box = boxes.at(i);
        for (int j = int(box.size()-1); j >= 0; j--) {
            const auto& [name, focal_length] = box.at(j);
            result += (i+1) * (j+1) * focal_length;
        }
    }
    
    cout << result << endl;
    return 0;
}
