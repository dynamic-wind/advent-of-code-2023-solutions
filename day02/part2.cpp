#include <iostream>
#include <cassert>
#include <limits>
#include <string>

using namespace std;

int main() {
    int result = 0;
    for (int i = 1; !cin.ignore(numeric_limits<streamsize>::max(), ':').eof(); i++) {
        int red = 0;
        int green = 0;
        int blue = 0;
        while (cin.get() != '\n') {
            int n;
            assert(cin >> n);
            string color;
            assert(cin >> color);
            switch (color.front()) {
                case 'r':
                    red = max(red, n);
                    break;
                case 'g':
                    green = max(green, n);
                    break;
                case 'b':
                    blue = max(blue, n);
                    break;
            }
        }
        result += red*green*blue;
    }
    cout << result << '\n';
    return 0;
}
