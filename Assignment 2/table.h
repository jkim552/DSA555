/*************************************************************/
/*                                                           */
/*  Starter file for a2                                      */
/*                                                           */
/*  Author1 Name: Minsu Kim                                  */
/*                                                           */
/*  Author2 Name: JunHee Kim                                 */
/*                                                           */
/*                                                           */
/*************************************************************/

#include <string>
#include <utility>
#include <functional>
#include "mylist.h"

template <class TYPE>
class Table{
public:
	Table(){}
	virtual void update(const std::string& key, const TYPE& value)=0;
	virtual bool remove(const std::string& key)=0;
	virtual bool find(const std::string& key, TYPE& value)=0;
	virtual int numRecords() const = 0;
	virtual bool isEmpty() const = 0;
   virtual int capacity() const = 0;
	virtual ~Table(){}
};

template <class TYPE>
class SimpleTable:public Table<TYPE>{

	struct Record{
		TYPE data_;
		std::string key_;
		Record(const std::string& key, const TYPE& data){
			key_=key;
			data_=data;
		}
	};

	Record** records_;   //the table
	int maxExpected_;       //capacity of the array

	void grow(){
		Record** tmp=new Record*[maxExpected_*2 +1];
		for(int i=0;i<maxExpected_;i++){
			tmp[i]=records_[i];
		}
		for(int i=maxExpected_;i <maxExpected_*2+1;i++){
			tmp[i]=nullptr;
		}
		delete[] records_;
		records_=tmp;
		maxExpected_*=2;
	}
public:
	SimpleTable(int capacity);
	SimpleTable(const SimpleTable& rhs);
	SimpleTable(SimpleTable&& rhs);
	virtual void update(const std::string& key, const TYPE& value);
	virtual bool remove(const std::string& key);
	virtual bool find(const std::string& key, TYPE& value);
	virtual const SimpleTable& operator=(const SimpleTable& rhs);
	virtual const SimpleTable& operator=(SimpleTable&& rhs);
	virtual ~SimpleTable();
	virtual bool isEmpty() const;
	virtual int numRecords() const;
	virtual int capacity() const;

};

template <class TYPE>
int SimpleTable<TYPE>::numRecords() const{
	int rc=0;
	for(int i=0;records_[i]!=nullptr;i++){
		rc++;
	}
	return rc;
}



template <class TYPE>
SimpleTable<TYPE>::SimpleTable(int capacity): Table<TYPE>(){
	records_=new Record*[capacity+1];
	maxExpected_=capacity;
	for(int i=0;i<maxExpected_+1;i++){
		records_[i]=nullptr;
	}
}

template <class TYPE>
SimpleTable<TYPE>::SimpleTable(const SimpleTable<TYPE>& rhs){
	records_=new Record*[rhs.maxExpected_+1];
	maxExpected_=rhs.maxExpected_;
    for(int i=0;i<maxExpected_+1;i++){
        records_[i]=nullptr;
    }
	for(int i=0;i<rhs.numRecords();i++){
		update(rhs.records_[i]->key_,rhs.records_[i]->data_);
	}
}
template <class TYPE>
SimpleTable<TYPE>::SimpleTable(SimpleTable<TYPE>&& rhs){
	maxExpected_=rhs.maxExpected_;
	records_=rhs.records_;
	rhs.records_=nullptr;
	rhs.maxExpected_=0;
}

template <class TYPE>
void SimpleTable<TYPE>::update(const std::string& key, const TYPE& value){
	int idx=-1;
	int sz=numRecords();
	for(int i=0;i<sz;i++){
		if(records_[i]->key_ == key){
			idx=i;
		}
	}
	if(idx==-1){
		if(numRecords() == maxExpected_){
			grow();
		}
		records_[numRecords()]=new Record(key,value);
		for(int i=numRecords()-1;i>0 && records_[i]->key_ < records_[i-1]->key_;i--){
			Record* tmp=records_[i];
			records_[i]=records_[i-1];
			records_[i-1]=tmp;
		}

	}
	else{
		records_[idx]->data_=value;
	}

}

