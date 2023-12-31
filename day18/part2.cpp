#include <iostream>
#include <cstdint>
#include <optional>
#include <set>
#include <cassert>
#include <utility>
#include <limits>
#include <vector>
#include <string>

using namespace std;

pair<int, int> direction_to_offset(char ch) {
    switch (ch) {
        case 'U': return {-1, 0};
        case 'D': return {1, 0};
        case 'L': return {0, -1};
        case 'R': return {0, 1};
    }
    cerr << "Character not a direction: '" << ch << '\'' << endl;
    terminate();
}

vector<pair<char, int64_t>> read_input() {
    vector<pair<char, int64_t>> result;
    while (true) {
        if (!cin.ignore(numeric_limits<streamsize>::max(), '#')) {
            break;
        }
        int64_t num;
        if (!(cin >> hex >> num)) {
            break;
        }
        char ch = ("RDLU"s).at(num%16);
        num /= 16;
        result.emplace_back(ch, num);
    }
    return result;
}

vector<pair<int64_t, int64_t>> get_points(const vector<pair<char, int64_t>>& input) {
    vector<pair<int64_t, int64_t>> result;
    result.reserve(input.size());
    int64_t i = 0;
    int64_t j = 0;
    for (const auto& [ch, n] : input) {
        auto [oi, oj] = direction_to_offset(ch);
        result.emplace_back(i, j);
        i += n*oi;
        j += n*oj;
    }
    return result;
}

struct wall {
    int64_t i_top;
    int64_t i_bottom;
    int64_t j;
    
    wall(int64_t i_top, int64_t i_bottom, int64_t j) : i_top(i_top), i_bottom(i_bottom), j(j) {}

    bool operator<(const wall& other) const {
        return tie(i_top, i_bottom, j) < tie(other.i_top, other.i_bottom, other.j);
    }
};

wall find_matching_wall(const set<wall>& walls, const wall& w) {
    optional<wall> closest_left;
    int left_count = 0;
    optional<wall> closest_right;
    int right_count = 0;
    for (const wall& w2 : walls) {
        if (w2.i_top <= w.i_top && w.i_top < w2.i_bottom) {
            if (w2.j < w.j) {
                left_count++;
            } else if (w.j < w2.j) {
                right_count++;
            }
        }
        if (w.i_top == w2.i_top && w.i_bottom == w2.i_bottom) {
            if (w2.j < w.j) {
                if (!closest_left.has_value() || closest_left.value().j < w2.j) {
                    closest_left = w2;
                }
            } else if (w.j < w2.j) {
                if (!closest_right.has_value() || w2.j < closest_right.value().j) {
                    closest_right = w2;
                }
            }
        }
    }
    assert((left_count%2) != (right_count%2));
    if (left_count % 2 == 1) {
        return closest_left.value();
    } else {
        return closest_right.value();
    }
}

int main() {
    auto input = read_input();
    auto points = get_points(input);

    set<wall> walls;
    for (int k = 0; k < int(points.size()); k++) {
        const auto& [i1, j1] = points.at(k);
        const auto& [i2, j2] = points.at((k+1) % points.size());
        if (j1 == j2) {
            walls.emplace(min(i1, i2), max(i1, i2), j1);
        }
    }
    for (const auto& [i, j] : points) {
        optional<wall> left_wall;
        int left_walls_count = 0;
        optional<wall> right_wall;
        int right_walls_count = 0;
        for (const wall& w : walls) {
            if (w.i_top <= i && i <= w.i_bottom) {
                if (w.j < j) {
                    if (w.i_bottom != i) {
                        left_walls_count++;
                    }
                    if (!left_wall.has_value() || left_wall.value().j < w.j) {
                        left_wall = w;
                    }
                } else if (j < w.j) {
                    if (w.i_bottom != i) {
                        right_walls_count++;
                    }
                    if (!right_wall.has_value() || w.j < right_wall.value().j) {
                        right_wall = w;
                    }
                }
            }
        }
        if (right_walls_count % 2 == 1 &&
            right_wall.has_value() &&
            i != right_wall.value().i_top &&
            i != right_wall.value().i_bottom)
        {
            walls.erase(right_wall.value());
            walls.emplace(right_wall.value().i_top, i, right_wall.value().j);
            walls.emplace(i, right_wall.value().i_bottom, right_wall.value().j);
        }
        if (left_walls_count % 2 == 1 &&
            left_wall.has_value() &&
            i != left_wall.value().i_top &&
            i != left_wall.value().i_bottom)
        {
            walls.erase(left_wall.value());
            walls.emplace(left_wall.value().i_top, i, left_wall.value().j);
            walls.emplace(i, left_wall.value().i_bottom, left_wall.value().j);
        }
    }
    int64_t result = 0;
    for (const wall& w : walls) {
        wall closest = find_matching_wall(walls, w);
        if (w.j < closest.j) {
            result += int64_t(closest.j-w.j+1)*int64_t(w.i_bottom-w.i_top+1);
        }
        for (const wall& w2 : walls) {
            if (w.i_bottom == w2.i_top) {
                wall w3 = find_matching_wall(walls, w2);
                if (w2.j < w3.j) {
                    int64_t left = max(w.j, w2.j);
                    int64_t right = min(closest.j, w3.j);
                    if (left <= right) {
                        result -= right-left+1;
                    }
                }
            }
        }
    }
    cout << result << endl;
    return 0;
}
