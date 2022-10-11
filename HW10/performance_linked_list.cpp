#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <string>
#include <list>


// defined in performance.cpp
void usage();

template <class T>
void list_sort(std::list<T>& lst, std::ostream& ostr, int& output_count) {
    sort(lst.begin(), lst.end());
    output_count = lst.size();
    for (std::list<T>::iterator itr = lst.begin(); itr != lst.end(); ++itr) {
        ostr << *itr << std::endl;
    }
}

template <class T>
void list_remove_duplicates(const std::list<T>& lst, std::ostream &ostr, int& output_count) {
}

template <class T>
void list_mode(std::list<T>& lst, std::ostream, int& output_count) {
    sort(lst.begin(), lst.end());
    int current_count = 1;
    int mode_count = 0;
    std::list<T>::iterator mode;
    for (std::list<T>::iterator itr = lst.begin(); itr != lst.end(); ++itr) {
        if (itr++ == itr) {
            current_count += 1;
        } else {
            if (current_count > mode count) {
                mode_count = current_count;
                mode = itr;
            }
        }
    }
    output_count = 1;
    ostr << *mode << std::endl;
}

void list_closest_pair(std::list<int> &lst, std::ostream &ostr, int &output_count) {
    if (lst.size >= 2) {
        sort(lst.begin(), lst.end());
        output_count = 2;
        int best;
        std::list<int>::iterator index;
        for (std::list<int>::iterator itr = lst.begin(); itr != lst.end(); ++itr) {
            int diff = *itr++ - *itr;
            if (index == NULL || diff < best) {
                best = diff;
                index = itr;
            }
        }
        ostr << *index << std::endl;
        ostr << *index++ << std::endl;
    }
}

template <class T>
void list_first_sorted(std::list<T> &lst, std::ostream &ostr, int &output_count, int optional_arg) {
    if (optional_arg >= 1 && vec.size() >= optional_arg) {
        sort(lst.begin(), lst.end());
        output_count = optional_arg;
        for (int i = 0; i < output_count; i++) {
            std::list<T>::iterator itr = lst.begin();
            ostr << *itr << std::endl;
            itr++;
        }
    }
}

void list_longest_substring(std::list<std::string>& lst, std::ostream &ostr, int &output_count) {
    if (lst.size() > 1) {
        
    }
}

void list_test(const std::string &operation, const std::string &type,
                 std::istream &istr, std::ostream &ostr,
                 int &input_count, int &output_count, int optional_arg) {

  // HINT: For the string element type, declare your list like this:
  // std::list<std::string> lst;

  std::cerr << "Error: DATA STRUCTURE NOT IMPLEMENTED" << std::endl;
  exit(0);
}
