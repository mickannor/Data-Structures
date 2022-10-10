#ifndef unrolled_h_
#define unrolled_h_
#include <string.h>
#include <iostream>

const int NUM_ELEMENTS_PER_NODE = 6;

// NODE CLASS
template <class T>
class Node {
public:
  Node() : next_(nullptr), prev_(nullptr), num_elements(0) {}
  // REPRESENTATION
  Node<T>* next_;
  Node<T>* prev_;
  T elements_[NUM_ELEMENTS_PER_NODE];
  int num_elements;
};

// A "forward declaration" of this class is needed
template <class T> class UnrolledLL;
// -----------------------------------------------------------------
// LIST ITERATOR
template <class T>
class list_iterator {
public:
    // default constructor, copy constructor, assignment operator, & destructor
    list_iterator(Node<T>* p=nullptr) : ptr_(p), offset_(0) {}
    // NOTE: the implicit compiler definitions of the copy constructor,
    // assignment operator, and destructor are correct for this class

    // dereferencing operator gives access to the value at the pointer
    T& operator*()  { return ptr_->elements_[offset_]; }

    // increment & decrement operators
    list_iterator<T>& operator++(); /* pre-increment, e.g., ++iter */
    list_iterator<T> operator++(int); // post-increment, e.g., iter++
    list_iterator<T>& operator--(); // pre-decrement, e.g., --iter
    list_iterator<T> operator--(int); // post-decrement, e.g., iter--

    // the UnrolledLL class needs access to the private ptr_ member variable
    friend class UnrolledLL<T>;

    // Comparions operators are straightforward
    bool operator==(const list_iterator<T>& r) const {return ptr_ == r.ptr_; }
    bool operator!=(const list_iterator<T>& r) const {return ptr_ != r.ptr_; }

private:
    // REPRESENTATION
    Node<T>* ptr_; // ptr to node in the list
    int offset_;
};

// LIST CLASS DECLARATION
// Note that it explicitly maintains the size of the list.
template <class T>
class UnrolledLL {
public:
  // default constructor, copy constructor, assignment operator, & destructor
    UnrolledLL() : head_(nullptr), tail_(nullptr), size_(0) {}
    UnrolledLL(const UnrolledLL<T>& old) { copy_list(old); }
    UnrolledLL& operator= (const UnrolledLL<T>& old);
    ~UnrolledLL() { destroy_list(); }

    typedef list_iterator<T> iterator;

    // simple accessors & modifiers
    unsigned int size() const { return size_; }
    bool empty() const { return head_ == nullptr; }
    void clear() { destroy_list(); }
    void print(std::ostream& ostr);

    // read/write access to contents
    const T& front() const { return head_->elements_[0];  }
    T& front() { return head_->elements_[0]; }
    const T& back() const { return tail_->elements_[tail_->num_elements-1]; }
    T& back() { return tail_->elements_[tail_->num_elements-1]; }

    // modify the linked list structure
    void push_front(const T& v);
    void pop_front();
    void push_back(const T& v);
    void pop_back();

    iterator erase(iterator itr);
    iterator insert(iterator itr, const T& v);
    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(NULL); }

private:
    // private helper functions
    void copy_list(const UnrolledLL<T>& old);
    void destroy_list();

    //REPRESENTATION
    Node<T>* head_;
    Node<T>* tail_;
    unsigned int size_;
};

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//I FOUND OUT ABOUT MEMMOVE AND MEMCPY WHEN I LOOKED UP HOW TO SET TWO ARRAYS EQUAL TO EACH OTHER
//FUNCTION IMPLEMENTATION
template <class T>
list_iterator<T>& list_iterator<T>::operator++()  {
    if ((offset_ != NUM_ELEMENTS_PER_NODE - 1) && (offset_!= ptr_->num_elements - 1)) {
        offset_ += 1;
    } else {
        ptr_ = ptr_->next_;
        offset_ = 0;
    }
    return *this;
}
template <class T>
list_iterator<T> list_iterator<T>::operator++(int) {
    list_iterator<T> temp(*this);
    if ((offset_ != NUM_ELEMENTS_PER_NODE - 1) && (offset_!= ptr_->num_elements - 1)) {
        offset_ += 1;
    } else {
        ptr_ = ptr_->next_;
        offset_ = 0;
    }
    return temp;
}
template <class T>
list_iterator<T>& list_iterator<T>::operator--() {
    if (offset_ != 0) {
        offset_ -= 1;
    } else {
        ptr_ = ptr_->prev_;
        offset_ = 0;
    }
    return *this;
}
template <class T>
list_iterator<T> list_iterator<T>::operator--(int) {
    if (offset_ != 0) {
        offset_ -= 1;
        offset_ = 0;
    } else {
        ptr_ = ptr_->prev_;
        offset_ = 0;
    }
    return *this;
}
template <class T>
UnrolledLL<T>& UnrolledLL<T>::operator= (const UnrolledLL<T>& old) {
  // check for self-assignment
  if (&old != this) {
    destroy_list();
    copy_list(old);
  }
  return *this;
}

