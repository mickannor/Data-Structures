#ifndef __bowling_scores_h_
#define __bowling_scores_h_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Player {
public:
    Player(const string& fstname = "", const string& lstname = "");

    void setFirst (const string& fstname) { first_ = fstname; }
    void setLast (const string& lstname) { last_ = lstname; }
    void addScore (unsigned int x);

    const string getFirst () const { return first_; }
    const string getLast () const {return last_; }
    vector<int> scores_() const {return player_scores_;}

    int getTotal ();
    int pp() const { return total_; }
    void total(unsigned int pp);
private:
    string first_, last_;
    vector<int> player_scores_;
    int total_;
};

bool operator< (const Player& left, const Player& right);
bool sort_scores(const Player& player1, const Player& player2);
ostream& operator<< (ostream& ostr, const Player& n);
ostream& print_frame_scores (ostream& out_str, int longest_name, vector<int> frame_scores);
int find_position(unsigned start_pos, vector<int> &scores, unsigned int search_for);
int getTotal(vector<int>scores);
int recursiveTotal (vector<int> scores, int a );

#endif
