#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <array>

using namespace std;

int parse_card(char ch) {
    if ('2' <= ch && ch <= '9') return ch - '0';
    switch (ch) {
        case 'T': return 10;
        case 'J': return 1;
        case 'Q': return 12;
        case 'K': return 13;
        case 'A': return 14;
    }
    cout << "'" << int(ch) << "'" << endl;
    terminate();
}

struct card_count {
    int count;
    int card;
};

struct hand {
    string string;
    vector<card_count> card_counts;
};

hand parse_hand(const string& str) {
    hand result;
    result.string = str;
    
    string sorted = str;
    sort(sorted.begin(), sorted.end());
    int jokers = 0;
    for (char ch : sorted) {
        if (ch == 'J') {
            jokers++;
        } else {
            int card = parse_card(ch);
            if (result.card_counts.size() == 0 || result.card_counts.back().card != card) {
                card_count& last = result.card_counts.emplace_back();
                last.count = 1;
                last.card = card;
            } else {
                result.card_counts.back().count++;
            }
        }
    }
    if (result.card_counts.size() == 0) {
        card_count& c = result.card_counts.emplace_back();
        c.count = 1;
        c.card = 14;
        jokers--;
    }
    sort(
        result.card_counts.begin(), 
        result.card_counts.end(), 
        [](const auto& c1, const auto& c2) {
            return c1.count > c2.count;
        }
    );
    result.card_counts.at(0).count += jokers;
    return result;
}

struct bid {
    hand hand;
    int amount;
};

vector<bid> read_bids() {
    vector<bid> result;
    string str;
    while (cin >> str) {
        int n;
        cin >> n;
        bid& bid = result.emplace_back();
        bid.hand = parse_hand(str);
        bid.amount = n;
    }
    return result;
}

bool weaker_hand(const hand& h1, const hand& h2) {
    if (h1.card_counts.at(0).count != h2.card_counts.at(0).count) {
        return h1.card_counts.at(0).count < h2.card_counts.at(0).count;
    }
    if (h1.card_counts.size() > 1 && h2.card_counts.size() > 1 &&
        h1.card_counts.at(1).count != h2.card_counts.at(1).count)
    {
        return h1.card_counts.at(1).count < h2.card_counts.at(1).count;
    }
    for (int i = 0; i < int(h1.string.size()); i++) {
        int c1 = parse_card(h1.string.at(i));
        int c2 = parse_card(h2.string.at(i));
        if (c1 != c2) return c1 < c2;
    }
    return false;
}

int main() {
    auto bids = read_bids();
    sort(bids.begin(), bids.end(), [](const auto& b1, const auto& b2) {
        return weaker_hand(b1.hand, b2.hand);
    });
    long long result = 0;
    for (int i = 0; i < int(bids.size()); i++) {
        result += static_cast<long long>(i+1) * static_cast<long long>(bids.at(i).amount);
    }
    cout << result << '\n';
    return 0;
}
