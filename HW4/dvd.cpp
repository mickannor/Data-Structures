#include "dvd.h"
#include <string>

DvD::DvD(const std::string& dvd, const int& x) {
    name_ = dvd;
    remain_ = x;
}
void DvD::shipped() {
    remain_ -= 1;
    rented_ += 1;
}
bool DvD::available() {
    if (remain_ > 0) {
        return true;
    }
    return false;
}
void DvD::returned() {
    remain_ += 1;
    rented_ -= 1;
}
