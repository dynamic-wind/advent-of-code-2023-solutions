#include <iostream>
#include <string>
#include <array>
#include <cctype>
#include <string_view>
#include <optional>

using namespace std;

optional<int> parse_num_starting_at(const string& s, int i) {
    constexpr array<string_view, 9> nums = {
        "one",
        "two",
        "three",
        "four",
        "five",
        "six",
        "seven",
        "eight",
        "nine",
    };
    if (isdigit(s.at(i))) {
        return s.at(i)-'0';
    }
    for (int j = 0; j < int(nums.size()); j++) {
        auto substr = string_view(s).substr(i, nums.at(j).size());
        if (nums.at(j) == substr) {
            return j+1;
        }
    }
    return nullopt;
}

int main() {
    int result = 0;
    string line;
    while (getline(cin, line)) {
        optional<int> left_num;
        for (int i = 0; !left_num.has_value(); i++) {
            left_num = parse_num_starting_at(line, i);
        }
        optional<int> right_num;
        for (int j = int(line.size()-1); !right_num.has_value(); j--) {
            right_num = parse_num_starting_at(line, j);
        }
        result += 10*left_num.value() + right_num.value();
    }
    cout << result << '\n';
}
