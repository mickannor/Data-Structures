#ifndef hash_table_h_
#define hash_table_h_
// The set class as a hash table instead of a binary search tree.  The
// primary external difference between ds_set and ds_hashset is that
// the iterators do not step through the hashset in any meaningful
// order.  It is just the order imposed by the hash function.
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <utility>

// The ds_hashset is templated over both the type of key and the type
// of the hash function, a function object.
template < class KeyType, class HashFunc >
class ds_hashset {
private:
  // HASH SET REPRESENTATION
  std::vector< std::list<std::pair<KeyType, int> > > hash_table;  // actual table
  HashFunc hash_func;                            // hash function
  unsigned int hash_size;                        // number of keys

  typedef typename std::list<std::pair<KeyType, int> >::iterator hash_itr;
public:

  // The iterator class is defined as a nested class and does not have
  // to be templated separately over the hash function object type.
  class iterator {
  public:
    friend class ds_hashset;   // allows access to private variables
    hash_itr list_itr; // current iterator at the current index
  private:
    // ITERATOR REPRESENTATION
    ds_hashset* m_hs;
    int hash_index;               // current index in the hash table

  private:
    // private constructors for use by the ds_hashset only
    iterator(ds_hashset * hs) : m_hs(hs), hash_index(-1) {}
    iterator(ds_hashset* hs, int index, hash_itr loc)
      : m_hs(hs), hash_index(index), list_itr(loc) {}

  public:
    // Ordinary constructors & assignment operator
    iterator() : m_hs(0), hash_index(-1)  {}
    iterator(iterator const& itr)
      : m_hs(itr.m_hs), hash_index(itr.hash_index), list_itr(itr.list_itr) {}
    iterator&  operator=(const iterator& old) {
      m_hs = old.m_hs;
      hash_index = old.hash_index;
      list_itr = old.list_itr;
      return *this;
    }

    // The dereference operator need only worry about the current
    // list iterator, and does not need to check the current index.
    const KeyType& operator*() const { return *list_itr; }

    // The comparison operators must account for the list iterators
    // being unassigned at the end.
    friend bool operator== (const iterator& lft, const iterator& rgt)
    { return lft.m_hs == rgt.m_hs && lft.hash_index == rgt.hash_index &&
	(lft.hash_index == -1 || lft.list_itr == rgt.list_itr); }
    friend bool operator!= (const iterator& lft, const iterator& rgt)
    { return lft.m_hs != rgt.m_hs || lft.hash_index != rgt.hash_index ||
	(lft.hash_index != -1 && lft.list_itr != rgt.list_itr); }

    // increment and decrement
    iterator& operator++() {
      this->next();
      return *this;
    }
    iterator operator++(int) {
      iterator temp(*this);
      this->next();
      return temp;
    }
    iterator & operator--() {
      this->prev();
      return *this;
    }
    iterator operator--(int) {
      iterator temp(*this);
      this->prev();
      return temp;
    }

  private:
    // Find the next entry in the table
    void next() {
      ++ list_itr;  // next item in the list

      // If we are at the end of this list
      if (list_itr == m_hs->hash_table[hash_index].end()) {
        // Find the next non-empty list in the table
        for (++hash_index;
             hash_index < int(m_hs->hash_table.size()) && m_hs->hash_table[hash_index].empty();
             ++hash_index) {}

        // If one is found, assign the list_itr to the start
        if (hash_index != int(m_hs->hash_table.size()))
          list_itr = m_hs->hash_table[hash_index].begin();

        // Otherwise, we are at the end
        else
          hash_index = -1;
      }
    }

    // Find the previous entry in the table
    void prev() {
      // If we aren't at the start of the current list, just decrement
      // the list iterator
      if (list_itr != m_hs->hash_table[hash_index].begin())
	list_itr -- ;

      else {
        // Otherwise, back down the table until the previous
        // non-empty list in the table is found
        for (--hash_index; hash_index >= 0 && m_hs->hash_table[hash_index].empty(); --hash_index) {}

        // Go to the last entry in the list.
        list_itr = m_hs->hash_table[hash_index].begin();
        hash_itr p = list_itr; ++p;
        for (; p != m_hs->hash_table[hash_index].end(); ++p, ++list_itr) {}
      }
    }
  };


  // =================================================================
  // HASH SET IMPLEMENTATION

  // Constructor for the table set just accepts the size of the table.
  // The default constructor for the hash function object is
  // implicitly used.
  ds_hashset() : hash_table(100), hash_size(0) {}
  ds_hashset(const int& N) : hash_table(N), hash_size(0) {}

  // Copy constructor just uses the member function copy constructors.
  ds_hashset(const ds_hashset<KeyType, HashFunc>& old)
    : hash_table(old.hash_table), hash_size(old.hash_size) {}

  ~ds_hashset() {}

