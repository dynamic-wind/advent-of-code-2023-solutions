#include <iostream>
#include <limits>
#include <vector>

using namespace std;

using ll = long long;

ll digit_concat(ll a, ll b) {
    ll power_of_ten = 1;
    while (power_of_ten <= b) power_of_ten *= 10;
    return a * power_of_ten + b;
}

ll parse_line() {
    ll result = 0;
    cin.ignore(numeric_limits<streamsize>::max(), ' ');
    { ll n; while (cin >> n) result = digit_concat(result, n); }
    cin.clear(); // clear the failbit
    return result;
}

int main() {
    const ll total_time = parse_line();
    const ll record_distance = parse_line();
    
    ll number_of_ways_to_beat_record = 0;
    for (ll time_holding_button = 0; time_holding_button <= total_time; time_holding_button++) {
        const ll distance = time_holding_button * (total_time - time_holding_button);
        if (distance > record_distance) {
            number_of_ways_to_beat_record++;
        }
    }
    cout << number_of_ways_to_beat_record << '\n';
    return 0;
}
