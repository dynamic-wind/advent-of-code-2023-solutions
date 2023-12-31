#include <iostream>
#include <limits>
#include <string>
#include <cassert>

using namespace std;

int main() {
    int result = 0;
    for (int i = 1; !cin.ignore(numeric_limits<streamsize>::max(), ':').eof(); i++) {
        bool possible = true;
        while (cin.get() != '\n') {
            int n;
            assert(cin >> n);
            string color;
            assert(cin >> color);
            char c = color.front();
            if ((c == 'r' && n > 12) ||
                (c == 'g' && n > 13) ||
                (c == 'b' && n > 14)) {
                possible = false;
            }
        }
        if (possible) {
            result += i;
        }
    }
    cout << result << '\n';
    return 0;
}
