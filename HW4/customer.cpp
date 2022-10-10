#include "customer.h"
#include <list>
#include <string>

Customer::Customer(const std::string& person) {
    name_ = person;
    received_ = 0;
}
bool Customer::has_max_num_movies() {
    if (received_ < 3) {
        return false;
    }
    return true;
}
bool Customer::preference_list_empty() {
    if (preference_.size() > 0) {
        return false;
    }
    return true;
}
void Customer::add_movies(const std::string& dvd) {
    preference_.push_back(dvd);
}
void Customer::receives(const std::string& dvd) {
    has_.push_back(dvd);
    received_ += 1;
    for (list_type::iterator itr = preference_.begin(); itr != preference_.end(); itr++) {
    if (*itr == dvd) {
        itr = preference_.erase(itr);
    }
  }
}
bool Customer::find_movie(const std::string& dvd) {
    for (list_type::iterator itr = preference_.begin(); itr != preference_.end(); itr++) {
        if (*itr == dvd) {
            return true;
        }
    }
    return false;
}
void Customer::return_newest() {
    has_.pop_back();
    received_ -= 1;
}
void Customer::return_oldest() {
    has_.pop_front();
    received_ -= 1;
}