template <class T>
void UnrolledLL<T>::push_front(const T& v) {
    //THIS IS THE FIRST ELEMENT ADDED TO THE LIST
    if (!tail_) {
        ++size_;
        T temp[NUM_ELEMENTS_PER_NODE];
        temp[0] = v;
        typename UnrolledLL::iterator newp = new Node<T>();
        newp.ptr_->num_elements = 1;
        head_ = tail_ = newp.ptr_;
        memmove(newp.ptr_->elements_, temp, sizeof(temp));
    } else {
        //THE ELEMENT IS BEING ADDED TO A PRE-EXISTING NODE
        if (head_->num_elements < NUM_ELEMENTS_PER_NODE) {
            ++size_;
            T temp[NUM_ELEMENTS_PER_NODE];
            temp[0] = v;
            for (unsigned int i = 0; i < head_->num_elements; ++i) {
                temp[i+1] = head_->elements_[i];
            }
            head_->num_elements += 1;
            memmove(head_->elements_, temp, sizeof(temp));
        //A NEW NODE MUST BE MADE FOR THE ELEMENT
        } else if (head_->num_elements == NUM_ELEMENTS_PER_NODE) {
            ++size_;
            T temp[NUM_ELEMENTS_PER_NODE];
            temp[0] = v;
            typename UnrolledLL::iterator newp = new Node<T>();
            newp.ptr_->num_elements = 1;
            newp.ptr_->prev_ = 0;
            newp.ptr_->next_ = head_;
            head_->prev_ = newp.ptr_;
            head_ = newp.ptr_;
            memmove(head_->elements_, temp, sizeof(temp));
        }
    }
}

template <class T>
void UnrolledLL<T>::pop_front() {
    if (tail_) {
        //THERE IS MORE THAN ONE ELEMENT IN THE HEAD NODE
        if (head_->num_elements > 1) {
            T temp[NUM_ELEMENTS_PER_NODE];
            for (unsigned int i = 1; i < head_->num_elements; ++i) {
                temp[i-1] = head_->elements_[i];
            }
            memmove(head_->elements_, temp, sizeof(temp));
            head_->num_elements -= 1;
        //REMOVING THE ONLY ELEMENT IN THE HEAD NODE
        } else if (head_->num_elements == 1) {
            head_ = head_->next_ ;
            head_->prev_ = 0;
        }
        --size_;
    }
}

template <class T>
void UnrolledLL<T>::push_back(const T& v) {
    //THE FIRST NODE IS BEING MADE
    if (!tail_) {
        T temp[NUM_ELEMENTS_PER_NODE];
        temp[0] = v;
        typename UnrolledLL::iterator newp = new Node<T>();
        newp.ptr_->num_elements = 1;
        head_ = tail_ = newp.ptr_;
        memmove(newp.ptr_->elements_, temp, sizeof(temp));
    } else {
        //A NODE EXIST BUT IT IS NOT FULL
        if (tail_->num_elements < NUM_ELEMENTS_PER_NODE) {
            tail_->elements_[tail_->num_elements] = v;
            tail_->num_elements += 1;
        } else if (tail_->num_elements == NUM_ELEMENTS_PER_NODE) {
        //A NEW NODE IS BEING MADE BECAUSE THE PREVIOUS ONE IS FULL
            T temp[NUM_ELEMENTS_PER_NODE];
            temp[0] = v;
            typename UnrolledLL::iterator newp = new Node<T>();
            memmove(newp.ptr_->elements_, temp, sizeof(temp));
            newp.ptr_->num_elements = 1;
            newp.ptr_->prev_ = tail_;
            tail_->next_ = newp.ptr_;
            tail_ = newp.ptr_;
        }
    }
    ++size_;
}

