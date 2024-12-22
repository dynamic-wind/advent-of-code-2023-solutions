#include <iostream>
#include <set>
#include <string>
#include <limits>
#include <cassert>

using namespace std;

int main() {
    int result = 0;
    
    while (!cin.ignore(numeric_limits<streamsize>::max(), ':').eof()) {
        set<int> winning_numbers;

        int num;
        while (cin >> num) {
            winning_numbers.insert(num);
        }
        cin.clear();
        
        cin.ignore(numeric_limits<streamsize>::max(), '|');
        
        int correct_guesses = 0;
        while (cin.get() != '\n') {
            assert(cin >> num);
            if (winning_numbers.count(num) > 0) {
                correct_guesses++;
            }
        }
        if (correct_guesses > 0) {
            result += 1 << (correct_guesses-1);
        }
    }
    cout << result << '\n';
    return 0;
}
