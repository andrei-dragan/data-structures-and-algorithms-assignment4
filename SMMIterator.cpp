#include "SMMIterator.h"
#include "SortedMultiMap.h"


SMMIterator::SMMIterator(const SortedMultiMap& d) : map(d){
	// Create a new array containing all the elements
	int N = 0;
	for (int i = 0; i < this->map.m1; i++) {
		if (this->map.hashTable[i].t != -1 && this->map.hashTable[i].t != -2) N++;
	}
	this->iteratedArray = new Pair[N];

	this->max_size = 0;
	for (int i = 0; i < this->map.m1; i++) {
		if (this->map.hashTable[i].t != -1 && this->map.hashTable[i].t != -2) {

			this->iteratedArray[this->max_size].cap = this->map.hashTable[i].cap;
			this->iteratedArray[this->max_size].size = this->map.hashTable[i].size;
			this->iteratedArray[this->max_size].t = this->map.hashTable[i].t;

			delete[] this->iteratedArray[this->max_size].elems;
			this->iteratedArray[this->max_size].elems = this->map.hashTable[i].elems; 
			
			int k = this->max_size;
			while (k > 0 && !this->map.r(this->iteratedArray[k - 1].t, this->iteratedArray[k].t)) {
				swap(this->iteratedArray[k].cap, this->iteratedArray[k - 1].cap);
				swap(this->iteratedArray[k].size, this->iteratedArray[k - 1].size);
				swap(this->iteratedArray[k].t, this->iteratedArray[k - 1].t);
				swap(this->iteratedArray[k].elems, this->iteratedArray[k - 1].elems);
				k--;
			}
			this->max_size++;
		}
	}
	this->pos_of_key = 0;
	this->pos_of_value = 0;
} // Theta(m1^2) 

void SMMIterator::first(){
	this->pos_of_key = 0;
	this->pos_of_value = 0;
} // Theta(1)

void SMMIterator::next(){
	if (this->valid()) {
		if (this->pos_of_value == this->iteratedArray[pos_of_key].size - 1) {
			pos_of_key++;
			pos_of_value = 0;
		}
		else {
			pos_of_value++;
		}
	}
	else {
		throw exception();
	}
} // Theta(1)

bool SMMIterator::valid() const{
	return (this->pos_of_key < this->max_size);
} // Theta(1)

TElem SMMIterator::getCurrent() const{
	if (this->valid()) {
		TElem x;
		x.first = this->iteratedArray[pos_of_key].t;
		x.second = this->iteratedArray[pos_of_key].elems[pos_of_value];
		return x;
	}
	else {
		throw exception();
	}
} // Theta(1)

SMMIterator::~SMMIterator() {
	delete[] this->iteratedArray;
} // Theta(1)


