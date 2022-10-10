#include <iostream>
#include "bowling_scores.h"
using namespace std;

Player::Player(const string& fstname, const string& lstname) {
    first_ = fstname;
    last_ = lstname;
}
bool operator< (const Player& left, const Player& right) {
    if ((left.getLast() < right.getLast()) || (left.getLast() == right.getLast() && left.getFirst() < right.getFirst())) {
        return true;
    } else {
        return false;
    }
}
ostream& operator<< (ostream& ostr, const Player& n) {
    ostr << n.getFirst() << " " << n.getLast();
    return ostr;
}
int find_position(unsigned start_pos, vector<int> &scores, unsigned int search_for) {
    unsigned int current_pos = start_pos;
    for (current_pos; current_pos < scores.size(); ++current_pos) {
        if (scores[current_pos] == search_for) {
            return (current_pos);
        }
    }
    return (0);
}
void Player::total(unsigned int pp) {
    total_ = pp;
}
void Player::addScore(unsigned int x) {
    player_scores_.push_back(x);
}
ostream& print_frame_scores (ostream& out_str, int longest_name, vector<int> frame_scores) {
    const string empty(longest_name, ' ');
    out_str << "| " << empty << " | ";
    unsigned int frame = 0;
    for (frame; frame < 9; ++frame) {
        if (frame == 0) {
            string frame_space( 3-(to_string(recursiveTotal(frame_scores, frame)).size()), ' ');
            out_str << frame_space << frame_scores[frame] << " | ";
        }
        else {
            string frame_space( 3-(to_string(recursiveTotal(frame_scores, frame)).size()), ' ');
            out_str << frame_space << recursiveTotal(frame_scores, frame) << " | ";
        }
    }
    if (frame == 9) {
        int total = getTotal(frame_scores);
        string end_space(5-(to_string(total).size()), ' ');
        out_str << end_space << total << " |" << endl;
    }
    return out_str;
}

int getTotal(vector<int>scores) {
    unsigned int sum = 0;
    for (unsigned int x = 0; x < scores.size(); ++x) {
        sum += scores[x];
    }
    return sum;
}
int recursiveTotal (vector<int> scores, int a ) {
    if (a > 0) {
        return (scores[a] + recursiveTotal(scores, a - 1));
    } else {
        return (scores[0]);
    }
}
bool sort_scores(const Player& player1, const Player& player2) {
    return player1.pp() > player2.pp() ||
    (player1.pp() == player2.pp() && player1.getLast() < player2.getLast());
}
