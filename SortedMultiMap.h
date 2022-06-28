#pragma once
//DO NOT INCLUDE SMMITERATOR

//DO NOT CHANGE THIS PART
#include <vector>
#include <utility>
typedef int TKey;
typedef int TValue;
typedef std::pair<TKey, TValue> TElem;
#define NULL_TVALUE -111111
#define NULL_TELEM pair<TKey, TValue>(-111111, -111111);
using namespace std;
class SMMIterator;
typedef bool(*Relation)(TKey, TKey);

struct Pair {
    TKey t;
    TValue* elems;
    int cap, size;
    Pair();
};

class SortedMultiMap {
	friend class SMMIterator;
    private:
        Relation r;
        int m1, m2;
        int keys;

        Pair* hashTable;
        int h1(TKey c) const;
        int h2(TKey c) const; 
		
        void resizeAndRehash();
    public:

    //constructor
    SortedMultiMap(Relation r);

	//adds a new key value pair to the sorted multi map
    void add(TKey c, TValue v);

	//returns the values belonging to a given key
    vector<TValue> search(TKey c) const;

	//removes a key value pair from the sorted multimap
	//returns true if the pair was removed (it was part of the multimap), false if nothing is removed
    bool remove(TKey c, TValue v);

    //returns the number of key-value pairs from the sorted multimap
    int size() const;

    //verifies if the sorted multi map is empty
    bool isEmpty() const;

    // returns an iterator for the sorted multimap. The iterator will returns the pairs as required by the relation (given to the constructor)	
    SMMIterator iterator() const;

    // returns the difference between the maximum and the minimum value (if empty, return -1)
    int getValueRange() const;

    // destructor
    ~SortedMultiMap();
};