template <class TYPE>
bool SimpleTable<TYPE>::remove(const std::string& key){
	int idx=-1;
	int size=numRecords();
	for(int i=0;i<size;i++){
		if(records_[i]->key_ == key){
			idx=i;
		}
	}
	if(idx!=-1){

		delete records_[idx];
		for(int i=idx;i<numRecords()-1;i++){
			records_[i]=records_[i+1];
		}
		records_[size-1]=nullptr;
		return true;
	}
	else{
		return false;
	}
}

template <class TYPE>
bool SimpleTable<TYPE>::find(const std::string& key, TYPE& value){
	int idx=-1;
	for(int i=0;i<numRecords();i++){
		if(records_[i]->key_ == key){
			idx=i;
		}
	}
	if(idx==-1)
		return false;
	else{
		value=records_[idx]->data_;
		return true;
	}
}

template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(const SimpleTable<TYPE>& rhs){
	if(this!=&rhs){
		if(records_){
			while(numRecords() != 0){
				remove(records_[0]->key_);
			}
			delete [] records_;
		}
		records_=new Record*[rhs.maxExpected_+1];
		maxExpected_=rhs.maxExpected_;
        for(int i=0;i<maxExpected_;i++){
            records_[i]=nullptr;
        }
		for(int i=0;i<rhs.numRecords();i++){
			update(rhs.records_[i]->key_,rhs.records_[i]->data_);
		}

	}
	return *this;
}
template <class TYPE>
const SimpleTable<TYPE>& SimpleTable<TYPE>::operator=(SimpleTable<TYPE>&& rhs){
	if(records_){
		while(numRecords() != 0){
			remove(records_[0]->key_);
		}
		delete [] records_;
	}
	records_=rhs.records_;
	maxExpected_=rhs.maxExpected_;
	rhs.records_=nullptr;
	rhs.maxExpected_=0;

	return *this;
}
template <class TYPE>
SimpleTable<TYPE>::~SimpleTable(){
	if(records_){
		int sz=numRecords();
		for(int i=0;i<sz;i++){
			remove(records_[0]->key_);
		}
		delete [] records_;
	}
}


template <class TYPE>
class ChainingTable:public Table<TYPE>, public SortedList<TYPE>{
	SortedList<TYPE>** records_;
	int maxExpected_;
	double maxLoadFactor_;
	int numOfRecord;

	void grow() {
		std::hash<std::string> hashFunction;
		size_t hashIdx = 0;
		int preCapacity = maxExpected_;
		maxExpected_ *= 2;
		SortedList<TYPE>** tmp = new SortedList<TYPE> *[maxExpected_ + 1];


		for (int i = 0; i < maxExpected_ + 1; i++) {
			tmp[i] = nullptr;
		}

		for (int i = 0; i < preCapacity; i++) {
			if (records_[i]) {
				for (typename SortedList<TYPE>::iterator temp = records_[i]->begin(); temp != records_[i]->end(); ++temp) {
					hashIdx = hashFunction(temp.getKey()) % maxExpected_;

					if (tmp[hashIdx] == nullptr)
						tmp[hashIdx] = new SortedList<TYPE>();
					tmp[hashIdx]->insert(temp.getKey(), *temp);
				}
			}
		}

		for (int i = 0; i < preCapacity; i++) {
			if (records_[i] != nullptr) {
				while (!records_[i]->empty()) {
					records_[i]->erase(records_[i]->begin());
				}
				delete records_[i];
			}
		}
		delete[] records_;
		records_ = tmp;
	}
public:
	// constructor
	ChainingTable(int maxExpected, double maxLoadFactor);
	// copy constructor
	ChainingTable(const ChainingTable& other);
	// move constructor
	ChainingTable(ChainingTable&& other);
	// function to be added a record
	// if record with matching key, erase and insert key and value pair
	// if record does not match the key, check numOfRecord. If full, double its slot. If not insert key and value pair
	virtual void update(const std::string& key, const TYPE& value);
	// retrun true when record has matching key and removed
	// return false when record not has matching key
	virtual bool remove(const std::string& key);
	// return false when rocrod has no matching key
	// return true when record has matching key
	virtual bool find(const std::string& key, TYPE& value);
	// copy assigment operator
	virtual const ChainingTable& operator=(const ChainingTable& other);
	// move assignment operator
	virtual const ChainingTable& operator=(ChainingTable&& other);
	// Desturctor for preventing memory leak
	virtual ~ChainingTable();
	//check table is empty or not
	virtual bool isEmpty() const;
	// return numOfRecord
	virtual int numRecords() const;
	// return maxExpected_
   	virtual int capacity() const;

};

