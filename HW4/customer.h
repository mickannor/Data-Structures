#ifndef __customer_h_
#define __customer_h_
#include <list>
#include <string>

class Customer {
    typedef std::list<std::string> list_type;
public:
    Customer(const std::string& person);
    bool has_max_num_movies();
    bool preference_list_empty();
    list_type get_preferences() {return preference_;}
    list_type get_has() {return has_;}
    int get_received() {return received_;}
    std::string get_name() {return name_;}
    void add_movies(const std::string& dvd);
    void receives(const std::string& dvd);
    void return_newest();
    void return_oldest();
    bool find_movie(const std::string& dvd);
private:
    std::list<std::string> preference_;
    std::list<std::string> has_;
    std::string name_;
    int received_;
};
#endif
