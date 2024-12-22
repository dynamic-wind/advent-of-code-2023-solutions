#include <iostream>
#include <string>
#include <cctype>

using namespace std;

int main() {
    string line;
    int result = 0;
    while (getline(cin, line)) {
        int i = 0;
        while (!isdigit(line.at(i))) i++;
        int j = int(line.size()-1);
        while (!isdigit(line.at(j))) j--;
        result += 10*(line.at(i)-'0') + (line.at(j)-'0');
    }
    cout << result << '\n';
    return 0;
}
