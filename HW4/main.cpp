// ==================================================================
// Important Note: You are encouraged to read through this provided
//   code carefully and follow this structure.  You may modify the
//   file as needed to complete your implementation.
// ==================================================================

#include <fstream>
#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include <algorithm>
#include "dvd.h"
#include "customer.h"

// several type defs to keep things neat
// (hint, these are STL lists)
typedef std::list<DvD> inventory_type;
typedef std::list<Customer> customers_type;
typedef std::list<std::string> preference_type;

// ==================================================================

// Helper function prototypes
std::string read_customer_name(std::istream &istr);
std::string read_dvd_name(std::istream &istr);
inventory_type::iterator find_DVD(inventory_type &inventory, const std::string &dvd_name);

// The main algorithm for DVD distribution
void shipping_algorithm(inventory_type &inventory, customers_type &customers, std::ostream &ostr);

// ==================================================================
// A helper function to find a DVD from the inventory
inventory_type::iterator find_DVD(inventory_type &inventory, const std::string &dvd_name) {
  for (inventory_type::iterator inventory_itr = inventory.begin();
       inventory_itr != inventory.end(); inventory_itr++) {
    if (inventory_itr->get_name() == dvd_name) {
      return inventory_itr;
    }
  }
  // if the DVD is not found, return the .end() iterator
  return inventory.end();
}
customers_type::iterator find_Customer(customers_type &customers, const std::string &customer_name) {
    for (customers_type::iterator customer_itr = customers.begin(); customer_itr != customers.end(); ++customer_itr) {
        if (customer_itr->get_name() == customer_name) {
            return customer_itr;
        }
    }
    return customers.end();
}

void usage(const char* program_name) {
  std::cerr << "Usage: " << program_name << " <input_file> <output_file>" << std::endl;
  std::cerr << " -or-  " << program_name << " <input_file> <output_file> --analysis" << std::endl;
  std::cerr << " -or-  " << program_name << " <input_file> <output_file> --improved" << std::endl;
  std::cerr << " -or-  " << program_name << " <input_file> <output_file> --improved --analysis" << std::endl;
  exit(1);
}


