#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> get_coefficients(const vector<int>& sequence) {
    vector<int> v = sequence;
    vector<int> result;
    while (!all_of(v.begin(), v.end(), [](int x) { return x == 0; })) {
        result.push_back(v[0]);
        for (int i = 0; i < int(v.size()) - 1; i++) {
            v.at(i) = v.at(i+1) - v.at(i);
        }
        v.pop_back();
    }
    return result;
}

int evaluate_sequence(const vector<int>& coefficients, int n) {
    int result = 0;
    int nCi = 1;
    for (int i = 0; i < int(coefficients.size()); i++) {
        result += coefficients.at(i) * nCi;
        nCi = nCi * (n-i) / (i+1);
    }
    return result;
}

int main() {
    int result = 0;
    string line;
    while (getline(cin, line)) {
        istringstream stream(move(line));
        vector<int> sequence;
        { int n; while (stream >> n) sequence.push_back(n); }
        vector<int> coefficients = get_coefficients(sequence);
        result += evaluate_sequence(coefficients, sequence.size());
    }
    cout << result << endl;
    return 0;
}
