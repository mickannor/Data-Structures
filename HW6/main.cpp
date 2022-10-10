#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdlib.h>

void recurse_search_across(const std::vector<std::string>& grid, const std::vector<std::string>& dict, int words_found, std::vector<std::string>& solution, unsigned int& x, int c) {
    if (grid[x].size() < 8) {
        if (c <= grid[x].size()-3) {
            if (std::find(dict.begin(), dict.end(), grid[x].substr(c,grid[x].size()-1)) != dict.end()) {
                words_found += 1;
                for (unsigned int v = 0; v < c; ++v) {
                    solution[x][v] = '#';
                }
            } else if (std::find(dict.begin(), dict.end(), grid[x].substr(0, grid[x].size()-c)) != dict.end()) {
                words_found += 1;
                for (unsigned int v =  (grid[x].substr(0, grid[x].size()-c)).size(); v < grid[x].size(); ++v) {
                    solution[x][v] = '#';
                }
            } else {
                recurse_search_across(grid, dict, words_found, solution, x, c+1);
            }
        }
    } else {
        if (std::find(dict.begin(), dict.end(), grid[x].substr(0,2+c)) != dict.end()) {
            words_found += 1;
            solution[x][2+c+1] = '#';
            x += 2;
        } else if (std::find(dict.begin(), dict.end(), grid[x].substr(c)) != dict.end()) {
            solution[x][c-1] = '#';
            x += 2;
        }
         else {
            recurse_search_across(grid, dict, words_found, solution, x, c+1);
        }
    }
}
void recurse_search_down(const std::string& down, const std::vector<std::string>& dict, int words_found, std::vector<std::string>& solution, const int column, int c){
    if (c <= down.size()-3) {
        if (std::find(dict.begin(), dict.end(), down.substr(c)) != dict.end()) {
            words_found += 1;
            for (unsigned int v = c-1; v >= 0; --v) {
                solution[column][v] = '#';
            }
        }  else if (std::find(dict.begin(), dict.end(), down.substr(0, down.size()-c)) != dict.end()) {
            words_found += 1;
            for (unsigned int v = down.size()-c; v < down.size(); ++v) {
                solution[column][v] = '#';
            }
        } else {
            recurse_search_down(down, dict, words_found, solution, column, c+1);
        }
    }
}

std::ostream& print_solution (std::ostream& out_mode, int& total, std::vector<std::string>& solution, const std::vector<std::string>& grid, const int& num_words) {
    if (total > 0) {
        //IF THERE IS MORE THAN ONE SOLUTION
        if (num_words < total) {
            out_mode << "Number of Solution(s): " << (total - num_words) << std::endl;
            out_mode << "Board:" << std::endl;
            for (unsigned int t = 0; t < solution.size(); ++t) {
                out_mode << solution[t] << std::endl;
            }
            solution = grid;

        } else if (num_words == total) {
            out_mode << "Number of Solution(s): 1" << std::endl;
            out_mode << "Board:" << std::endl;
            for (unsigned int t = 0; t < solution.size(); ++t) {
                out_mode << solution[t] << std::endl;
            }
        }
    } else if (total == 0) {
        out_mode << "Number of Solution(s): 0" << std::endl;
    }
    return out_mode;
}

