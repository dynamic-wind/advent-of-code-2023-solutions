#include <iostream>
#include <cassert>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include <cstdlib>

using namespace std;

pair<string, vector<int>> parse_line(const string& line) {
    istringstream stream(line);
    string field;
    assert(stream >> field);
    vector<int> nums;
    int n;
    while (stream >> n) {
        nums.push_back(n);
        stream.ignore(1);
    }
    return {move(field), move(nums)};
}

int rec(const string& field, int field_index, const vector<int>& nums, int nums_index, int count) {
    if (field_index == int(field.size())) {
        return nums_index == int(nums.size());
    } else if (field.at(field_index) == '#') {
        return rec(field, field_index+1, nums, nums_index, count+1);
    } else if (field.at(field_index) == '.') {
        if (count == 0) {
            return rec(field, field_index+1, nums, nums_index, count);
        } else if (nums_index == int(nums.size())) {
            return 0;
        } else if (count == nums.at(nums_index)) {
            return rec(field, field_index+1, nums, nums_index+1, 0);
        } else {
            return 0;
        }
    } else {
        int if_damaged = rec(field, field_index+1, nums, nums_index, count+1);
        int if_operational;
        if (count == 0) {
            if_operational = rec(field, field_index+1, nums, nums_index, count);
        } else if (nums_index == int(nums.size())) {
            if_operational = 0;
        } else if (count == nums.at(nums_index)) {
            if_operational = rec(field, field_index+1, nums, nums_index+1, 0);
        } else {
            if_operational = 0;
        }
        return if_damaged + if_operational;
    }
}

int main() {
    string line;
    int result = 0;
    while (getline(cin, line)) {
        auto [field, nums] = parse_line(line);
        field.push_back('.');
        result += rec(field, 0, nums, 0, 0);
    }
    cout << result << endl;
    return 0;
}
