#ifndef __bowling_scores_h_
#define __bowling_scores_h_

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Player {
public:
    Name(const string& fst = "", const string& lst = "");

    void setFirst (const string& fst) { first_ = fst; }
    void setLast (const string& lst) { last_ = lst; }

    const string getFirst ( string& first()) const { return first_; }
    const string getLast ( string& last()) const {return last_; }

    int getTotal const;
private:
    string first_, last_;
    vector<int> scores_;
};

bool operator< (const Player& left, const Player& right);

#endif