void search(const std::vector<std::string>& grid, std::vector<std::string>& dict, int& words_found, int& total, std::vector<std::string>& solution, int& column, const std::vector<int>& word_len, const int& num_words, std::ostream& out_mode) {
    // CREATE A STRING THAT HOLDS THE LETTER IN THE COLUMN
    std::string down;
    down = grid[0][column];
    // ADD THEM DEPENDING ON THE SIZE OF THE ROW AND THE LARGEST WORD TO BE FOUND
    if (word_len.back() <= grid.size()) {
        for (int i = 1; i < word_len.back(); ++i) {
            down += grid[i][column];
        }
    }

    if (down.size() == grid.size()) {
        while (words_found < num_words) {
            // IF THE WHOLE FIRST COLUMN IS A WORD
            if (std::find(dict.begin(), dict.end(), down) != dict.end() && column != grid[0].size()-1) {
                words_found += 1;
                if (grid[0].size() == 3) {
                    for (unsigned int x = 0; x < down.size(); ++x) {
                        std::vector<std::string>::const_iterator word_iter = std::find(dict.begin(), dict.end(), grid[x]);
                        if (word_iter == dict.end()) {
                            solution[x][column+1] = '#';
                        } else {
                            words_found += 1;
                            dict.erase(word_iter);
                            for (unsigned int r = 1; x+r < down.size(); ++r) {
                                solution[x+r][column+1] = '#';
                            }
                            column += 2;
                            search(grid, dict, words_found, total, solution, column, word_len, num_words, out_mode);
                        }
                    }
                } else if (grid[0].size() > 3) {
                    for (unsigned int x = 0; x < down.size(); ++x) {
                        std::vector<std::string>::const_iterator word_iter = std::find(dict.begin(), dict.end(), grid[x]);
                        if (word_iter == dict.end()) {
                            int c = 1;
                            recurse_search_across(grid, dict, words_found, solution, x, c);
                        }
                    }
                }
            } else if (std::find(dict.begin(), dict.end(), down) != dict.end() && column == grid[0].size()-1) {
                words_found += 1;
            //IF THE WHOLE FIRST COLUMN ISN'T A WORD
            } else if (std::find(dict.begin(), dict.end(), down) == dict.end()) {
                int c = 1;
                recurse_search_down(down, dict, words_found, solution, column, c);
                for (unsigned int x = 0; x < grid[x].size(); ++x) {
                    c = 1;
                    if (std::find(dict.begin(), dict.end(), grid[x]) == dict.end()) {
                        recurse_search_across(grid, dict, words_found, solution, x, c);
                    }
                }
            }
            words_found = num_words;
        }
        //WHEN ALL THE WORDS FOR THAT SOLUTION ARE FOUND PRINT THE SOLUTION
        if (words_found == num_words) {
            if (total > num_words) {
                print_solution(out_mode, total, solution, grid, num_words);
                int stop = total;
                total = num_words;
                int times = 1;
                if (times <= stop) {
                    words_found = 0;
                    column = 0;
                    search(grid, dict, words_found, total, solution, column, word_len, num_words, out_mode);
                    out_mode << "Board:" << std::endl;
                    for (unsigned int i = 0; i < solution.size(); ++i) {
                        out_mode << solution[i] << std::endl;
                    }
                    times += 1;
                } else {}
            } else {
                print_solution(out_mode, total, solution, grid, num_words);
            }
        }
    //IF THE COLUMN BIGGER THAN THE LARGEST WORD
    } else if (down.size() < grid.size()) {
        while (words_found < num_words) {
            if (std::find(dict.begin(), dict.end(), down) != dict.end()) {
                words_found += 1;
                solution[0][down.size()] = '#';
                for (unsigned int x = 0; x < down.size(); ++x) {
                    int c = 1;
                    if (std::find(dict.begin(), dict.end(), grid[x]) == dict.end()) {
                        recurse_search_across(grid, dict, words_found, solution, x, c);
                    }
                }
            }
        }
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 5) {
        std::cerr << "ERROR: Incorrect number of arguments." << std::endl;
    }
    std::ifstream dict_file(argv[1]);
    if (!dict_file.good()) {
	    std::cerr << "ERROR: Unable to open dictionary file." << std::endl;
	}
    std::ifstream grid_file(argv[2]);
	if (!grid_file.good()) {
	    std::cerr << "ERROR: Unable to open initial grid file." << std::endl;
	}
    std::ofstream out_mode(argv[4]);
    if (!out_mode.good()) {
        std::cerr << "ERROR: Output mode unavailable." << std::endl;
    }

    std::vector<std::string> dict;
    std::vector<std::string> grid;
    std::vector<std::string> found;
    std::vector<std::string> solution;
    std::vector<int> word_len;
    int words_found = 0;
    int num_words = 0;

    std::string word;
    while (dict_file >> word) {
        dict.push_back(word);
    }
    int total = dict.size();
    //IN CASE THE DICTIONARY FILE IS EMPTY
    if (total == 0) {
        print_solution(out_mode, total, solution, grid, num_words);
        exit(1);
    }
    std::sort(dict.begin(), dict.end());
    while (grid_file >> word) {
        if (word[0] == '!') {}
        else if (word[0] == '+') {
            num_words += 1;
            word_len.push_back(stoi(word.substr(1)));
        } else {
            grid.push_back(word);
        }
    }
    solution = grid;
    std::sort(word_len.begin(), word_len.end());
    int column = 0;
    search(grid, dict, words_found, total, solution, column, word_len, num_words, out_mode);
}