// ChainingTable Constructor
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(int maxExpected, double maxLoadFactor)
	: Table<TYPE>(), records_{ nullptr }, maxExpected_{ 0 }, maxLoadFactor_{ 0.0 }, numOfRecord{ 0 }{
	// create table with capacity
	records_ = new SortedList<TYPE> *[maxExpected + 1];
	maxExpected_ = maxExpected;
	maxLoadFactor_ = maxLoadFactor;
	
	//set table empty
	for (int i = 0; i < maxExpected_ + 1; i++) {
		records_[i] = nullptr;
	}
}

// Copy Constructor
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(const ChainingTable<TYPE>& other)
	: records_{ nullptr }, maxExpected_{ 0 }, maxLoadFactor_{ 0 }, numOfRecord{ 0 }{
	// creates table with capacity
	records_ = new SortedList<TYPE> *[other.maxExpected_ + 1];
	maxExpected_ = other.maxExpected_;
	maxLoadFactor_ = other.maxLoadFactor_;
	
	// Assign a nullptr to all elements in the array
	for (int i = 0; i < maxExpected_ + 1; i++) {
		records_[i] = nullptr;
	}
	
	// Then, using the iterator, update its key and value from other.records_
	for (int i = 0; i < other.maxExpected_; i++) {
		if (other.records_[i] != nullptr) {
			for (typename SortedList<TYPE>::iterator temp = other.records_[i]->begin(); temp != other.records_[i]->end(); ++temp) {
				update(temp.getKey(), *temp);
			}
		}
	}
}

// Move Constructor
template <class TYPE>
ChainingTable<TYPE>::ChainingTable(ChainingTable<TYPE>&& other){
	// set current table to point at other table
	records_ = other.records_;
	// move over rest of data
	maxExpected_ = other.maxExpected_;
	maxLoadFactor_ = other.maxLoadFactor_;
	numOfRecord = other.numOfRecord;
	
	// disconnect the pointer from other table
	other.records_ = nullptr;
	//reset the rest of the data
	other.maxExpected_ = 0;
	other.maxLoadFactor_ = 0;
	other.numOfRecord = 0;
}

// Update if matching key is found, else add the new key-value pair
template <class TYPE>
void ChainingTable<TYPE>::update(const std::string& key, const TYPE& value){
	std::hash<std::string> hashFunction;
	size_t hash = hashFunction(key);
	size_t idx = hash % maxExpected_;
	
	if (records_) {
		// If matching key is found, erase the orignal data, and update it as new pair
		if (records_[idx] != nullptr && records_[idx]->search(key) != records_[idx]->cend()) {
			records_[idx]->erase(records_[idx]->begin());
			records_[idx]->insert(key,value);
		}
		else{	
			// If load factor goes above maxLoadFactor, call grow()
			if ((double)numRecords() / maxExpected_ == maxLoadFactor_) {
				// Make size of table double
				grow();
				// change hash index
				idx = hash % maxExpected_;
			}
			// Insert a new key-value pair in the table
			if (records_[idx] == nullptr)
				records_[idx] = new SortedList<TYPE>;
			records_[idx]->insert(key,value);
			numOfRecord++;
		}
	}

}

