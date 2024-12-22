#include <iostream>
#include <limits>
#include <cassert>
#include <array>
#include <vector>
#include <string>
#include <queue>

using namespace std;

using ii = pair<int, int>;

enum Direction {
    Down = 0,
    Right = 1,
    Up = 2,
    Left = 3,
};

struct Node {
    ii pos;
    int total_cost = -1;
    Direction dir;
    int dir_counter = 0;
};

bool valid_direction(Direction d1) {
    return 0 <= d1 && d1 < 4;
}

bool opposite_directions(Direction d1, Direction d2) {
    switch (d1) {
        case Down: return d2 == Up;
        case Right: return d2 == Left;
        case Up: return d2 == Down;
        case Left: return d2 == Right;
    }
    assert(!"Invalid directions");
}

int main() {
    vector<string> lines;
    while (lines.emplace_back(), getline(cin, lines.back()));
    lines.pop_back();
    auto pq_comp = [](const Node& n1, const Node& n2) {
        return n1.total_cost > n2.total_cost;
    };
    priority_queue<Node, vector<Node>, decltype(pq_comp)> pq(pq_comp);
    vector<vector<array<array<bool, 3>, 4>>> visited(
        lines.size(), 
        vector(lines.at(0).size(), array<array<bool, 3>, 4>{})
    );
    {
        Node start;
        start.pos = ii(0, 0);
        start.total_cost = 0;
        start.dir = Down;
        start.dir_counter = 0;
        pq.push(start);
    }
    int result = -1;
    while (pq.size() > 0) {
        Node n = pq.top();
        pq.pop();
        if (n.pos == ii(int(lines.size())-1, int(lines.at(0).size())-1)) {
            result = n.total_cost;
            break;
        }
        if (n.pos == ii(0, 0)) n.dir_counter = 1;
        if (visited.at(n.pos.first).at(n.pos.second).at(n.dir).at(n.dir_counter-1)) {
            continue;
        }
        visited.at(n.pos.first).at(n.pos.second).at(n.dir).at(n.dir_counter-1) = true;
        if (n.pos == ii(0, 0)) n.dir_counter = 0;
        const array<pair<int, int>, 4> offsets = {{{1,0},{0,1},{-1,0},{0,-1}}};
        for (int d = 0; d < int(offsets.size()); d++) {
            Direction dir = Direction(d);
            if (opposite_directions(dir, n.dir)) {
                continue;
            }
            int dir_counter = dir == n.dir ? n.dir_counter + 1 : 1;
            if (dir_counter > 3) {
                continue;
            }
            int ai = n.pos.first + offsets.at(d).first;
            int aj = n.pos.second + offsets.at(d).second;
            if (ai < 0 || ai >= int(lines.size()) || aj < 0 || aj >= int(lines.at(0).size())) {
                continue;
            }
            Node adjacent;
            adjacent.pos = ii(ai, aj);
            adjacent.total_cost = n.total_cost + (lines.at(ai).at(aj)-'0');
            adjacent.dir = dir;
            adjacent.dir_counter = dir_counter;
            pq.push(adjacent);
        }
    }
    cout << result << endl;
    return 0;
}
