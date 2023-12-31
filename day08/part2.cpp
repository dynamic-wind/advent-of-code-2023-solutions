#include <iostream>
#include <cstdint>
#include <numeric>
#include <vector>
#include <limits>
#include <utility>
#include <cassert>
#include <string>
#include <unordered_map>

using namespace std;

using graph = unordered_map<string, pair<string, string>>;

graph read_graph() {
    graph g;
    while (true) {
        string key;
        if (!(cin >> key)) break;
        string equals;
        if (!(cin >> equals)) break;
        string left;
        if (!(cin >> left)) break;
        left.erase(0, 1);
        left.pop_back();
        string right;
        if (!(cin >> right)) break;
        right.pop_back();
        g.emplace(key, pair(left, right));
    }
    return g;
}

int main() {
    string instructions;
    assert(getline(cin, instructions));
    assert(cin.ignore(numeric_limits<streamsize>::max(), '\n'));
    graph g = read_graph();
    vector<int64_t> cycles;
    for (auto& [key, lr] : g) {
        if (key.back() == 'A') {
            string s = key;
            int r = 0;
            while (s.back() != 'Z') {
                if (instructions.at(r % instructions.size()) == 'L') {
                    s = g.at(s).first;
                } else {
                    s = g.at(s).second;
                }
                r++;
            }
            cycles.push_back(r);
        }
    }
    int64_t result = 1;
    for (int64_t i : cycles) {
        result = lcm(result, i);
    }
    cout << result << '\n';
    return 0;
}
