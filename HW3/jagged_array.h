#ifndef jagged_array_h_
#define jagged_array_h_
#include <iostream>
#include <string.h>
#include <string>
using namespace std;

template <class T> class JaggedArray {
    typedef unsigned int size_type;

    public:
        // CONVERTERS
        void pack();
        void unpack();
        // ACCESSORS
        int numBins() const {return numBins_;}
        int numElements() const {return numElements_;}
        bool isPacked() const {return packed_;};
        const int numElementsInBin(const size_type& bin);
        const T& getElement(const size_type& bin, const size_type& position);
        void print();
        //MODIFIERS
        void addElement(const size_type& bin, const T& object);
        void removeElement(const size_type& bin, const T& object);
        void clear();
        //CONSTRUCTOR, ASSIGNMENT OPERATOR, & DESTRUCTOR
        JaggedArray(const size_type numBins) { this->create(numBins); }
        JaggedArray& operator=(const JaggedArray& j);
        JaggedArray(const JaggedArray& j) {copy(j);}
        ~JaggedArray();

    private:
        int numBins_;
        int numElements_;
        bool packed_;
        T** unpacked_ptr;
        T* packed_ptr;
        int* counts_ptr;
        int* offsets_ptr;
        void copy(const JaggedArray<T>& j);
        void create(unsigned int numBins);
        void create();
};
template <class T> void JaggedArray<T>::create(const size_type numBins) {
    // I set my arrays equal to zero cause they can't be set equal to nullptr and I assume 0 is kinda the same as null in this case cause of /0 being null.
    numBins_ = numBins;
    numElements_ = 0;
    packed_ = false;
    unpacked_ptr = new T*[numBins];
    for (unsigned int x = 0; x < numBins_; ++x) {
        unpacked_ptr[x] = nullptr;
    }
    counts_ptr = new int[numBins];
    for (unsigned int x = 0; x < numBins_; ++x) {
        counts_ptr[x] = 0;
    }
    offsets_ptr = new int[numBins];
    for (unsigned int x = 0; x < numBins_; ++x) {
        offsets_ptr[x] = 0;
    }
}
template <class T> void JaggedArray<T>::unpack() {
    //sets the appropriate values to nullptr
    int* offsets = nullptr;
    T* packed_values = nullptr;
    packed_ = false;
}
template <class T> void JaggedArray<T>::pack() {
    packed_ = true;
    //sets the appropriate values to nullptr
    int* counts = {nullptr};
    T* unpacked_values = {nullptr};
    packed_ptr = new T[numElements_];
    int position = 0;
    // Goes into each pointer in unpacked_ptr that isn't null and put the elements in the array the pointer points to in the packed_values position
    for (unsigned int i = 0; i < numBins_; ++i) {
        if (unpacked_ptr[i] != nullptr) {
            for (unsigned int j = 0; j < counts_ptr[i]; ++j) {
                packed_ptr[position] = unpacked_ptr[i][j];
                position += 1;
            }
        }
    }
    //Fill in the elements for offsets
    for (unsigned int i = 1; i < numBins_; ++i) {
        offsets_ptr[i] = counts_ptr[i-1] + offsets_ptr[i-1];
    }
}
template <class T>const int JaggedArray<T>::numElementsInBin(const size_type& bin) {
    return counts_ptr[bin];
}
template <class T> const T& JaggedArray<T>::getElement(const size_type& bin, const size_type& position) {
    return unpacked_ptr[bin][position];
}
template <class T> void JaggedArray<T>::addElement(const size_type& bin, const T& object) {
    // increases the # of elements and the number of elements in bin.
    if (packed_ == false) {
        numElements_ += 1;
        counts_ptr[bin] += 1;
        // creates a temp pointer that will hold the old and new values then clears old space and sets unpacked_ptr and temp then deletes temp
        T* temp = new T[counts_ptr[bin]];
        temp[counts_ptr[bin]-1] = object;
        for (unsigned int x = 0; x < (counts_ptr[bin] - 1); ++x) {
            if ((counts_ptr[bin] - 1) != 0) {
                temp[x] = unpacked_ptr[bin][x];
            }
        }
        delete [] unpacked_ptr[bin];
        unpacked_ptr[bin] = temp;
        temp = new T[1];
        delete [] temp;
    } else {
        cerr << "Cannot edit in packed mode" << endl;
        exit(1);
    }
}
template <class T> void JaggedArray<T>::removeElement(const size_type& bin, const T& object) {
    if (packed_ == false) {
        numElements_ -= 1;
        counts_ptr[bin] -= 1;
        T* temp = new T[counts_ptr[bin]];
        for (unsigned int x = 0; x < (counts_ptr[bin]); ++x) {
            if (unpacked_ptr[bin][counts_ptr[bin]] != object) {
                temp[x] = unpacked_ptr[bin][counts_ptr[bin]];
            } else {
                x += 1;
            }
        }
        delete [] unpacked_ptr[bin];
        unpacked_ptr[bin] = temp;
        temp = new T[1];
        delete [] temp;
    } else {
        cerr << "Cannot edit in packed mode" << endl;
        exit(1);
    }
}
template <class T> void JaggedArray<T>::clear() {
    //deletes all old space including packed_ptr only if it's been initialized
    delete [] unpacked_ptr;
    delete [] offsets_ptr;
    delete [] counts_ptr;
    if (packed_ptr == new T[numElements_]) {
        delete [] packed_ptr;
    }
    create(this->numBins_);
}
template <class T> JaggedArray<T>& JaggedArray<T>::operator=(const JaggedArray<T>& j) {
    if (this != &j) {
        delete [] unpacked_ptr;
        if (packed_ptr == new T[numElements_]) {
            delete [] packed_ptr;
        }
        delete [] counts_ptr;
        delete [] offsets_ptr;
        this -> copy(j);
    }
    return *this;
}
template <class T> JaggedArray<T>::~JaggedArray() {
    delete [] unpacked_ptr;
    if (packed_ptr == new T[numElements_]) {
        delete [] packed_ptr;
    }
    delete [] counts_ptr;
    delete [] offsets_ptr;
}
template <class T> void JaggedArray<T>::copy(const JaggedArray<T>& j) {
    this->numBins_ = j.numBins_;
    this->numElements_ = j.numElements_;
    this->packed_ = j.packed_;
    this->unpacked_ptr = new T*[this->numBins_];
    for (unsigned int x = 0; x < this->numBins_; ++x) {
        this->unpacked_ptr[x] = new T[this->counts_ptr[x]];
    }
    for (unsigned int x = 0; x < this->numBins_; ++x) {
        for (unsigned int y = 0; y < this->counts_ptr[x]; ++y) {
            if (counts_ptr[x] != 0) {
                this ->unpacked_ptr[x][y] = j.unpacked_ptr[x][y];
            }
        }
    }
    this->packed_ptr = new T[this->numBins_];
    for (unsigned int x = 0; x < this->numBins_; ++x) {
        this -> packed_ptr[x] = j.packed_ptr[x];
    }
    this->counts_ptr = new int[this->numBins_];
    for (unsigned int x = 0; x < this->numBins_; ++x) {
        this -> counts_ptr[x] = j.counts_ptr[x];
    }
    this->offsets_ptr = new int[this->numBins_];
    for (unsigned int x = 0; x < this->numBins_; ++x) {
        this -> offsets_ptr[x] = j.offsets_ptr[x];
    }
};
template <class T> void JaggedArray<T>::print() {
    if (packed_ == false) {
        const string spaces(10, ' ');
        cout << "unpacked JaggedArray" << endl << "  ";
        cout << "num_bins: " << numBins_ << endl << "  ";
        cout << "num_elements: " << numElements_ << endl << "  ";
        cout << "count:  ";
        for (unsigned int x = 0; x < numBins_; ++x) {
            cout << counts_ptr[x] << " ";
            if (x == numBins_ - 1) {
                cout << endl << "  ";
            }
        }
        cout << "values: ";
        unsigned int z = 0;
        unsigned int bin = 0;
        unsigned int highest = 0;
        for (unsigned int p = 0; p < numBins_; ++p) {
            if (highest < counts_ptr[p]) {
                highest = counts_ptr[p];
            }
        }
        while (z < highest) {
            if (bin != numBins_ - 1) {
                if (counts_ptr[bin] != 0 && z <= counts_ptr[bin] - 1) {
                    cout << unpacked_ptr[bin][z] << " ";
                    bin += 1;
                } else {
                    cout << "  ";
                    bin += 1;
                }
            } else {
                if (z != highest - 1) {
                    if (counts_ptr[bin] != 0 && z <= counts_ptr[bin] - 1) {
                        cout << unpacked_ptr[bin][z] << endl << spaces;
                    } else {
                        cout << " " << endl << spaces;
                    }
                    bin = 0;
                    z += 1;
                } else {
                    if (counts_ptr[bin] != 0 && z <= counts_ptr[bin] - 1) {
                        cout << unpacked_ptr[bin][z] << endl << endl;
                    } else {
                        cout << " " << endl << endl;
                    }
                    bin = 0;
                    z += 1;
                }
            }
        }
    } else {
        cout << "packed JaggedArray" << endl << "  ";
        cout << "num_bins: " << numBins_ << endl << "  ";
        cout << "num_elements: " << numElements_ << endl << "  ";
        cout << "offsets:  ";
        for (unsigned int x = 0; x < numBins_; ++x) {
            cout << offsets_ptr[x] << " ";
            if (x == numBins_ - 1) {
                cout << endl << "  ";
            }
        }
        cout << "values:   ";
        for (unsigned int x = 0; x < numElements_; ++x) {
            if (x != numElements_ - 1) {
                cout << packed_ptr[x] << " ";
            } else {
                cout << packed_ptr[x] << endl << endl;
            }
        }
    }
}
#endif
