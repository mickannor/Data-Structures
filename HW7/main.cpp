#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

typedef std::map<const std::string, std::pair<int, int>> COSTUME_SHOP_TYPE;
typedef std::map<const std::string, std::string> PEOPLE_TYPE;
typedef std::map<const std::string, std::vector<std::string>> ORDER_TYPE;
// prototypes for the helper functions
void addCostume(COSTUME_SHOP_TYPE &costume_shop);
void rentCostume(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people, std::vector<std::string>& customers, ORDER_TYPE& order);
void lookup(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people, const ORDER_TYPE& order);
void printPeople(PEOPLE_TYPE& people, std::vector<std::string>& customers);

int main() {
  // two data structures store all of the information for efficiency
  COSTUME_SHOP_TYPE costume_shop;
  PEOPLE_TYPE people;
  std::vector<std::string> customers;
  ORDER_TYPE order;
  char c;
  while (std::cin >> c) {
    if (c == 'a') {
      addCostume(costume_shop);
    } else if (c == 'r') {
      rentCostume(costume_shop,people, customers, order);
    } else if (c == 'l') {
      lookup(costume_shop, people, order);
    } else if (c == 'p') {
      printPeople(people, customers);
    } else {
      std::cerr << "error unknown char " << c << std::endl;
      exit(0);
    }
  }
}


void addCostume(COSTUME_SHOP_TYPE& costume_shop) {
  std::string costume_name;
  int num;
  std::cin >> costume_name >> num;
  COSTUME_SHOP_TYPE::iterator costume_itr = costume_shop.find(costume_name);
  if (costume_itr != costume_shop.end()) {
    (costume_itr->second).first += num;
  } else {
    costume_shop[costume_name] = std::make_pair(num, 0);
  }
  if (num == 1) {
    std::cout << "Added " << num << " " << costume_name << " costume." << std::endl;
  } else if (num > 1) {
    std::cout << "Added " << num << " " << costume_name << " costumes." << std::endl;
  }
}


void rentCostume(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people, std::vector<std::string>& customers, ORDER_TYPE& order) {
    std::string first_name;
    std::string last_name;
    std::string costume_name;
    std::cin >> first_name >> last_name >> costume_name;
    std::string name = first_name + " " + last_name;
    COSTUME_SHOP_TYPE::iterator shop_itr = costume_shop.find(costume_name);
    if (std::find(customers.begin(), customers.end(), name) == customers.end()) {
        customers.push_back(name);
    }
    //CHECK IF THE COSTUME IS CARRIED BY THE STORE
    if (shop_itr != costume_shop.end()) {
        //IF IT IS MAKE SURE ONE IS AVAILABLE FOR RENT
        if ((shop_itr->second).first - (shop_itr->second).second >= 1) {
            (shop_itr->second).second += 1;
            order[costume_name].push_back(name);
            //CHECK IF THE CUSTOMER IS ALREADY RENTING ANOTHER COSTUME
            PEOPLE_TYPE::iterator people_itr = people.find(name);
            if (people_itr == people.end()) {
                people[name] = costume_name;
                std::cout << name << " rents a " << costume_name << " costume." << std::endl;
            } else {
                //IF THEY ARE RETURN IT BEFORE RENTING THE OTHER
                costume_shop[people_itr->second].second -= 1;
                if (people_itr->second != costume_name) {
                    order[people_itr->second].erase(std::find(order[people_itr->second].begin(), order[people_itr->second].end(), name));
                    std::cout << name << " returns a " << people_itr->second << " costume before renting a " << costume_name << " costume." << std::endl;
                    people[name] = costume_name;
                } else {
                    std::cout << name << " already has a " << costume_name << " costume." << std::endl;
                }
            }
        } else {
            std::cout << "No " << costume_name << " costumes available." << std::endl;
        }
    } else {
        std::cout << "The shop doesn't carry " << costume_name << " costumes." << std::endl;
    }
}

void who_rentin(const ORDER_TYPE& order, const std::string& costume_name) {
    ORDER_TYPE::const_iterator renters = order.find(costume_name);
    if (renters != order.end()) {
        for (std::vector<std::string>::const_iterator order_itr = (renters->second).begin(); order_itr != (renters->second).end(); ++order_itr) {
            std::cout << "    " << *order_itr << std::endl;
        }
    }
}


void lookup(COSTUME_SHOP_TYPE& costume_shop, PEOPLE_TYPE& people, const ORDER_TYPE& order) {
    std::string costume_name;
    std::cin >> costume_name;
    COSTUME_SHOP_TYPE::iterator costume_itr = costume_shop.find(costume_name);
    if (costume_itr != costume_shop.end()) {
        std::cout << "Shop info for " << costume_name << " costumes:" << std::endl;
        if ((costume_itr->second).first - (costume_itr->second).second == 1) {
            std::cout << "  1 copy available" << std::endl;
            if ((costume_itr->second).second == 1) {
                std::cout << "  1 copy rented by:" << std::endl;
                who_rentin(order, costume_name);
            } else if ((costume_itr->second).second > 1) {
                std::cout << "  " << (costume_itr->second).second << " copies rented by:" << std::endl;
                who_rentin(order, costume_name);
            }
        } else if ((costume_itr->second).first - (costume_itr->second).second > 1){
            std::cout << "  " << (costume_itr->second).first - (costume_itr->second).second << " copies available" << std::endl;
            if ((costume_itr->second).second == 1) {
                std::cout << "  1 copy rented by:" << std::endl;
                who_rentin(order, costume_name);
            } else if ((costume_itr->second).second > 1) {
                std::cout << "  " << (costume_itr->second).second << " copies rented by:" << std::endl;
                who_rentin(order, costume_name);
            }
        } else if ((costume_itr->second).first - (costume_itr->second).second == 0) {
            if ((costume_itr->second).second == 1) {
                std::cout << "  1 copy rented by:" << std::endl;
                who_rentin(order, costume_name);
            } else if ((costume_itr->second).second > 1) {
                std::cout << "  " << (costume_itr->second).second << " copies rented by:" << std::endl;
                who_rentin(order, costume_name);
            }
        }
    } else {
        std::cout << "The shop doesn't carry " << costume_name << " costumes." << std::endl;
    }
}


bool lesser_than(const std::string name1, const std::string name2) {
    std::size_t space1 = name1.find(" ");
    std::size_t space2 = name2.find(" ");
    if (name1.substr(space1+1) < name2.substr(space2+1)) {
        return true;
    } else if (name1.substr(space1+1) == name2.substr(space2+1)) {
        if (name1.substr(0,space1) < name2.substr(0,space2)) {
            return true;
        }
    }
    return false;
}


void printPeople(PEOPLE_TYPE& people, std::vector<std::string>& customers) {
    std::sort(customers.begin(), customers.end(), lesser_than);
    if (customers.size() == 1) {
        std::cout << "Costume info for the 1 party attendee:" << std::endl;
    } else {
        std::cout << "Costume info for the " << customers.size() << " party attendees:" << std::endl;
    }
    for (std::vector<std::string>::iterator customers_itr = customers.begin(); customers_itr != customers.end(); ++customers_itr) {
        if (people.find(*customers_itr) != people.end()) {
            std::cout << "  " << *customers_itr << " is wearing a " << people[*customers_itr] << " costume." << std::endl;;
        } else {
            std::cout << "  " << *customers_itr << " does not have a costume." << std::endl;
        }
    }
}
