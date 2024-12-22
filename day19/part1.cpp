#include <iostream>
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

    Rule(Category category, Comparison comparison, int number, string workflow) :
        category(category),
        comparison(comparison),
        number(number),
        workflow(workflow)
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
            conditional_workflow
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

int main() {
    auto workflows = read_all_workflows();
    auto ratings = read_all_ratings();

    int result = 0;
    for (const Rating& rating : ratings) {
        string workflow_name = "in";
        while (workflow_name != "R" && workflow_name != "A") {
            const Workflow& workflow = workflows.at(workflow_name);
            bool any_match = false;
            for (const Rule& rule : workflow.rules) {
                if (rule.comparison == Greater) {
                    if (rating.at(rule.category) > rule.number) {
                        workflow_name = rule.workflow;
                        any_match = true;
                        break;
                    }
                } else if (rule.comparison == Smaller) {
                    if (rating.at(rule.category) < rule.number) {
                        workflow_name = rule.workflow;
                        any_match = true;
                        break;
                    }
                } else {
                    cerr << "Invalid comparison: " << rule.comparison << endl;
                    terminate();
                }
            }
            if (!any_match) {
                workflow_name = workflow.else_workflow;                
            }
        }
        if (workflow_name == "A") {
            result += rating.at(0) + rating.at(1) + rating.at(2) + rating.at(3);
        }
    }
    cout << result << endl;
    return 0;
}
