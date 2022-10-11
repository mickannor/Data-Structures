#include <cassert>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <string>
#include <queue>


// defined in performance.cpp
void usage();


void priority_queue_test(const std::string &operation, const std::string &type,
                         std::istream &istr, std::ostream &ostr,
                         int &input_count, int &output_count, int optional_arg) {

  // HINT: For the string element type, declare your priority_queue like this:

  // std::priority_queue<std::string,std::vector<std::string> > pq;
  // (this will make a "max" priority queue, with big values at the top)

  // OR
  
  // std::priority_queue<std::string,std::vector<std::string>, std::greater<std::string> > pq;
  // (this will make a "min" priority queue, with big values at the bottom)

  std::cerr << "Error: DATA STRUCTURE NOT IMPLEMENTED" << std::endl;
  exit(0);
}
