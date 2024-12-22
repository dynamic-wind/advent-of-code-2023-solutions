#include <iostream>
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
        g.emplace(key, pair<string, string>(left, right));
    }
    return g;
}

int main() {
    string instructions;
    assert(getline(cin, instructions));
    assert(cin.ignore(numeric_limits<streamsize>::max(), '\n'));
    graph g = read_graph();
    int result = 0;
    string key = "AAA";
    while (key != "ZZZ") {
        const char instruction = instructions.at(result % instructions.size());
        if (instruction == 'L') {
            key = g.at(key).first;
        } else {
            key = g.at(key).second;
        }
        result++;
    }
    cout << result << '\n';
    return 0;
}
