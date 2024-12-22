#include <iostream>
#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <set>
#include <utility>

using namespace std;

pair<int, int> find_start(const vector<string>& lines) {
    for (int i = 0; i < int(lines.size()); i++) {
        for (int j = 0; j < int(lines.at(i).size()); j++) {
            if (lines.at(i).at(j) == 'S') {
                return {i, j};
            }
        }
    }
    cerr << "No start" << endl; terminate();
}

int mod(int a, int b) {
    int m = a % b;
    if (m < 0) {
        return m+b;
    } else {
        return m;
    }
}

int main() {
    const array<pair<int, int>, 4> offsets = {{{1, 0}, {0, 1}, {-1, 0}, {0, -1}}};
    vector<string> lines;
    while (lines.emplace_back(), getline(cin, lines.back()));
    lines.pop_back();
    const int rows = int(lines.size());
    const int cols = int(lines.at(0).size());
    auto [si, sj] = find_start(lines);
    set<pair<int, int>> current_positions = {{si, sj}};
    for (int i = 0; i < rows/2; i++) {
        set<pair<int, int>> next_positions;
        for (const auto& [ni, nj] : current_positions) {
            for (const auto& [oi, oj] : offsets) {
                int ai = ni + oi;
                int aj = nj + oj;
                if (lines.at(mod(ai, rows)).at(mod(aj, cols)) != '#') {
                    next_positions.emplace(ai, aj);
                }
            }
        }
        current_positions = move(next_positions);
    }
    const int b1 = current_positions.size();
    
    auto copy = lines;
    for (const auto& [ni, nj] : current_positions) {
        for (int j = nj; j < cols; j++) copy.at(ni).at(j) = ' ';
        for (int i = ni; i < rows; i++) copy.at(i).at(nj) = ' ';
    }
    int c1 = 0;
    int c2 = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (copy.at(i).at(j) == '.') {
                if ((i+j)%2 == 0) {
                    c1++;
                } else {
                    c2++;
                }
            }
        }
    }
    const int ul1 = min(c1, c2);
    
    copy = lines;
    for (const auto& [ni, nj] : current_positions) {
        for (int j = nj; j >= 0; j--) copy.at(ni).at(j) = ' ';
        for (int i = ni; i < rows; i++) copy.at(i).at(nj) = ' ';
    }
    c1 = 0;
    c2 = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (copy.at(i).at(j) == '.') {
                if ((i+j)%2 == 0) {
                    c1++;
                } else {
                    c2++;
                }
            }
        }
    }
    const int ur1 = min(c1, c2);
    
    copy = lines;
    for (const auto& [ni, nj] : current_positions) {
        for (int j = nj; j < cols; j++) copy.at(ni).at(j) = ' ';
        for (int i = ni; i >= 0; i--) copy.at(i).at(nj) = ' ';
    }
    c1 = 0;
    c2 = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (copy.at(i).at(j) == '.') {
                if ((i+j)%2 == 0) {
                    c1++;
                } else {
                    c2++;
                }
            }
        }
    }
    const int dl1 = min(c1, c2);
    
    copy = lines;
    for (const auto& [ni, nj] : current_positions) {
        for (int j = nj; j >= 0; j--) copy.at(ni).at(j) = ' ';
        for (int i = ni; i >= 0; i--) copy.at(i).at(nj) = ' ';
    }
    c1 = 0;
    c2 = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (copy.at(i).at(j) == '.') {
                if ((i+j)%2 == 0) {
                    c1++;
                } else {
                    c2++;
                }
            }
        }
    }
    const int dr1 = min(c1, c2);
    
    current_positions = {{si, sj}};
    for (int i = 0; i < rows/2-1; i++) {
        set<pair<int, int>> next_positions;
        for (const auto& [ni, nj] : current_positions) {
            for (const auto& [oi, oj] : offsets) {
                int ai = ni + oi;
                int aj = nj + oj;
                if (lines.at(mod(ai, rows)).at(mod(aj, cols)) != '#') {
                    next_positions.emplace(ai, aj);
                }
            }
        }
        current_positions = move(next_positions);
    }
    const int b2 = current_positions.size();
    
    copy = lines;
    for (const auto& [ni, nj] : current_positions) {
        for (int j = nj; j < cols; j++) copy.at(ni).at(j) = ' ';
        for (int i = ni; i < rows; i++) copy.at(i).at(nj) = ' ';
    }
    c1 = 0;
    c2 = 0;
    for (int i = 0; i < rows/2+1; i++) {
        for (int j = 0; j < cols/2+1; j++) {
            if (copy.at(i).at(j) == '.') {
                if ((i+j)%2 == 0) {
                    c1++;
                } else {
                    c2++;
                }
            }
        }
    }
    const int ul2 = min(c1, c2);
    
    copy = lines;
    for (const auto& [ni, nj] : current_positions) {
        for (int j = nj; j >= 0; j--) copy.at(ni).at(j) = ' ';
        for (int i = ni; i < rows; i++) copy.at(i).at(nj) = ' ';
    }
    c1 = 0;
    c2 = 0;
    for (int i = 0; i < rows/2+1; i++) {
        for (int j = cols/2+1; j < cols; j++) {
            if (copy.at(i).at(j) == '.') {
                if ((i+j)%2 == 0) {
                    c1++;
                } else {
                    c2++;
                }
            }
        }
    }
    const int ur2 = min(c1, c2);
    
    copy = lines;
    for (const auto& [ni, nj] : current_positions) {
        for (int j = nj; j < cols; j++) copy.at(ni).at(j) = ' ';
        for (int i = ni; i >= 0; i--) copy.at(i).at(nj) = ' ';
    }
    c1 = 0;
    c2 = 0;
    for (int i = rows/2+1; i < rows; i++) {
        for (int j = 0; j < cols/2+1; j++) {
            if (copy.at(i).at(j) == '.') {
                if ((i+j)%2 == 0) {
                    c1++;
                } else {
                    c2++;
                }
            }
        }
    }
    const int dl2 = min(c1, c2);
    
    copy = lines;
    for (const auto& [ni, nj] : current_positions) {
        for (int j = nj; j >= 0; j--) copy.at(ni).at(j) = ' ';
        for (int i = ni; i >= 0; i--) copy.at(i).at(nj) = ' ';
    }
    c1 = 0;
    c2 = 0;
    for (int i = rows/2+1; i < rows; i++) {
        for (int j = cols/2+1; j < cols; j++) {
            if (copy.at(i).at(j) == '.') {
                if ((i+j)%2 == 0) {
                    c1++;
                } else {
                    c2++;
                }
            }
        }
    }
    const int dr2 = min(c1, c2);
    
    const int b3 = dr1 + dl2 + ur2 + ul1;
    const int b4 = dr2 + dl1 + ur1 + ul2;
    
    const long long n = 26501365;
    const long long m = (n - rows/2)/rows;
    const long long o = 2*m+1;
    const long long s = (o*o - (o/2+1)*(o/2+1) - (o/2)*(o/2)) / 2;
    const long long r = (o/2+1)*(o/2+1)*b1 + (o/2)*(o/2)*b2 + s*b3 + s*b4;
    
    cout << r << '\n';
    return 0;
}
