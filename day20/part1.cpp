#include <iostream>
#include <cstdint>
#include <tuple>
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

enum ModuleType {
    FlipFlop,
    Conjunction,
    Broadcast,
};

struct Module {
    string name;
    ModuleType type;
    bool last_pulse_sent;
    vector<string> outputs;
    int low_inputs_count;
};

Module parse_line(const string& line) {
    Module result;
    result.last_pulse_sent = 0;
    result.low_inputs_count = 0;
    auto next_space = line.find(' ');
    if (line.at(0) == 'b') {
        result.type = Broadcast;
        result.name = line.substr(0, next_space);
    } else {
        if (line.at(0) == '%') {
            result.type = FlipFlop;
        } else if (line.at(0) == '&') {
            result.type = Conjunction;
        } else {
            cerr << "Invalid type: " << line.at(0) << endl; terminate();
        }
        result.name = line.substr(1, next_space-1);
    }
    auto i = next_space + 4;
    while (i < line.size()) {
        auto j = line.find(',', i);
        if (j == line.npos) {
            j = line.size();
        }
        result.outputs.push_back(line.substr(i, j-i));
        i = j+2;
    }
    return result;
}

int main() {
    unordered_map<string, Module> modules;
    string line;
    while (getline(cin, line)) {
        Module m = parse_line(line);
        // cout << m.name << " -> ";
        // for (const string& s : m.outputs) cout << s << ' ';
        // cout << endl;
        string name = m.name;
        modules.emplace(move(name), move(m));
    }
    for (auto& [name, mod] : modules) {
        for (const string& output : mod.outputs) {
            auto it = modules.find(output);
            if (it != modules.end()) {
                it->second.low_inputs_count++;
            }
        }
    }
    vector<tuple<bool, bool, reference_wrapper<const string>>> pulses;
    const string broadcaster_name = "broadcaster";
    int64_t low_pulses_sent = 0;
    int64_t high_pulses_sent = 0;
    for (int i = 0; i < 1000; i++) {
        pulses.emplace_back(0, 0, broadcaster_name);
        while (pulses.size() > 0) {
            decltype(pulses) new_pulses;
            for (const auto& [prev_pulse, pulse, name] : pulses) {
                if (pulse == 1) high_pulses_sent++; else low_pulses_sent++;
                auto it = modules.find(name);
                if (it == modules.end()) {
                    continue;
                }
                Module& mod = it->second;
                if (mod.type == FlipFlop) {
                    if (pulse == 0) {
                        for (const string& output : mod.outputs) {
                            new_pulses.emplace_back(mod.last_pulse_sent, !mod.last_pulse_sent, output);
                        }
                        mod.last_pulse_sent ^= 1;
                    }
                } else if (mod.type == Conjunction) {
                    if (pulse == 1 && prev_pulse == 0) {
                        mod.low_inputs_count--;
                    } else if (pulse == 0 && prev_pulse == 1) {
                        mod.low_inputs_count++;
                    }
                    if (mod.low_inputs_count == 0) {
                        for (const string& output : mod.outputs) {
                            new_pulses.emplace_back(mod.last_pulse_sent, 0, output);
                        }
                        mod.last_pulse_sent = 0;
                    } else {
                        for (const string& output : mod.outputs) {
                            new_pulses.emplace_back(mod.last_pulse_sent, 1, output);
                        }
                        mod.last_pulse_sent = 1;
                    }
                } else if (mod.type == Broadcast) {
                    for (const string& output : mod.outputs) {
                        new_pulses.emplace_back(mod.last_pulse_sent, pulse, output);
                    }
                    mod.last_pulse_sent = pulse;
                }
            }
            pulses = move(new_pulses);
        }
    }
    cout << (low_pulses_sent * high_pulses_sent) << endl;
    return 0;
}
