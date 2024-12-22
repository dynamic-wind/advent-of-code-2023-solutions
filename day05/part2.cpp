#include <iostream>
#include <tuple>
#include <vector>
#include <limits>
#include <string>

using namespace std;

using ll = long long;

// returns vector<interval[start, end)>
vector<tuple<ll, ll>> read_seeds() {
    cin.ignore(numeric_limits<streamsize>::max(), ':');
    vector<tuple<ll, ll>> result;
    while (cin.get() != '\n') {
        ll num, amount;
        cin >> num >> amount;
        result.emplace_back(num, num + amount);
    }
    return result;
}

// returns vector<(output_start, input_start, amount)>
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
    for (auto& map : maps) {
        // the seed intervals after the mapping
        // are to be placed in here
        vector<tuple<ll, ll>> next_seeds;
        next_seeds.reserve(seeds.size());
        while (seeds.size() != 0) {
            // very important to make a copy here, otherwise it would potentially
            // be a dangling reference
            const auto [seed_start, seed_end] = seeds.back();
            seeds.pop_back();
            bool mapped = false;
            for (auto& [output_start, input_start, amount] : map) {
                const ll input_end = input_start + amount;
                
                /* interval which can be mapped
                 * Example: 
                 *   seed:  ###########
                 *   input:    ##########
                 *   to_map:   ########
                 * it can be thought of as the intersection of the intervals */
                const ll to_map_start = max(seed_start, input_start);
                const ll to_map_end = min(seed_end, input_end);
                
                if (to_map_start < to_map_end) {
                    
                    /*
                     * Example: 
                     *   input_start = 10
                     *   output_start = 11
                     *   to_map_start = 16
                     *      
                     * 10 11 12 13 14 15 16 17 18 19 20
                     * input_start       to_map_start
                     *  \                 \
                     *   \                 \
                     *    output_start      mapped_start
                     *    
                     *    mapped_start = 17 */
                    const ll mapped_start = output_start + (to_map_start - input_start);
                    const ll mapped_end = output_start + (to_map_end - input_start);
                    
                    next_seeds.emplace_back(mapped_start, mapped_end);
                    
                    // add back to seeds parts of the seeds which weren't mapped
                    if (seed_start < to_map_start) {
                        seeds.emplace_back(seed_start, to_map_start);
                    }
                    if (to_map_end < seed_end) {
                        seeds.emplace_back(to_map_end, seed_end);
                    }
                    mapped = true;
                    break;
                }
            }
            // if no map can change the seed interval then
            // it is passed unchanged
            if (!mapped) {
                next_seeds.emplace_back(seed_start, seed_end);
            }
        }
        seeds = move(next_seeds);
    }
    ll result = numeric_limits<ll>::max();
    for (auto& [seed_start, seed_end] : seeds) {
        result = min(result, seed_start);
    }
    cout << result << '\n';
    return 0;
}