int main(int argc, char* argv[]) {

  if (argc < 3 || argc > 5) {
    usage(argv[0]);
  }

  // open input and output file streams
  std::ifstream istr(argv[1]);
  if (!istr) {
    std::cerr << "ERROR: Could not open " << argv[1] << " for reading." << std::endl;
    usage(argv[0]);
  }
  std::ofstream ostr(argv[2]);
  if (!ostr) {
    std::cerr << "ERROR: Could not open " << argv[2] << " for writing." << std::endl;
    usage(argv[0]);
  }

  // optional parameters for extra credit
  bool use_improved_algorithm = false;
  bool print_analysis = false;
  for (int i = 3; i < argc; i++) {
    if (std::string(argv[i]) == "--improved") {
      use_improved_algorithm = true;
    } else if (std::string(argv[i]) == "--analysis") {
      print_analysis = true;
    } else {
      usage(argv[0]);
    }
  }


  // stores information about the DVDs
  inventory_type inventory;
  // stores information about the customers
  customers_type customers;


  // read in and handle each of the 8 keyword tokens
  std::string token;
  while (istr >> token) {
    if (token == "dvd") {
        std::string dvd_name = read_dvd_name(istr);
        int copies;
        istr >> copies;
//SEARCH FOR IT IN INVENTORY AND ADD THE MOVIE AND COPIES DEPENDING ON IF IT'S IN THE LIST ALREADY OR NOT
        inventory_type::iterator spot = find_DVD(inventory, dvd_name);
        if (spot == inventory.end()) {
            DvD film(dvd_name, copies);
            inventory.push_back(film);
            if (copies > 1) {
                ostr << copies << " copies of " << dvd_name << " added" << std::endl;
            } else {
                ostr << copies << " copy of " << dvd_name << " added" << std::endl;
            }
        } else {
            spot->more(copies);
            if (copies > 1) {
                ostr << copies << " copies of " << dvd_name << " added" << std::endl;
            } else {
                ostr << copies << " copy of " << dvd_name << " added" << std::endl;
            }
        }

    } else if (token == "customer") {
        std::string customer_name = read_customer_name(istr);
        int num_movies;
        istr >> num_movies;

//CHECKS IT THE CUSTOMER IS ALREADY IN THE LIST
        customers_type::iterator spot = find_Customer(customers,customer_name);
        Customer person(customer_name);
        if (spot != customers.end()) {
            ostr << "WARNING: Already have a customer named " << customer_name << std::endl;
        }
//READS THE NAME AND ADDS UPDATES THEIR PREFERENCE LIST IF DEPENDING ON IF THE MOVIE IS IN THE INVENTORY
        for (int i = 0; i < num_movies; i++) {
            std::string dvd_name = read_dvd_name(istr);
            inventory_type::iterator spot = find_DVD(inventory, dvd_name);
            if (spot!=inventory.end()) {
                bool in_it = person.find_movie(dvd_name);
                if (in_it == false) {
                    person.add_movies(dvd_name);
                } else {
                    ostr << "WARNING: Duplicate movie " << dvd_name << " on preference list!" << std::endl;
                }
            } else {
                ostr << "WARNING: No movie named " << dvd_name << " in the inventory" << std::endl;
            }
        }
        if (spot == customers.end()) {
            customers.push_back(person);
            ostr << "new customer: " << customer_name << std::endl;
        }


    } else if (token == "ship") {

      shipping_algorithm(inventory,customers,ostr);

    } else if (token == "return_oldest") {
        //SEARCHES FOR THE USES NAME IN THE CUSTOMERS LIST AND RETURNS THE FIRST MOVIE IN THE LIST OF MOVIES THEY HAVE
        std::string customer_name = read_customer_name(istr);
        customers_type::iterator spot = find_Customer(customers, customer_name);
        if (spot != customers.end()) {
            if (spot->get_has().size() != 0) {
                ostr << customer_name << " returns " << (spot->get_has()).front() << std::endl;
                inventory_type::iterator position = find_DVD(inventory, (spot->get_has()).front());
                position->returned();
                spot->return_oldest();
            } else {
                ostr << "WARNING: " << customer_name << " has no DVDs to return!" << std::endl;
            }
        } else {
            ostr << "WARNING: No customer named " << customer_name << std::endl;
        }

    } else if (token == "return_newest") {
        std::string customer_name = read_customer_name(istr);
//SEARCHES FOR THE USES NAME IN THE CUSTOMERS LIST AND RETURNS THE LAST MOVIE IN THE LIST OF MOVIES THEY HAVE
        customers_type::iterator spot = find_Customer(customers, customer_name);
        if (spot != customers.end()) {
            if (spot->get_received() > 0) {
              ostr << customer_name << " returns " << (spot->get_has()).back() << std::endl;
              inventory_type::iterator position = find_DVD(inventory, (spot->get_has()).back());
              position->returned();
              spot->return_newest();
            } else {
              ostr << "WARNING: " << customer_name << " has no DVDs to return!" << std::endl;
          }
        } else {
          ostr << "WARNING: No customer named " << customer_name << std::endl;
        }

    } else if (token == "print_customer") {
        //CHECKS FOR THE USER IN THE CUSTOMER LIST AND PRINTS THE # OF MOVIES THEY HAVE
        std::string customer_name = read_customer_name(istr);
        customers_type::iterator spot = find_Customer(customers, customer_name);
        if (spot != customers.end()) {
            preference_type has = spot->get_has();
            preference_type::iterator movie = has.begin();
            if (spot->get_received() == 0) {
                ostr << customer_name << " has no movies" << std::endl;
            } else {
                if (spot->get_received()>1) {
                    ostr << customer_name << " has " << spot->get_received() << " movies:" << std::endl;
                } else {
                    ostr << customer_name << " has " << spot->get_received() << " movie:" << std::endl;
                }

            }
            int i = 0;
            while (i < spot->get_received()) {
                ostr << "    " << *movie << std::endl;
                ++movie;
                ++i;
            }
            //PRINTS THE MOVIES IN THEIR PREFERENCE LIST
            preference_type preference = spot->get_preferences();
            if (preference.size() > 0) {
                ostr << "  preference list:" << std::endl;
                for (preference_type::iterator preference_itr = preference.begin(); preference_itr != preference.end(); ++preference_itr) {
                    ostr << "    " << *preference_itr << std::endl;
                }
            }
        } else {
            ostr << "WARNING: No customer named " << customer_name << std::endl;
        }

    }   else if (token == "print_dvd") {
        std::string dvd_name = read_dvd_name(istr);
//PRINTS THE MOVIE AND THE NUMBER THAT ARE BEING RENTED AND THOSE THAT ARE STILL IN INVENTORY
        inventory_type::iterator spot = find_DVD(inventory, dvd_name);
        if (spot != inventory.end()) {
            ostr << dvd_name << ":" << std::endl;
            if (spot->get_rented() > 0) {
                if (spot->get_rented() > 1 ) {
                    ostr << "  " << spot->get_rented() << " copies checked out ";
                    if (spot->get_remain() > 1 && spot->get_remain()!= 0) {
                        ostr << spot->get_remain() << " and copies available" << std::endl;
                    } else if (spot->get_remain()==1) {
                        ostr << spot->get_remain() << " copy available" << std::endl;
                    } else {
                        ostr << std::endl;
                    }
                } else {
                    ostr << "  " << spot->get_rented() << " copy checked out and ";
                    if (spot->get_remain() > 1) {
                        ostr << spot->get_remain() << " copies available" << std::endl;
                    } else if (spot->get_remain() == 1) {
                        ostr << spot->get_remain() << " copy available" << std::endl;
                    } else {
                        ostr << std::endl;
                    }
                }
            } else {
                ostr << "  " << spot->get_remain() << " copies available" << std::endl;
            }
        } else {
          ostr << "WARNING: No movie named " << dvd_name << " in the inventory" << std::endl;
        }

    } else if (token == "add_preference") {
        std::string customer_name = read_customer_name(istr);
        std::string dvd_name = read_dvd_name(istr);

        customers_type::iterator spot = find_Customer(customers, customer_name);
        if (spot!=customers.end()) {
            inventory_type::iterator position = find_DVD(inventory, dvd_name);
            if (position != inventory.end()) {
                bool in_it = spot->find_movie(dvd_name);
                if (in_it == false) {
                    spot->add_movies(dvd_name);
                } else {
                    ostr << "WARNING: " << spot->get_name() << " already has " << dvd_name << " on his/her preference list!" << std::endl;
                }
            } else {
                ostr << "WARNING: No movie named " << dvd_name << " in the inventory" << std::endl;
            }
        } else {
            ostr << "WARNING: No customer named " << customer_name << std::endl;
        }
    } else {
      std::cerr << "ERROR: Unknown token " << token << std::endl;
      exit(1);
    }
  }
}


