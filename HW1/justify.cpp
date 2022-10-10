#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctype.h>


using namespace std;

int main(int argc, char* argv[]) {
	// if the number of arguments is incorrect an error
	// message is printed and gracefully exited.

	if (argc != 5) {
	cerr << "ERROR: Incorrect number of arguments." << endl;
	exit(1);
	}

	// if input file isn't opened an error message is printed and gracefully exited.
	ifstream in_str(argv[1]);
	if (!in_str.good()) {
	cerr << "ERROR: Unable to open input file" << endl;
	exit(1);
	}

	// if output file isn't opened an error message is printed and gracefully exited.
	ofstream out_str(argv[2]);
	if (!out_str.good()) {
	cerr << "ERROR: Unable to open output file." << endl;
	exit(1);
	}

	// if argument 3 isn't a number an error message is printed and gracefully exited.
	const int width = stoi(argv[3]);

	//if argument 4 isn't one of the alignments an error message is printed and gracefully exited.
	const string s(argv[4]);
	if ((!s.compare("flush_left")) && (!s.compare("flush_right")) && (!s.compare("full_justify"))) {
	cerr << "ERROR: Invalid value." << endl;
	exit(1);
	}

	const string top_down(width+4, '-');
	out_str << top_down << endl;

	vector<string> words;
	string x;
	if (s.compare("flush_left") == 0) {
		out_str << "| ";
		while (in_str >> x) {
			words.push_back(x);
		}
		int length = 0; // length of each word
		int diff = width - length; // how much space is left to be filled
		for (unsigned int i = 0; i < words.size(); ++i) {
			if (words[i].size() < diff) {
				out_str << words[i] + ' ';
				length += words[i].size() + 1;
				diff = width - length;
			// ^if the word is less than the space left to be filled it is sent to the output file
			} else if (words[i].size() > diff) {
				if (words[i].size() < width) {
					const string spaces(diff, ' ');
					out_str <<  spaces + " |" << endl;
					out_str << "| " + words[i] + ' ';
					length = words[i].size() + 1;
					diff = width - length;
			// ^if the word is bigger than the space that was left but smaller than the width, a new line is made and the word is sent to the output file
				} else if (words[i].size() > width) {
					const string long_word(words[i], 0, diff);
					out_str << long_word + " |" << endl << "| ";
					const string long_word2(words[i], diff, -1);
					if  (long_word2.size()+1 < width) {
						out_str << "-" + long_word2 + ' ';
						length = long_word2.size();
						diff = width - length;
					}
			// ^ if the word is bigger than the space left and the width, the space is filled with a portion of the word and a new line is made and filled with
			// a hyphen and the rest of the word
					else if  (long_word2.size()+1 == width) {
						out_str << "-" + long_word2 + " |" << endl;
						length = long_word2.size();
						diff = width - length;
					}
				}
			// ^ if the word is bigger than the space left and the rest of the space is equal to the width, the space is filled with a portion of the word and a
			// new line is made and filled with a hyphen and the rest of the word
			} else if (words[i].size() == diff) {
				out_str << words[i] + " |" << endl << "| ";
				length = 0;
				diff = width - length;
			}

			if (i == words.size() - 1) {
				if (diff == 0) {
					out_str << " |" << endl;
				} else if (diff != 0) {
					const string spaces(diff, ' ');
					out_str << spaces << " |" << endl;
				}
			}
		}
		out_str << top_down << endl;
	}

	if (s.compare("flush_right") == 0) {
		out_str << "| ";
		cout << "| ";
		while (in_str >> x) {
			words.push_back(x);
		}
		int length = 0;
		int diff = width - length;
		unsigned int z = 0;
		for (unsigned int i = 0; i < words.size(); ++i) {
			if (words[i].size() < diff) {
				length += words[i].size() + 1;
				diff = width - length;
			// this keep track of the number of characters that are going to fill the line
			} else if (words[i].size() > diff) {
				if (words[i].size() < width) {
					const string spaces(diff + 1, ' ');
					out_str << spaces;
					cout << spaces;
					for (z; z < i; ++z) {
						out_str << words[z] + ' ';
						cout << words[z] + ' ';
						if (z == i-1) {
							out_str << "|" << endl << "| ";
							cout << "|" << endl << "| ";
							length = words[i].size() + 1;
							diff = width - length;
							z = i-1;
						}
					}
			//^ all the words that can fit in the line is know the number of spaces are sent to the output file along with the words
				} else if (words[i].size() > width) {
					for (unsigned int z; z < i; ++z) {
						out_str << words[z] + ' ';
						cout << words[z] + ' ';
					}
					const string long_word(words[i], 0, diff);
					out_str << long_word + "|" << endl << "| ";
					cout << long_word + "|" << endl << "| ";
					const string long_word2(words[i], diff, -1);
					length = long_word2.size();
					diff = width - length;
					string spaces(diff, ' ');
					out_str << spaces + long_word2 + "|" << endl << "| ";
					cout << spaces + long_word2 + "|" << endl << "| ";
					length = 0;
					diff = width - length;
				}
			//^  when a word is larger than the width the word is broken up to fit the previous line then the rest is put into a line by itself
			} else if (words[i].size() == diff) {
				for (z; z <= i; ++z) {
					out_str << words[z] + ' ';
					cout << words[z] + ' ';
					if (z == i) {
						out_str << "|" << endl << "| ";
						cout << "|" << endl << "| ";
						length = 0;
						diff = width - length;
						z = i;
					}
				}
			} if (i == (words.size() - 1)) {
				if (diff == 0) {
					for (z; z <= i; ++z) {
						out_str << words[z] + ' ';
						cout << words[z] + ' ';
						if (z == i) {
							out_str << "|" << endl;
							cout << "|" << endl;
						}
					}
				} else if (diff != 0) {
					const string spaces(diff + 1, ' ');
					out_str << spaces;
					cout << spaces;
					for (z; z <= i; ++z) {
						out_str << words[z] + ' ';
						cout << words[z] + ' ';
						if (z == i) {
							out_str << "|" << endl;
							cout << "|" << endl;
						}
					}
				}
			}
		}
		out_str << top_down << endl;
		cout << top_down << endl;
	}

	if (s.compare("full_justify") == 0) {
		out_str << "| ";
		cout << "| ";
		while (in_str >> x) {
			words.push_back(x);
		}
		int length = 0;
		int diff = width - length;
		unsigned int y = 0;
		for (unsigned int i = 0; i < words.size(); ++i) {
			if (words[i].size() < diff) {
				length += words[i].size() + 1;
				diff = width - length;
			// this keep track of the number of characters that are going to fill the line
			} else if (words[i].size() > diff) {
				if (words[i].size() < width) {
					length -= 1;
					int numb_words = ((i - 1) - y);
					int blank = ((width - length) / numb_words);
					const string blanks(blank, ' ');
					if ((blank + length) == width) {
						for (y; y < (i - 1); ++y) {
							out_str << words[y] + " " + blanks;
							cout << words[y] + " " + blanks;
							if (y == (i - 1)) {
								out_str << words[y] + " |" << endl << "| ";
								cout << words[y] + " |" << endl << "| ";
								length = 0;
								diff = width - length;
								y = i - 1;
							}
						}
					} else if ((blank + length) < width) {
						int extra_spaces = width - (blank + length);
						int c = y + extra_spaces;
						for (y; y < c; ++y) {
							out_str << words[y] + "  "  + blanks;
							cout << words[y] + "  "  + blanks;
						}
						for (c; c < (i - 1); ++c) {
							out_str << words[c] + " " + blanks;
							cout << words[c] + " " + blanks;
							if (c == (i - 1)) {
								out_str << words[c] + " |" << endl << "| ";
								cout << words[c] + " |" << endl << "| ";
								length = 0;
								diff = width - length;
								y = i - 1;
							}
						}
					}
				}
			} else if (words[i].size() == diff) {
				for (y; y < (i - 1); ++y) {
					out_str << words[y] + " ";
					cout << words[y] + " ";
					if (y == (i - 1)) {
						out_str << words[y] + " " + " |" << endl << "| ";
					}
				}
			}
			if (i == words.size() - 1) {
				length -= 1;
				int numb_words = (i - y);
				int blank = ((width - length) / numb_words);
				const string blanks(blank, ' ');
				if ((blank + length) == width) {
					for (y; y < i; ++y) {
						out_str << words[y] + " " + blanks;
						cout << words[y] + " " + blanks;
						if (y == i) {
							out_str << words[y] + " |" << endl;
						}
					}
				} else if ((blank + length) < width) {
					int extra_spaces = width - (blank + length);
					int c = y + extra_spaces;
					for (y; y < c; ++y) {
						out_str << words[y] + "  "  + blanks;
						cout << words[y] + "  "  + blanks;
					}for (c; c < i; ++c) {
						out_str << words[c] + " " + blanks;
						cout << words[c] + " " + blanks;
						if (c == i) {
							out_str << words[c] + " |" << endl;
							cout << words[c] + " |" << endl;
							length = 0;
							diff = width - length;
						}
					}
				}
			}
		}
	return (0);
	}
}
