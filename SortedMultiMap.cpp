#include "SMMIterator.h"
#include "SortedMultiMap.h"
#include <iostream>
#include <vector>
#include <exception>
using namespace std;

Pair::Pair() {
	this->cap = 1;
	this->size = 0;
	this->elems = new TValue[this->cap];
	this->t = -1;
} // Theta(1)

int SortedMultiMap::h1(TKey c) const {
	return c % this->m1;
} // Theta(1)

int SortedMultiMap::h2(TKey c) const {
	return 1 + (c % this->m2);
} // Theta(1)

SortedMultiMap::SortedMultiMap(Relation r) {
	this->r = r;
	this->m1 = 13;
	this->m2 = this->m1 - 1; 
	this->keys = 0;
	this->hashTable = new Pair[this->m1];
} // Theta(1) 

void SortedMultiMap::resizeAndRehash() {
	Pair* oldHashTable = this->hashTable;
	this->m1 *= 2;
	this->m2 = this->m1 - 1;
	this->keys = 0;

	Pair* newHashTable = new Pair[this->m1];
	this->hashTable = newHashTable;

	for (int i = 0; i < this->m1 / 2; i++) {
		if (oldHashTable[i].t != -1) {
			for (int j = 0; j < oldHashTable[i].size; j++) {
				TValue v = oldHashTable[i].elems[j];
				TKey c = oldHashTable[i].t;
				this->add(c, v);
			}
		}
		delete[] oldHashTable[i].elems;
	}

	delete[] oldHashTable;
} // Theta(N)

void SortedMultiMap::add(TKey c, TValue v) {
	// Search for the key
	int pos_in_hash = this->h1(c);
	int step = 0;

	while (step < this->m1 && hashTable[pos_in_hash].t != -1 && hashTable[pos_in_hash].t != -2 && hashTable[pos_in_hash].t != c) {
		step++;
		pos_in_hash = (this->h1(c) + step * this->h2(c)) % this->m1;
	}

	if (step == this->m1) {
		// There is no place to put the new key => resize and rehash
		this->resizeAndRehash();
		this->add(c, v);
	}
	else {
		if (hashTable[pos_in_hash].t == c) {
			// The key is already inside the hash => we just add the TValue v to the dynammic array
			if (hashTable[pos_in_hash].size == hashTable[pos_in_hash].cap) {
				// Resize the dynammic array
				TValue* newElems = new TValue[this->hashTable[pos_in_hash].cap * 2];
				for (int i = 0; i < this->hashTable[pos_in_hash].size; i++) {
					newElems[i] = this->hashTable[pos_in_hash].elems[i];
				}
				this->hashTable[pos_in_hash].cap *= 2;
				delete[] this->hashTable[pos_in_hash].elems;
				this->hashTable[pos_in_hash].elems = newElems;
			}

			this->hashTable[pos_in_hash].elems[this->hashTable[pos_in_hash].size] = v;
			this->hashTable[pos_in_hash].size++;
		}
		else {
			// The key was not yet added => we need to add it to this empty slot, which is either -1 or -2
			// If it's -1, we add it to the load factor
			// If it's -2, it's just a "DELETED" slot that is already taken into consideration for the load factor 
			if (hashTable[pos_in_hash].t == -1) this->keys++;

			this->hashTable[pos_in_hash].t = c;
			this->hashTable[pos_in_hash].elems[this->hashTable[pos_in_hash].size] = v;
			this->hashTable[pos_in_hash].size++;
		}
	}

	double loadFactor = (double)this->keys / (double)this->m1;
	if (loadFactor > 0.7) {
		// We resize the hash if the load factor becomes bigger
		this->resizeAndRehash();
	}
} // Best Case: Theta(1), Worst Case: Theta(N + m1) => Total: O(N + m1)

vector<TValue> SortedMultiMap::search(TKey c) const {
	int pos_in_hash = this->h1(c);
	int step = 0;
	vector<TValue>ans;

	while (step < this->m1 && hashTable[pos_in_hash].t != -1 && hashTable[pos_in_hash].t != c) {
		step++;
		pos_in_hash = (this->h1(c) + step * this->h2(c)) % this->m1;
	}

	if (step == this->m1 || hashTable[pos_in_hash].t == -1) {
		return vector<TValue>();
	}
	else {
		for (int i = 0; i < this->hashTable[pos_in_hash].size; i++) {
			ans.push_back(this->hashTable[pos_in_hash].elems[i]);
		}
	}
	return ans; 
} // Best Case: Theta(1), Worst Case: Theta(m1) => Total: O(m1) 

bool SortedMultiMap::remove(TKey c, TValue v) {
	// Search for the key
	int pos_in_hash = this->h1(c);
	int step = 0;

	while (step < this->m1 && hashTable[pos_in_hash].t != -1 && hashTable[pos_in_hash].t != c) {
		step++;
		pos_in_hash = (this->h1(c) + step * this->h2(c)) % this->m1;
	}

	if (step == this->m1 || hashTable[pos_in_hash].t == -1) {
		// The value was not found
		return false;
	}
	else {
		bool found = false;
		for (int i = 0; i < this->hashTable[pos_in_hash].size; i++) {
			if (this->hashTable[pos_in_hash].elems[i] == v) {
				// We remove this value, by swapping it with the last one
				this->hashTable[pos_in_hash].elems[i] = this->hashTable[pos_in_hash].elems[this->hashTable[pos_in_hash].size - 1];
				found = true;
				break;
			}
		}

		if (found == false)
			return false;

		this->hashTable[pos_in_hash].size--;

		if (this->hashTable[pos_in_hash].size == 0) {
			// we remove the whole key - mark it as deleted with -2 (in this case, the load factor doesn't change)
			this->hashTable[pos_in_hash].t = -2; 
		}
		return true;
	}
} // Best Case: Theta(1), Worst Case: Theta(m1) => Total: O(m1) 


int SortedMultiMap::size() const {
	int ans = 0;
	for (int i = 0; i < this->m1; i++) {
		if (this->hashTable[i].t != -1) {
			ans += this->hashTable[i].size;
		}
	}
	return ans;
} // Theta(m1)

bool SortedMultiMap::isEmpty() const {
	return (this->size() == 0);
} // Theta(m1)

SMMIterator SortedMultiMap::iterator() const {
	return SMMIterator(*this);
}

SortedMultiMap::~SortedMultiMap() {
	for (int i = 0; i < this->m1; i++) {
		delete[] this->hashTable[i].elems;
	}
	delete[] this->hashTable;
} // Theta(m1)

int SortedMultiMap::getValueRange() const {
	int mini = INT_MAX;
	int maxi = -INT_MAX;
	bool found = false;

	for (int i = 0; i < this->m1; i++) {
		if (this->hashTable[i].t != -1 && this->hashTable[i].t != -2) {
			// we have some elements
			for (int j = 0; j < this->hashTable[i].size; j++) {
				int x = this->hashTable[i].elems[j];
				if (x > maxi) maxi = x;
				if (x < mini) mini = x;
				found = true;
			}
		}
	}

	if (found == false)
		return -1;
	else
		return maxi - mini;
} // Theta(N)
 