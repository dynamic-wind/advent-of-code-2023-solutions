#include <iostream>
#include <tuple>
#include <vector>
#include <limits>
#include <string>

using namespace std;

using ll = long long;

vector<ll> read_seeds() {
    cin.ignore(numeric_limits<streamsize>::max(), ':');
    vector<ll> result;
    while (cin.get() != '\n') {
        ll num;
        cin >> num;
        result.push_back(num);
    }
    return result;
}

vector<vector<tuple<ll, ll, ll>>> read_maps() {
    vector<vector<tuple<ll, ll, ll>>> result;
    while (cin.get(), !cin.eof()) {
        cin.ignore(numeric_limits<streamsize>::max(), ':');
        auto& map = result.emplace_back();
        ll left, right, amount;
        while (cin >> left >> right >> amount) {
            map.emplace_back(left, right, amount);
        }
        cin.clear();
    }
    return result;
}

int main() {
    auto seeds = read_seeds();
    const auto maps = read_maps();
    for (const auto& map : maps) {
        for (ll& seed : seeds) {
            for (const auto& [l, r, n] : map) {
                if (r <= seed && seed < r + n) {
                    seed = l + (seed-r);
                    break;
                }
            }
        }
    }
    ll result = numeric_limits<ll>::max();
    for (ll seed : seeds) {
        result = min(result, seed);
    }
    cout << result << '\n';
    return 0;
}