  ds_hashset& operator=(const ds_hashset<KeyType,HashFunc>& old) {
    if (&old != this)
      *this = old;
  }

  unsigned int size() const { return hash_size; }


  void insert(KeyType const& key, int const& location, float const& f, int& N) {
    if (hash_size >= f * hash_table.size()) {
        N = 2*N+1;
        this->resize_table(2*hash_table.size()+1);
    }
    //  Compute the hash value and then the table index
    unsigned int index = hash_func(key, N);
	//  put it in front and construct an iterator
    if (hash_table[index].size() == 0) {
        hash_table[index].push_front( std::make_pair(key, location) );
    } else {
        int i;
        for (i = 1; i+index < hash_table.size(); ++i) {
            if (hash_table[index+i].size() == 0) {
                hash_table[index+i].push_front( std::make_pair(key, location) );
                break;
            }
        }
        for (i = 0; i < index; ++i) {
            if (hash_table[i].size() == 0) {
                hash_table[i].push_front( std::make_pair(key, location) );
                break;
            }
        }
    }
	hash_size++;
  }


  // Find the key, using hash function, indexing
  iterator find(const KeyType& key, const int& N) {
    unsigned int index = hash_func(key, N);
    //CHECK IF THE WHOLE INDEX IS EQUAL TO THE KEY
    if ((hash_table[index].front()).first == key) {
        return iterator(this, index, hash_table[index].begin());
    } else {
        //SEARCH PAST THE INDEX BECAUSE OF LINEAR PROBING
        for (unsigned int i = 1; i+index < hash_table.size(); ++i) {
            if (hash_table[index+i].size() != 0) {
                if (hash_table[index+i].front().first == key) {
                    return iterator(this, index, hash_table[index+i].begin());
                }
            }
        }
        //IF IT IS NOT AFTER THE INDEX IT MAY BE BEFORE IT
        for (unsigned int i = 0; i < index; ++i) {
            if (hash_table[i].size() != 0) {
                if (hash_table[i].front().first == key) {
                    return iterator(this, index, hash_table[i].begin());
                }
            }
        }
    }
    return this->end();
  }

  KeyType find_by_loc(const KeyType& key, const int& N, const int& location) {
    unsigned int index = hash_func(key, N);
    if ((hash_table[index].front()).second == location) {
        return hash_table[index].front().first;
    } else {
        //SEARCH PAST THE INDEX BECAUSE OF LINEAR PROBING
        for (unsigned int i = 1; i+index < hash_table.size(); ++i) {
            if (hash_table[index+i].size() != 0) {
                if (hash_table[index+i].front().second == location) {
                    return hash_table[index+i].front().first;
                }
            }
        }
        //IF IT IS NOT AFTER THE INDEX IT MAY BE BEFORE IT
        for (unsigned int i = 0; i < index; ++i) {
            if (hash_table[i].size() != 0) {
                if (hash_table[i].front().first == key) {
                    return hash_table[i].front().first;
                }
            }
        }
    }
    return " ";
  }

  // Erase the key
  int erase(const KeyType& key) {
    // Find the key and use the erase iterator function.
    iterator p = find(key);
    if (p == end())
      return 0;
    else {
      erase(p);
      return 1;
    }
  }

  // Erase at the iterator
  void erase(iterator p) {
    hash_table[ p.hash_index ].erase(p.list_itr);
    --hash_size;
  }

  // Find the first entry in the table and create an associated iterator
  iterator begin() {

    iterator p( this );
    for ( p.hash_index = 0; p.hash_index<int(this->hash_table.size()); ++ p.hash_index ){
      if ( !hash_table[p.hash_index].empty() )
      {
        hash_itr q = hash_table[ p.hash_index ].begin();
        p.list_itr = q;
        return p;
      }
    }
    p.hash_index = -1;
    return p;
  }

  // Create an end iterator.
  iterator end() {
    iterator p(this);
    p.hash_index = -1;
    return p;
  }

  // A public print utility.
  void print(std::ostream & ostr) {
    for (unsigned int i=0; i<hash_table.size(); ++i) {
      ostr << i << ": ";
      for (hash_itr p = hash_table[i].begin(); p != hash_table[i].end(); ++p)
        ostr << ' ' << p->first;
      ostr << std::endl;
    }
  }

private:
  // resize the table with the same values but a
  void resize_table(unsigned int new_size) {
    std::vector< std::list<std::pair<KeyType, int> > > old_table = hash_table;
    hash_table.clear();
    hash_table.resize( new_size );
    for ( unsigned int i=0; i<old_table.size(); ++i )
      for ( hash_itr p = old_table[i].begin(); p!=old_table[i].end(); ++p )
	{
	  unsigned int index = hash_func(p->first,  new_size);
	  hash_table[index].push_front(std::make_pair(p->first, p->second));
	}
  }
};
#endif
