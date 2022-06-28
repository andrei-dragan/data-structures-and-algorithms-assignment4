#pragma once

#include "SortedMultiMap.h"


class SMMIterator{
	friend class SortedMultiMap;
private:
	//DO NOT CHANGE THIS PART
	const SortedMultiMap& map;
	SMMIterator(const SortedMultiMap& map);

	//TODO - Representation
	int pos_of_key;
	int pos_of_value;
	int max_size;
	Pair* iteratedArray;
public:
	void first();
	void next();
	bool valid() const;
   	TElem getCurrent() const;
	~SMMIterator();
};