template <class T>
void UnrolledLL<T>::pop_back() {
    if (tail_) {
        //IF THERE IS MORE THAN ONE ELEMENT IN THE TAIL NODE
        if (tail_->num_elements > 1) {
            T temp[NUM_ELEMENTS_PER_NODE];
            for (unsigned int i = 0; i < tail_->num_elements-1; ++i) {
                temp[i] = tail_->elements_[i];
            }
            memmove(tail_->elements_, temp, sizeof(temp));
            tail_->num_elements -= 1;
        // THERE IS ONLY ONE ELEMENT IN THE TAIL NODE
        } else if (tail_->num_elements == 1) {
            tail_ = tail_->prev_;
            tail_->next_ = 0;
        }

    }
    --size_;
}

// do these lists look the same (length & contents)?
template <class T>
bool operator== (UnrolledLL<T>& left, UnrolledLL<T>& right) {
    if (left.size() != right.size()) return false;
    typename UnrolledLL<T>::iterator left_itr = left.begin();
    typename UnrolledLL<T>::iterator right_itr = right.begin();
    // walk over both lists, looking for a mismatched value
    while (left_itr != left.end()) {
        if (*left_itr != *right_itr) return false;
        left_itr++; right_itr++;
    }
    return true;
}

template <class T>
bool operator!= (UnrolledLL<T>& left, UnrolledLL<T>& right){ return !(left==right); }
template <class T>
typename UnrolledLL<T>::iterator UnrolledLL<T>::erase(iterator itr) {
    //IF THERE IS MORE THAN ONE ELEMENT LEFT IN THE LIST
    if (itr.ptr_->num_elements > 1) {
        unsigned int j = 0;
        T temp[NUM_ELEMENTS_PER_NODE];
        for (unsigned int i = 0; i < itr.ptr_->num_elements; ++i) {
            if (itr.ptr_->elements_[i] == itr.ptr_->elements_[itr.offset_]) {
                j += 1;
                temp[i] = itr.ptr_->elements_[j];
                j += 1;
            } else {
                temp[i] = itr.ptr_->elements_[j];
                j += 1;
            }
        }
        memmove(itr.ptr_->elements_, temp, sizeof(temp));
        //IF THE ELEMENT BEING ERASED IS THE LAST ONE IN IT'S LIST, ITR NEEDS TO BE MOVED TO THE NEXT NODE
        if (itr.offset_ == itr.ptr_->num_elements-1) {
            itr.ptr_->num_elements -= 1;
            itr = itr.ptr_->next_;
            itr.offset_ = 0;
            --size_;
            return itr;
        } else {
            itr.ptr_->num_elements -= 1;
            --size_;
            return itr;
        }
    //IF IT IS THE LAST ELEMENT IN THE LIST
    } else if (itr.ptr_->num_elements == 1) {
        iterator result(itr.ptr_->next_);
        //THE LIST BEING DELETED IS THE HEAD
        if (itr.ptr_ == head_) {
            head_ = head_->next_;
            head_->prev_ = 0;
        //THE LIST BEING DELETED IS THE TAIL
        } else if (itr.ptr_ == tail_) {
            tail_ = tail_->prev_;
            tail_->next_ = 0;
        } else {
            itr.ptr_->prev_->next_ = itr.ptr_->next_;
            itr.ptr_->next_->prev_ = itr.ptr_->prev_;
        }
        delete itr.ptr_;
        --size_;
        return result;
    }
    return itr;
}
template <class T>
typename UnrolledLL<T>::iterator UnrolledLL<T>::insert(iterator itr, const T& v) {
    //IF THE INSERTION DOESN'T CAUSE THE ARRAY TO OVERFLOW
    if ((itr.ptr_->num_elements) < NUM_ELEMENTS_PER_NODE) {
        unsigned int j = 0;
        ++size_;
        itr.ptr_->num_elements = itr.ptr_->num_elements += 1;
        T temp[NUM_ELEMENTS_PER_NODE];
        for (unsigned int i = 0; i < itr.ptr_->num_elements; ++i) {
            if (itr.ptr_->elements_[i] == itr.ptr_->elements_[itr.offset_]) {
                temp[i] = v;
                i += 1;
                temp[i] = itr.ptr_->elements_[itr.offset_];
                j += 1;
            } else {
                temp[i] = itr.ptr_->elements_[j];
                j += 1;
            }
        }
        memmove(itr.ptr_->elements_, temp, sizeof(temp));
    //IF THE INSERTION CAUSES THE ARRAY TO OVERFLOW
    } else if ((itr.ptr_->num_elements) == NUM_ELEMENTS_PER_NODE) {
        unsigned int j = 0;
        ++size_;
        itr.ptr_->num_elements = itr.offset_ + 1;
        T temp1[NUM_ELEMENTS_PER_NODE];
        T temp2[NUM_ELEMENTS_PER_NODE];
        typename UnrolledLL::iterator p = new Node<T>();
        for (unsigned int i = 0; i < itr.offset_+1; ++i) {
            if (itr.ptr_->elements_[i] != itr.ptr_->elements_[itr.offset_]) {
                temp1[i] = itr.ptr_->elements_[j];
                j += 1;
            } else {
                temp1[i] = v;
                unsigned int x = itr.offset_;
                for (unsigned int j = 0; j < ((NUM_ELEMENTS_PER_NODE + 1) - (itr.offset_ + 1)); ++j) {
                    temp2[j] = itr.ptr_->elements_[x];
                    x += 1;
                    p.ptr_->num_elements += 1;
                }
            }
        }
        memmove(itr.ptr_->elements_, temp1, sizeof(temp1));
        //IF THE NEW NODE MADE IS THE NEW TAIL
        if (itr.ptr_ == tail_) {
            tail_ = p.ptr_;
            p.ptr_->next_ = 0;
            p.ptr_->prev_ = itr.ptr_;
            itr.ptr_->next_ = p.ptr_;
        //IF THE NEW NODE IS NOT THE NEW TAIL
        } else {
            itr.ptr_->next_->prev_ = p.ptr_;
            p.ptr_->next_ = itr.ptr_->next_;
            p.ptr_->prev_ = itr.ptr_;
            itr.ptr_->next_ = p.ptr_;
        }
        memmove(p.ptr_->elements_, temp2, sizeof(temp2));
        return iterator(p);
    }
    return itr;
}
template <class T>
void UnrolledLL<T>::copy_list(const UnrolledLL<T>& old) {
    size_ = old.size_;
    // Handle the special case of an empty list.
    if (size_ == 0) {
        head_ = tail_ = 0;
        return;
    }
    // Create a new head node.
    head_ = new Node<T>();
    memcpy(head_->elements_, old.head_->elements_, sizeof(old.head_->elements_));
    head_->num_elements = old.head_->num_elements;
    // tail_ will point to the last node created and therefore will move
    // down the new list as it is built
    tail_ = head_;
    // old_p will point to the next node to be copied in the old list
    Node<T>* old_p = old.head_->next_;
    // copy the remainder of the old list, one node at a time
    while (old_p) {
        tail_->next_ = new Node<T>();
        memcpy(tail_->next_->elements_, old_p->elements_, sizeof(old_p->elements_));
        tail_->next_->num_elements = old_p->num_elements;
        tail_->next_->prev_ = tail_;
        tail_ = tail_->next_;
        old_p = old_p->next_;
    }
}
template <class T>
void UnrolledLL<T>::destroy_list() {
    if (head_ != tail_) {
        head_ = head_->next_;
        delete head_->prev_;
        head_->prev_ = 0;
    } else {
        head_ = tail_ = 0;
        this->size_ = 0;
    }
}
template <class T>
void UnrolledLL<T>::print(std::ostream& ostr) {
    if (this->size_ > 0) {
        ostr << "UnrolledLL, size: " << this->size() << std::endl;
        for (typename UnrolledLL<T>::iterator iter = this->begin(); iter != this->end(); ++iter) {
        if (iter.offset_ == 0 && iter.ptr_->elements_[iter.offset_] != this->back() && iter.offset_ != iter.ptr_->num_elements-1 ) {
                ostr << "  node:[" << iter.ptr_->num_elements << "] ";
                ostr << *iter << " ";
            } else if (iter.offset_ == 0 && iter.ptr_->elements_[iter.offset_] == this->back()) {
                ostr << "  node:[" << iter.ptr_->num_elements << "] ";
                ostr << *iter << " " << std::endl;
                break;
            } else if (iter.offset_ == 0 && iter.offset_ == iter.ptr_->num_elements-1) {
                ostr << "  node:[" << iter.ptr_->num_elements << "] ";
                ostr << *iter << " " << std::endl;
            } else if (iter.offset_ < NUM_ELEMENTS_PER_NODE-1 && iter.offset_ != iter.ptr_->num_elements-1) {
                ostr << *iter << " ";
            } else if (iter.offset_ == NUM_ELEMENTS_PER_NODE-1 || iter.offset_ == iter.ptr_->num_elements-1) {
                ostr << *iter << " " << std::endl;
            }
        }
    } else {
        ostr << "  node:[0]" << std::endl;
    }
}

#endif
