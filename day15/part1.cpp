#include <iostream>
#include <cstdint>
#include <string>
#include <cassert>
#include <string_view>

using namespace std;

int main() {
    int result = 0;
    string line;
    assert(getline(cin, line));
    int pos = -1;
    while (pos < int(line.size())) {
        int prev = pos+1;
        pos = int(line.find(',', prev));
        if (pos == int(line.npos)) pos = int(line.size());
        auto view = string_view(line).substr(prev, pos-prev);
        uint8_t view_hash = 0;
        for (char c : view) {
            view_hash += uint8_t(c);
            view_hash *= 17;
        }
        result += view_hash;
    }
    cout << result << endl;
    return 0;
}
