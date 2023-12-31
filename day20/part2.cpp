#include <iostream>
#include <cassert>
#include <vector>
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

void reset_all_modules(unordered_map<string, Module>& modules) {
    for (auto& [name, mod] : modules) {
        mod.last_pulse_sent = 0;
        mod.low_inputs_count = 0;
    }
    for (auto& [name, mod] : modules) {
        for (const string& output : mod.outputs) {
            auto it = modules.find(output);
            if (it != modules.end()) {
                it->second.low_inputs_count++;
            }
        }
    }
}

int find_module_cycle(unordered_map<string, Module>& modules, const string& module_name) {
    reset_all_modules(modules);
    vector<tuple<bool, bool, reference_wrapper<const string>>> pulses;
    const string broadcaster_name = "broadcaster";
    int rx_count = 0;

    for (int i = 1; true; i++) {
        rx_count = 0;
        pulses.emplace_back(0, 0, broadcaster_name);
        while (pulses.size() > 0) {
            decltype(pulses) new_pulses;
            for (const auto& [prev_pulse, pulse, name] : pulses) {
                if (pulse == 0 && name.get() == "rx") rx_count++;
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
                        if (mod.name == module_name) {
                            return i;
                        }
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
}

int main() {
    unordered_map<string, Module> modules;
    string line;
    while (getline(cin, line)) {
        Module m = parse_line(line);
        string name = m.name;
        modules.emplace(move(name), move(m));
    }
    string outputs_to_rx;
    for (auto& [name, mod] : modules) {
        auto it = find(mod.outputs.begin(), mod.outputs.end(), "rx");
        if (it != mod.outputs.end()) {
            outputs_to_rx = name;
            break;
        }
    }
    assert(outputs_to_rx != "");
    vector<string> outputs_to_outputs_to_rx;
    for (auto& [name, mod] : modules) {
        auto it = find(mod.outputs.begin(), mod.outputs.end(), outputs_to_rx);
        if (it != mod.outputs.end()) {
            outputs_to_outputs_to_rx.push_back(name);
        }
    }

    int64_t result = 1;
    for (const string& s : outputs_to_outputs_to_rx) {
        result *= find_module_cycle(modules, s);
    }
    cout << result << endl;
    return 0;
}
