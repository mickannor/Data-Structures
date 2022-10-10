#ifndef __dvd_h_
#define __dvd_h_
#include <string>

class DvD {
public:
    DvD(const std::string& dvd, const int& x);
    //ACCESSORS
    std::string get_name(){return name_;} const
    int get_rented(){return rented_;} const
    int get_remain(){return remain_;} const
    //ADJUST THE AMOUNT OF COPIES THAT CAN BE SENT OUT
    void more(const int& c) {remain_ += c;}
    void shipped();
    void returned();
    bool available();
private:
    std::string name_;
    int remain_;
    int rented_;
};
#endif
