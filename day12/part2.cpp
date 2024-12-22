#include <iostream>
#include <cstdint>
#include <map>
#include <cassert>
#include <vector>
#include <sstream>
#include <string>
#include <utility>
#include <cstdlib>
#include <tuple>

using namespace std;

map<tuple<int, int, int>, int64_t> cache;

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

int64_t rec(const string& field, int field_index, const vector<int>& nums, int nums_index, int count) {
    if (field_index == int(field.size())) {
        return int64_t(nums_index == int(nums.size()));
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
        auto it = cache.find({field_index, nums_index, count});
        if (it != cache.end()) {
            return it->second;
        } else {
            int64_t if_damaged = rec(field, field_index+1, nums, nums_index, count+1);
            int64_t if_operational;
            if (count == 0) {
                if_operational = rec(field, field_index+1, nums, nums_index, count);
            } else if (nums_index == int(nums.size())) {
                if_operational = 0;
            } else if (count == nums.at(nums_index)) {
                if_operational = rec(field, field_index+1, nums, nums_index+1, 0);
            } else {
                if_operational = 0;
            }
            int64_t r = if_damaged + if_operational;
            cache.emplace(tuple(field_index, nums_index, count), r);
            return r;
        }
    }
}

int main() {
    string line;
    int64_t result = 0;
    while (getline(cin, line)) {
        auto [field, nums] = parse_line(line);
        int fs = int(field.size());
        int ns = int(nums.size());
        for (int i = 0; i < 4; i++) {
            field.push_back('?');
            for (int j = 0; j < fs; j++) {
                field.push_back(field.at(j));
            }
            for (int j = 0; j < ns; j++) {
                nums.push_back(nums.at(j));
            }
        }
        field.push_back('.');
        cache.clear();
        result += rec(field, 0, nums, 0, 0);
    }
    cout << result << endl;
    return 0;
}
