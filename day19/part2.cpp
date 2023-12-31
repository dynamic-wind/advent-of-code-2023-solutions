#include <iostream>
#include <cstdint>
#include <set>
#include <array>
#include <vector>
#include <limits>
#include <cassert>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

enum Category {
    X = 0,
    M = 1,
    A = 2,
    S = 3,
};

Category char_to_category(char ch) {
    switch (ch) {
        case 'x': return X;
        case 'm': return M;
        case 'a': return A;
        case 's': return S;
    }
    cerr << "Character does not name a category: " << ch << endl;
    terminate();
}

enum Comparison {
    Smaller = 0,
    Greater = 1,
};

struct Rule {
    Category category;
    Comparison comparison;
    int number;
    string workflow;

    Rule(Category category, Comparison comparison, int number, const string& workflow) :
        category(category),
        comparison(comparison),
        number(number),
        workflow(workflow)
    {}

    Rule(Category category, Comparison comparison, int number, const string&& workflow) :
        category(category),
        comparison(comparison),
        number(number),
        workflow(move(workflow))
    {}
};

struct Workflow {
    string name;
    vector<Rule> rules;
    string else_workflow;
};

Workflow read_workflow() {
    Workflow result;

    assert(getline(cin, result.name, '{'));
    
    while (true) {
        char category_char;
        assert(cin >> category_char);
        char comparison_char;
        assert(cin >> comparison_char);
        if (comparison_char != '<' && comparison_char != '>') {
            assert(cin.putback(comparison_char));
            assert(cin.putback(category_char));
            break;
        }
        int number;
        assert(cin >> number);
        assert(cin.ignore(1));
        string conditional_workflow;
        assert(getline(cin, conditional_workflow, ','));

        result.rules.emplace_back(
            char_to_category(category_char),
            comparison_char == '<' ? Smaller : Greater,
            number,
            move(conditional_workflow)
        );
    }
    
    assert(getline(cin, result.else_workflow, '}'));
    assert(cin.ignore(numeric_limits<streamsize>::max(), '\n'));

    return result;
}

unordered_map<string, Workflow> read_all_workflows() {
    unordered_map<string, Workflow> result;
    while (true) {
        char ch = cin.get();
        assert(cin);
        if (ch == '\n') {
            break;
        }
        cin.putback(ch);
        Workflow workflow = read_workflow();
        string name = workflow.name;
        result.emplace(move(name), move(workflow));
    }
    return result;
}

using Rating = array<int, 4>;

Rating read_rating() {
    Rating result;
    for (int i = 0; i < 4; i++) {
        cin.ignore(numeric_limits<streamsize>::max(), '=');
        assert(cin >> result.at(i));
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return result;
}

vector<Rating> read_all_ratings() {
    vector<Rating> result;
    while (cin.get(), cin) {
        cin.unget();
        result.push_back(read_rating());
    }
    return result;
}

void find_possible_ranges_rec(
    vector<array<pair<int, int>, 4>>& result,
    const string& workflow_name,
    const array<pair<int, int>, 4>& current_ranges,
    set<pair<string, array<pair<int, int>, 4>>>& visited_states,
    const unordered_map<string, Workflow>& workflows)
{
    if (workflow_name == "A") {
        result.push_back(current_ranges);
        //                                                  unnecessary copy :( could've used pair<string_view, span>
    } else if (workflow_name != "R" && visited_states.count(pair(workflow_name, current_ranges)) == 0) {
        visited_states.emplace(workflow_name, current_ranges);
        
        auto new_ranges = current_ranges;
        const Workflow& workflow = workflows.at(workflow_name);
        for (const Rule& rule : workflow.rules) {
            int& low = new_ranges.at(rule.category).first;
            int& high = new_ranges.at(rule.category).second;
            if (rule.comparison == Greater) {
                int prev_low = low;
                low = max(low, rule.number+1);
                if (low <= high) {
                    find_possible_ranges_rec(result, rule.workflow, new_ranges, visited_states, workflows);
                }
                low = prev_low;
                high = min(high, rule.number);
                if (high < low) {
                    return;
                }
            } else if (rule.comparison == Smaller) {
                int prev_high = high;
                high = min(high, rule.number-1);
                if (low <= high) {
                    find_possible_ranges_rec(result, rule.workflow, new_ranges, visited_states, workflows);
                }
                high = prev_high;
                low = max(low, rule.number);
                if (high < low) {
                    return;
                }
            }
        }
        find_possible_ranges_rec(result, workflow.else_workflow, new_ranges, visited_states, workflows);
    }
}

vector<array<pair<int, int>, 4>> find_possible_ranges(const unordered_map<string, Workflow>& workflows) {
    vector<array<pair<int, int>, 4>> result;
    array<pair<int, int>, 4> ranges;
    for (int i = 0; i < 4; i++) {
        ranges.at(i) = {1, 4000};
    }
    set<pair<string, array<pair<int, int>, 4>>> visited;
    find_possible_ranges_rec(result, "in", ranges, visited, workflows);
    return result;
}

int main() {
    auto workflows = read_all_workflows();

    auto ranges = find_possible_ranges(workflows);

    int64_t result = 0;
    for (const auto& a : ranges) {
        int64_t count = 1;
        for (const auto& [start, end] : a) {
            count *= end-start+1;
        }
        result += count;
    }
    cout << result << endl;
    return 0;
}
