#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include "bowling_scores.h"
using namespace std;


int main(int argc, char const *argv[]) {
    if (argc != 4) {
        cerr << "ERROR: Incorrect number of arguments." << endl;
    }
    ifstream in_str(argv[1]);
    if (!in_str.good()) {
	cerr << "ERROR: Unable to open input file." << endl;
	}
    ofstream out_str(argv[2]);
	if (!out_str.good()) {
	cerr << "ERROR: Unable to open output file." << endl;
	}
    const string s(argv[3]);
    if ((s.compare("standard") != 0 ) && (s.compare("custom") != 0)) {
        cerr << "ERROR: 4th argument is invalid." << endl;
    }
    string first_name;
    string last_name;
    vector<Player> Names;
    vector<int> frame_scores;
    int x;
    vector<string> totals;
    while (in_str >> first_name) {
        in_str >> last_name;
        Player person = Player(first_name, last_name);
        while (in_str >> x) {
            if (x <= 10) {
                person.addScore(x);
            } else {
                cerr << "ERROR: Number of pins knocked down is incorrect" << endl;
                exit(1);
            }
        }
        Names.push_back(person);
        in_str.clear();
    }


    sort(Names.begin(), Names.end());
    unsigned int longest = 0;
    for (unsigned int y = 0; y < Names.size(); ++y) {
        if (((Names[y].getFirst()).size() + (Names[y].getLast()).size() + 1) > longest) {
            longest = (Names[y].getFirst()).size() + (Names[y].getLast()).size() + 1;
        }
    }
    const int width = longest+66;
    const string top_down(width, '-');
    for (unsigned int y = 0; y < Names.size(); ++y) {
        out_str << top_down << endl;
        string spaces( longest - ((Names[y].getFirst()).size() + (Names[y].getLast()).size() + 1), ' ');
        out_str << "| " << Names[y] << spaces << " |";
        // KEEPS TRACK OF THE FRAME
        unsigned int frame_count = 0;
        for (unsigned int j = 0; j < (Names[y].scores_()).size(); ++j) {
            if (frame_count != 9) {
                // PRINTS THE NAME OF THE PLAYER AND THEIR SCORES AND ADDS ONE TO THE FRAME
                if ((Names[y].scores_())[j] != 10 && (Names[y].scores_())[j] + (Names[y].scores_())[j+1] < 10) {
                    if ((Names[y].scores_())[j] != 0 && (Names[y].scores_())[j+1] != 0) {
                        out_str << " " << (Names[y].scores_())[j] << " " << (Names[y].scores_())[j+1] << " |";
                        frame_scores.push_back((Names[y].scores_())[j]+(Names[y].scores_())[j+1]);
                        j += 1;
                        frame_count +=1;
                    } else if ((Names[y].scores_())[j] == 0 && (Names[y].scores_())[j+1] != 0) {
                        out_str << " " << "-" << " " << (Names[y].scores_())[j+1] << " |";
                        frame_scores.push_back(0+(Names[y].scores_())[j+1]);
                        j += 1;
                        frame_count += 1;
                    } else if ((Names[y].scores_())[j] == 0 && (Names[y].scores_())[j+1] == 0) {
                        out_str << " " << "-" << " -" << " |";
                        frame_scores.push_back(0);
                        j += 1;
                        frame_count += 1;
                    }
                    else {
                        out_str << " " << (Names[y].scores_())[j] << " " << "-" << " |";
                        frame_scores.push_back((Names[y].scores_())[j]+(Names[y].scores_())[j+1]);
                        j += 1;
                        frame_count += 1;
                    }
                }
                else if ((Names[y].scores_())[j] != 10 && (Names[y].scores_())[j] + (Names[y].scores_())[j+1] == 10) {
                    if ((Names[y].scores_())[j] == 0 ) {
                        out_str << " -" << " " << "/" << " |";
                        frame_scores.push_back((Names[y].scores_())[j]+(Names[y].scores_())[j+1]+(Names[y].scores_())[j+2]);
                        j += 1;
                        frame_count += 1;
                    } else {
                        out_str << " " << (Names[y].scores_())[j] << " " << "/" << " |";
                        frame_scores.push_back((Names[y].scores_())[j]+(Names[y].scores_())[j+1]+(Names[y].scores_())[j+2]);
                        j += 1;
                        frame_count += 1;
                    }
                }
                else if ((Names[y].scores_())[j] == 10) {
                    out_str << "   " << "X" << " |";
                    frame_count += 1;
                    frame_scores.push_back(10+(Names[y].scores_())[j+1]+(Names[y].scores_())[j+2]);
                }

            } else {
                if ((Names[y].scores_())[j] != 10 && (Names[y].scores_())[j] + (Names[y].scores_())[j+1] < 10) {
                    if ((Names[y].scores_())[j] != 0 && (Names[y].scores_())[j+1] != 0) {
                        out_str << " " << (Names[y].scores_())[j] << " " << (Names[y].scores_())[j+1] << "   |" << endl;
                        frame_scores.push_back((Names[y].scores_())[j]+(Names[y].scores_())[j+1]);
                        print_frame_scores(out_str, longest, frame_scores);
                    } else if ((Names[y].scores_())[j] == 0) {
                        out_str << " " << "-" << " " << (Names[y].scores_())[j+1] << "   |" << endl;
                        frame_scores.push_back(0+(Names[y].scores_())[j+1]);
                        print_frame_scores(out_str, longest, frame_scores);
                    } else if ((Names[y].scores_())[j+1]==0) {
                        out_str << " " << (Names[y].scores_())[j] << " " << "-" << "   |" << endl;
                        frame_scores.push_back((Names[y].scores_())[j]+0);
                        print_frame_scores(out_str, longest, frame_scores);
                    }
                    j += 1;
                }
                //PRINTS THE LAST THREE SCORES FOR A PERSON USING THE FUNCTION IN THE CPP FILE
                else if ((Names[y].scores_())[j] != 10 && (Names[y].scores_())[j] + (Names[y].scores_())[j+1] == 10) {
                    if ((Names[y].scores_())[j] == 0) {
                        out_str << " - /   |" << endl;
                        frame_scores.push_back(0+10+(Names[y].scores_())[j+2]);
                        print_frame_scores(out_str, longest, frame_scores);
                    }
                    else if ((Names[y].scores_())[j+2] != 10) {
                        out_str << " " << (Names[y].scores_())[j] << " " << "/" << " " << (Names[y].scores_())[j+2] << " |" << endl;
                        frame_scores.push_back((Names[y].scores_())[j]+(Names[y].scores_())[j+1]+(Names[y].scores_())[j+2]);
                        print_frame_scores(out_str, longest, frame_scores);
                    }
                    else if ((Names[y].scores_())[j+2] == 10) {
                        out_str << " " << (Names[y].scores_())[j] << " " << "/" << " X |" << endl;
                        frame_scores.push_back((Names[y].scores_())[j]+(Names[y].scores_())[j+1]+10);
                        print_frame_scores(out_str, longest, frame_scores);
                    }
                    j += 2;
                }
                else if ((Names[y].scores_())[j] == 10 && (Names[y].scores_())[j]+(Names[y].scores_())[j+1]+(Names[y].scores_())[j+2] < 30) {
                    if ((Names[y].scores_())[j+1] == 10) {
                        out_str << " X X " << (Names[y].scores_())[j+2] << " |" << endl;
                        frame_scores.push_back(10+10+(Names[y].scores_())[j+2]);
                        print_frame_scores(out_str, longest, frame_scores);
                    } else {
                        out_str << " X " << (Names[y].scores_())[j+1] << " /" << " |" << endl;
                        frame_scores.push_back(10+(Names[y].scores_())[j+1]+(Names[y].scores_())[j+2]);
                        print_frame_scores(out_str, longest, frame_scores);
                    }
                    j += 2;
                }
                else if ((Names[y].scores_())[j] == 10 && (Names[y].scores_())[j]+(Names[y].scores_())[j+1]+(Names[y].scores_())[j+2] == 30) {
                    out_str << " X X X |" << endl;
                    frame_scores.push_back(10+10+10);
                    print_frame_scores(out_str, longest, frame_scores);
                    j += 2;
                }
            }
        }
        int total = getTotal(frame_scores);
        Names[y].total(total);
        frame_scores.clear();
    }
    //PRINTS THE NAMES AND SCORES IN ORDER
    out_str << top_down << '\n' << endl;
    sort(Names.begin(), Names.end(), sort_scores);
    for (unsigned int i=0; i < Names.size(); ++i) {
        string blanks (longest - ( (Names[i].getFirst()).size() + (Names[i].getLast()).size() + 1) , ' ');
        out_str << Names[i] << blanks << "  ";
        out_str.width(3); out_str << right << Names[i].pp() << endl;
    }
    //CALCULATE THE PERCENT OF TIMES A PERSON DOES NOT GET A STRIKE BUT IS ABLE TO KNOCK DOWN ALL THE PINS
    //SUBTRACTING THIS PERCENT FROM 100 WOULD GIVE YOU THE PERCENT OF TIMES THEY DIDN'T GET A STRIKE AND DIDN'T KNOCK DOWN ALL TEN PINS
    if (s.compare("custom") == 0) {
        out_str << endl;
        for (unsigned int d = 0; d < Names.size(); ++d) {
            out_str << Names[d] << string (longest - ((Names[d].getFirst()).size() + (Names[d].getLast()).size() + 1), ' ') + "  ";
            double total = 0.00;
            double complete = 0.00;
            for (unsigned int p = 0; p < (Names[d].scores_()).size(); ++p) {
                if ((Names[d].scores_())[p] < 10) {
                    if ((Names[d].scores_())[p] + (Names[d].scores_())[p+1] == 10) {
                        total += 1.00;
                        complete += 1.00;
                    } else {
                        total += 1.00;
                    }
                }
            }
            if (total == 0) {
                total = 1.00;
            }
            out_str.width(3); out_str << right << setprecision(2) << (complete/total) * 100 << "%" << endl;
        }
    }
    return 0;
}