// Removes the key and value from the table if matching key is found
template <class TYPE>
bool ChainingTable<TYPE>::remove(const std::string& key){
	std::hash<std::string> hashFunction;
	size_t hash = hashFunction(key);
	size_t idx = hash % maxExpected_;
	bool isRemoved = false;
	
	// Using the index we found, erase the key and value
	if (records_[idx]->search(key) != records_[idx]->cend()) {
			// delete the record with matching key
			records_[idx]->erase(records_[idx]->begin()); 
			isRemoved = true;
			numOfRecord--;
	}

	return isRemoved;
}

// If matching key is found from the record, return true and its value
template <class TYPE>
bool ChainingTable<TYPE>::find(const std::string& key, TYPE& value){
	std::hash<std::string> hashFunction;
	size_t hash = hashFunction(key);
	size_t idx = hash % maxExpected_;
	bool isFound = false;
	
	if (records_) {
	// check if any record matches the key
		if (records_[idx]->search(key) != records_[idx]->cend()) {
			isFound = true;
			// pass back the value 
			value = *records_[idx]->cbegin();
		}
	}
	return isFound;
}

// Copy assignment operator
template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(const ChainingTable<TYPE>& other){
	if (this != &other) {
		// Remove all existing data in the original record
		if (records_) {
			for (int i = 0; i < maxExpected_; i++) {
				if (records_[i] != nullptr) {
					while (!records_[i]->empty()) {
						records_[i]->erase(records_[i]->begin());
					}
					delete records_[i];
				}
			}
			//delete the table
			delete[] records_;
		}
		
		// Then, copy all the data from other record
		records_ = new SortedList<TYPE> *[other.maxExpected_ + 1];
		// copy other data 
		maxExpected_ = other.maxExpected_;
		maxLoadFactor_ = other.maxLoadFactor_;
		// reset number of record in current table 
		numOfRecord = 0;
		//set table empty 
		for (int i = 0; i < maxExpected_ + 1; i++) {
			records_[i] = nullptr;
		}
		// copy over the data in other table
		for (int i = 0; i < other.maxExpected_; i++) {
			if (other.records_[i] != nullptr) {
				for (typename SortedList<TYPE>::iterator temp = other.records_[i]->begin(); temp != other.records_[i]->end(); ++temp) {
					update(temp.getKey(), *temp);
				}
			}
		}
	}
	return *this;

}

// Move assignemtn operator
template <class TYPE>
const ChainingTable<TYPE>& ChainingTable<TYPE>::operator=(ChainingTable<TYPE>&& other){
	// Delete all data from original record
	if (records_) {
		for (int i = 0; i < maxExpected_; i++) {
			if (records_[i] != nullptr) {
				while (!records_[i]->empty()) {
					records_[i]->erase(records_[i]->begin());
				}
				delete records_[i];
			}
		}
		//delete the table
		delete[] records_;
	}
	
	// Then, assign (move) all the data from other record
	records_ = other.records_;
	// move over the data
	maxExpected_ = other.maxExpected_;
	maxLoadFactor_ = other.maxLoadFactor_;
	numOfRecord = other.numOfRecord;
	
	// disconnect the pointer from the table
	other.records_ = nullptr;
	// reset the rest of the data
	other.maxExpected_ = 0;
	other.maxLoadFactor_ = 0;
	other.numOfRecord = 0;

	return *this;
}

// Destructor
template <class TYPE>
ChainingTable<TYPE>::~ChainingTable(){
	if (records_) {
		// Remove all data that are linked
		for (int i = 0; i < maxExpected_; i++) {
			if (records_[i] != nullptr) {
				while (!records_[i]->empty()) {
					records_[i]->erase(records_[i]->begin());
				}
				delete records_[i];
			}
		}
		delete[] records_;
	}
}

// Returns true if the table is empty
template <class TYPE>
bool ChainingTable<TYPE>::isEmpty() const {
	bool isEmpty = false;
	if (numRecords() == 0) 
		isEmpty = true;
    return isEmpty;
}

// Returns numOfRecord
template <class TYPE>
int ChainingTable<TYPE>::numRecords() const {
    return numOfRecord;
}

// Returns maxExpected
template <class TYPE>
int ChainingTable<TYPE>::capacity() const {
    return maxExpected_;
}

