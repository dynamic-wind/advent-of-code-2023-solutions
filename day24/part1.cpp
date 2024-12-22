#include <iostream>
#include <cassert>
#include <utility>
#include <vector>
#include <limits>
#include <optional>

using namespace std;

bool approximately_equal(double a, double b, double epsilon) {
    return abs(a-b) < epsilon;
}

optional<pair<double, double>> future_intersection_point(
    const pair<double, double>& p1,
    const pair<double, double>& v1,
    const pair<double, double>& p2,
    const pair<double, double>& v2,
    double epsilon)
{
    const auto& [x1, y1] = p1;
    const auto& [x2, y2] = p2;
    const auto& [vx1, vy1] = v1;
    const auto& [vx2, vy2] = v2;
    const double num = (y1-y2)*vx2 - (x1-x2)*vy2;
    const double den = vx1*vy2 - vy1*vx2;
    
    if (approximately_equal(den, 0.0, epsilon)) {
        return nullopt;
    }
    
    const double t1 = num/den;
    if (t1 < 0.0) {
        return nullopt;
    }
    
    const double t2 =
        approximately_equal(vy2, 0.0, epsilon)
        ? (x1+vx1*t1-x2)/vx2
        : (y1+vy1*t1-y2)/vy2;
    
    if (t2 < 0.0) {
        return nullopt;
    }
    
    return pair(x1 + vx1*t1, y1 + vy1*t1);
}

bool is_between(double value, double low, double high, double epsilon) {
    return low-epsilon <= value && value <= high+epsilon;
}

int main() {
    vector<pair<double, double>> positions;
    vector<pair<double, double>> velocities;
    while (true) {
        double x, y;
        if (!(cin >> x)) break;
        assert(cin.ignore(1));
        assert(cin >> y);
        assert(cin.ignore(numeric_limits<streamsize>::max(), '@'));
        positions.emplace_back(x, y);
        double vx, vy;
        assert(cin >> vx);
        assert(cin.ignore(1));
        assert(cin >> vy);
        assert(cin.ignore(numeric_limits<streamsize>::max(), '\n'));
        velocities.emplace_back(vx, vy);
    }
    assert(positions.size() == velocities.size());
    const int n = int(positions.size());
    const double low = 2.0e14;
    const double high = 4.0e14;
    int result = 0;
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            const auto& p1 = positions.at(i);
            const auto& p2 = positions.at(j);
            const auto& v1 = velocities.at(i);
            const auto& v2 = velocities.at(j);
            const auto& opt_ip = future_intersection_point(p1, v1, p2, v2, 0.0001);
            if (opt_ip.has_value()) {
                const auto& [ix, iy] = opt_ip.value();
                if (is_between(ix, low, high, 0.0001) && is_between(iy, low, high, 0.0001)) {
                    result++;
                }
            }
        }
    }
    cout << result << '\n';
    return 0;
}