// ==================================================================


// A customer name is simply two strings, first & last name
std::string read_customer_name(std::istream &istr) {
  std::string first, last;
  istr >> first >> last;
  return first + " " + last;
}


// A movie name is one or more strings inside of double quotes
std::string read_dvd_name(std::istream &istr) {
  std::string token;
  istr >> token;
  assert (token[0] == '"');
  std::string answer = token;
  while (answer[answer.size()-1] != '"') {
    istr >> token;
    answer += " " + token;
  }
  return answer;
}

// ==================================================================

//
// A basic algorithm for determining which movies to ship to each customer
//
void shipping_algorithm(inventory_type &inventory, customers_type &customers, std::ostream &ostr) {

  ostr << "Ship DVDs" << std::endl;

  // Loop over the customers in priority order
  //
  // Note that we edit the customers list as we go, to add customers
  // to the back of the list if they receive a DVD.  This allows a
  // customer to receive multiple DVDs in one shipment cycle, but only
  // after all other customers have had a chance to receive DVDs.
  //
  customers_type::iterator customer_itr = customers.begin();
  while (customer_itr != customers.end()) {

    // skip this customer if they already have 3 movies or if their
    // preference list is empty (no outstanding requests)
    if (customer_itr->has_max_num_movies() || customer_itr->preference_list_empty()) {
      // move on to the next customer
      customer_itr++;
      continue;
    }

    // a helper flag variable
    bool sent_dvd = false;

    // loop over the customer's preferences
    const preference_type &preferences = customer_itr->get_preferences();
    for (preference_type::const_iterator preferences_itr = preferences.begin();
         preferences_itr != preferences.end(); preferences_itr++) {

      // locate this DVD in the inventory
      inventory_type::iterator inventory_itr = find_DVD(inventory,*preferences_itr);
      if (inventory_itr != inventory.end() && inventory_itr->available()) {

        // if the DVD is available, ship it to the customer!
        ostr << "  " << customer_itr->get_name() << " receives " << *preferences_itr << std::endl;
        inventory_itr->shipped();
        customer_itr->receives(*preferences_itr);

        // move this customer to the back of the priority queue
        // they will get a chance to receive another DVD, but only
        // after everyone else gets a chance
        customers.push_back(*customer_itr);
        customer_itr = customers.erase(customer_itr);

        // after setting the flag to true, leave the iteration over preferences
        sent_dvd = true;
        break;
      }
    }

    // if no DVD was sent to this customer, then we move on to the next customer
    // (do not change this customer's priority for tomorrow's shipment)
    if (!sent_dvd) {
      customer_itr++;
    }
  }

}

// ==================================================================
