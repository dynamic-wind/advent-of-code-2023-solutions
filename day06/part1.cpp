#include <iostream>
#include <limits>
#include <vector>

using namespace std;

int main() {
    vector<int> times;
    cin.ignore(numeric_limits<streamsize>::max(), ' ');
    { int n; while (cin >> n) times.push_back(n); }
    cin.clear(); // clear the failbit
    
    vector<int> records;
    cin.ignore(numeric_limits<streamsize>::max(), ' ');
    { int n; while (cin >> n) records.push_back(n); }
    
    int result = 1;
    for (int i = 0; i < int(times.size()); i++) {
        const int total_time = times.at(i);
        const int record_distance = records.at(i);
        int number_of_ways_to_beat_record = 0;
        for (int time_holding_button = 0; time_holding_button <= total_time; time_holding_button++) {
            const int distance = time_holding_button * (total_time - time_holding_button);
            if (distance > record_distance) {
                number_of_ways_to_beat_record++;
            }
        }
        result *= number_of_ways_to_beat_record;
    }
    cout << result << '\n';
    return 0;
}
