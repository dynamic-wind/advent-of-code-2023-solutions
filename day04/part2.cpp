#include <iostream>
#include <vector>
#include <set>
#include <string>
#include <limits>
#include <cassert>

using namespace std;

vector<pair<set<int>, vector<int>>> parse_input() {
    vector<pair<set<int>, vector<int>>> result;
    
    while (!cin.ignore(numeric_limits<streamsize>::max(), ':').eof()) {
        auto& [winning_numbers, guesses] = result.emplace_back();
        
        int num;
        while (cin >> num) {
            winning_numbers.insert(num);
        }
        cin.clear();

        cin.ignore(numeric_limits<streamsize>::max(), '|');

        while (cin.get() != '\n') {
            assert(cin >> num);
            guesses.push_back(num);
        }
    }

    return result;
}

int main() {
    const auto input = parse_input();

    vector<int> scratch_cards(input.size(), 1);
    
    int card_number = 0;
    for (const auto& [winning_numbers, guesses] : input) {
        int correct_guesses = 0;
        for (int guess : guesses) {
            if (winning_numbers.count(guess) > 0) {
                correct_guesses++;
            }
        }
        for (int i = 1; i <= correct_guesses; i++) {
            scratch_cards.at(card_number + i) += scratch_cards.at(card_number);
        }
        card_number++;
    }
    
    int result = 0;
    for (int i : scratch_cards) result += i;
    cout << result << '\n';
    return 0;
}
