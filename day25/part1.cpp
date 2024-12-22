#include <iostream>
#include <cassert>
#include <unordered_map>
#include <vector>

using namespace std;

vector<int> dfs_num, dfs_low, dfs_parent;
int dfs_number_counter, dfs_root;
vector<vector<pair<int, int>>> graph;
int f3 = -1, t3 = -1;

int dfs(int u) {
    int result = 1;
    dfs_num[u] = dfs_number_counter++;
    dfs_low[u] = dfs_num[u];
    for (auto& [w, v] : graph[u]) {
        if (w == 0) {
            continue;
        }
        if (dfs_num[v] == -1) {
            dfs_parent[v] = u;

            result += dfs(v);

            if (dfs_low[v] > dfs_num[u]) {
                f3 = u;
                t3 = v;
            }

            dfs_low[u] = min(dfs_low[u], dfs_low[v]);
        } else if (v != dfs_parent[u]) {
            dfs_low[u] = min(dfs_low[u], dfs_num[v]);
        }
    }
    return result;
}

int main() {
    unordered_map<string, int> all_strings;
    vector<string> strings;
    int string_counter = 0;
    while (true) {
        string name;
        if (!(cin >> name)) {
            break;
        }
        name.pop_back();
        int name_id;
        auto it = all_strings.find(name);
        if (it == all_strings.end()) {
            name_id = string_counter;
            graph.emplace_back();
            all_strings.emplace(name, string_counter++);
            strings.push_back(name);
        } else {
            name_id = it->second;
        }
        while (cin.get() != '\n') {
            string adjacent;
            assert(cin >> adjacent);
            int adjacent_id;
            auto it2 = all_strings.find(adjacent);
            if (it2 == all_strings.end()) {
                adjacent_id = string_counter;
                graph.emplace_back();
                all_strings.emplace(adjacent, string_counter++);
                strings.push_back(adjacent);
            } else {
                adjacent_id = it2->second;
            }
            graph.at(name_id).emplace_back(1, adjacent_id);
            graph.at(adjacent_id).emplace_back(1, name_id);
        }
    }
    for (int f1 = 0; f1 < int(graph.size()); f1++) {
        for (auto& [w1, t1] : graph.at(f1)) {
            
            if (f1 > t1) continue;
            
            w1 = 0;
            for (auto& [w, u] : graph.at(t1)) {
                if (u == f1) { w = 0; break; }
            }
            
            for (int f2 = f1; f2 < int(graph.size()); f2++) {
                for (auto& [w2, t2] : graph.at(f2)) {
                    
                    if (f2 > t2 || (f1 == f2 && t1 >= t2)) continue;
                    
                    w2 = 0;
                    for (auto& [w, u] : graph.at(t2)) {
                        if (u == f2) { w = 0; break; }
                    }
                    
                    dfs_num.assign(graph.size(), -1); dfs_low.assign(graph.size(), 0);
                    dfs_parent.assign(graph.size(), -1);
                    dfs_number_counter = 0;
                    for (int u = 0; u < int(graph.size()); u++) {
                        if (dfs_num.at(u) == -1) {
                            dfs_root = u;
                            dfs(u);
                        }
                    }
                    if (f3 != -1) {
                        for (auto& [w, s] : graph.at(f3)) {
                            if (s == t3) {
                                w = 0;
                                break;
                            }
                        }
                        for (auto& [w, s] : graph.at(t3)) {
                            if (s == f3) {
                                w = 0;
                                break;
                            }
                        }
                        goto end;
                    }
                    
                    for (auto& [w, u] : graph.at(t2)) {
                        if (u == f2) { w = 1; break; }
                    }
                    w2 = 1;
                }
                
            }
            
            for (auto& [w, u] : graph.at(t1)) {
                if (u == f1) { w = 1; break; }
            }
            w1 = 1;
        }
    }
    end:
    dfs_num.assign(graph.size(), -1); dfs_low.assign(graph.size(), 0);
    dfs_parent.assign(graph.size(), -1);
    dfs_number_counter = 0;
    int r = 1;
    for (int u = 0; u < int(graph.size()); u++) {
        if (dfs_num.at(u) == -1) {
            dfs_root = u;
            r *= dfs(u);
        }
    }
    cout << r << '\n';
    return 0;
}
