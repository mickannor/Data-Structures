#include "hash_table.h"
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <utility>
#include <cmath>

class hash_string_obj {
public:
    int operator() (const std::string& key, const int& M) const {
        // THIS IMPLEMENTATION COMES FROM
        // https://opendsa-server.cs.vt.edu/ODSA/Books/CS3/html/HashFuncExamp.html
        long sum = 0, mul = 1;
        for (int i = 0; i < key.length(); i++) {
            mul = (i % 4 == 0) ? 1 : mul * 256;
            sum += key[i] * mul;
        }
        return (std::abs(sum) % M);
    }
};

int match(const std::string& rest, const int& k, const std::string& query_string, const int& remainder) {
    int mismatches = 0;
    for (unsigned int x = 0; x < remainder; ++x) {
        if (rest[x] != query_string[k+x]) {
            mismatches += 1;
        }
    }
    return mismatches;
}

int main() {
    std::string command;
    float f = 0.5;
    int N = 100;
    std::string file;
    int k;
    ds_hashset< std::string, hash_string_obj> table;
    typedef ds_hashset< std::string, hash_string_obj>::iterator table_itr;
    while (std::cin >> command) {
        if (command == "genome") {
            std::cin >> file;
        } else if (command == "table_size") {
            std::cin >> N;
        } else if (command == "occupancy") {
            std::cin >> f;
        } else if (command == "kmer") {
            if (N != 100) {
                ds_hashset<std::string, hash_string_obj> table(N);
            }
            std::cin >> k;
            // IF N AND f AREN'T INITIALIZED SET THEM TO THE DEFAULT VALUES SO THEY CAN BE USED IN THE INSERT FUNCTION
            std::string seq;
            std::ifstream filename(file);
            unsigned int y = 0;
            while (filename >> seq) {
                // PUT THE KMERS INTO THE HASH TABLE
                for (unsigned int i = 0; i < seq.size(); ++i) {
                    std::string portion = seq.substr(i, k);
                    if (portion.size() == k) {
                        table.insert(portion, y, f, N);
                    }
                    y += 1;
                }
            }
        } else if (command == "query") {
            int m;
            std::string query_string;
            std::cin >> m >> query_string;
            std::cout << "Query: " << query_string << std::endl;
            if (query_string.size() > k) {
                //IF THE QUERY_STRING IS LONGER THAN THE k MAKE A SUBSTR AND USE FIND ON THAT SUBSTR
                std::string seed;
                seed = query_string.substr(0, k);
                //IF THE FIND FUNCTION CANNOT FIND THE SEED IN ISN'T IN THE HASH TABLE
                table_itr info = table.find(seed, N);
                if (info == table.end()) {
                    std::cout << "No Match" << std::endl;
                //IF THE SEED IS FOUND
                } else {
                    std::string rest = "";
                    //FIND OUT HOW MUCH OF THE QUERY_STRING STILL NEEDS TO BE FOUND
                    for (unsigned int i = k; i < query_string.size(); i += 0) {
                        std::string next = query_string.substr(i, k);
                        if (query_string.size() - i >= k) {
                            rest += table.find_by_loc(next, N, info.list_itr->second+i);
                            i += k;
                        } else {
                            i = i - (k-next.size());
                            rest.erase(rest.size()-(k-next.size()));
                            next = query_string.substr(i - (k-next.size()), k);
                            rest += table.find_by_loc(next, N, info.list_itr->second+i);
                            i += k;
                        }
                    }
                    int diff = match(rest, k, query_string, query_string.size()-k);
                    if (diff > m) {
                        std::cout << "No Match" << std::endl;
                    } else {
                        std::cout << info.list_itr->second << " " << diff << " " << seed << rest << std::endl;
                    }
                }
                //USE FIND ON THE QUERY STRING
            } else if (query_string.size() == k) {
                table_itr info = table.find(query_string, N);
                if (info == table.end()) {
                    std::cout << "No Match" << std::endl;
                } else {
                    std::cout << info.list_itr->second << "0" << query_string << std::endl;
                }
                //IF THE QUERY_STRING IS LESS THAN k IT CANNOT FORM A SEED SO THERE WILL BE NO MATCH
            } else if (query_string.size() < k) {
                std::cout << "No Match" << std::endl;
            }
        } else if (command == "quit") {
            exit(1);
        }
    }
}
