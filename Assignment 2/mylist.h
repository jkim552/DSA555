/*******************************************************/
/*                                                     */
/*  Put your linked list code from A1/lab3/lab4 into   */
/*  this file                                          */
/*                                                     */
/*******************************************************/

#include <iostream>
#include <string>
template <typename T>

class SortedList {
	struct Node {
		std::string key_;
		T data_;
		Node* next_;
		Node* prev_;
		Node(std::string key = "", const T& data = T{}, Node* next = nullptr, Node* prev = nullptr) {
			key_ = key;
			data_ = data;
			next_ = next;
			prev_ = prev;
		}
	};
	Node* front_;
	Node* back_;

public:
	class const_iterator {
		friend class SortedList;
		Node* curr_;
		const_iterator(Node* n) {
			curr_ = n;
		}
	public:
		//constructor, create empty iteraotr.
		const_iterator() {
			curr_ = nullptr;
		}
		//prefix ++
		const_iterator& operator++() {
			curr_ = curr_->next_;
			return *this;
		}
		//postfix ++
		const_iterator operator++(int) {
			const_iterator temp = *this;
			curr_ = curr_->next_;
			return temp;
		}
		//prefix --
		const_iterator& operator--() {
			curr_ = curr_->prev_;
			return *this;
		}
		//postfix--
		const_iterator operator--(int) {
			const_iterator temp = *this;
			curr_ = curr_->prev_;
			return temp;
		}
		//function returns true when rhs current object is same as current object
		bool operator==(const_iterator rhs) {
			return (curr_ == rhs.curr_);
		}
		//function returns true when rhs current object is not same as current object
		bool operator!=(const_iterator rhs) {
			return !(*this == rhs);
		}
		//returns a const refrence to data in the node reffered to by the iterator
		const T& operator*()const {
			return curr_->data_;
		}

		const std::string& getKey()const {
			return curr_->key_;
		}
	};

	class iterator :public const_iterator {
		friend class SortedList;
		iterator(Node* n) { const_iterator::curr_ = n; }
	public:
		//constructor
		iterator() {}
		//prefix+++
		iterator& operator++() {
			const_iterator::curr_ = const_iterator::curr_->next_;
			return *this;
		}
		//postfix++
		iterator operator++(int) {
			iterator temp = *this;
			const_iterator::curr_ = const_iterator::curr_->next_;
			return temp;
		}
		//prefix==
		iterator& operator--() {
			const_iterator::curr_ = const_iterator::curr_->prev_;
			return *this;
		}
		//postfix --
		iterator operator--(int) {
			iterator temp = *this;
			const_iterator::curr_ = const_iterator::curr_->prev_;
			return temp;
		}
		//returns a const reference to data in the node referred to by the iterator.
		T& operator*() {
			return const_iterator::curr_->data_;
		}
		//returns a data from the node 
		const T& operator*()const {
			return const_iterator::curr_->data_;
		}

	};
	//constructor, create empty SortedList
	SortedList();

	//destructor
	~SortedList();

	//copy-constructor
	SortedList(const SortedList& rhs);

	//assignment operator
	SortedList& operator=(const SortedList& rhs);

	//move constructor
	SortedList(SortedList&& rhs);

	//move operator
	SortedList& operator=(SortedList&& rhs);

	//operator overloading for comparing data
	bool operator<(const T& data);

	//operator overloading for comparing data
	bool operator==(const T& data);

	//returns the appropriate iterator to the first node
	iterator begin() {
		return iterator(front_);
	}

	//return the appropriate iterator to the node after last node
	iterator end() {
		const T data{};
		//check node exists 
		if (back_ == nullptr) {
			return iterator(nullptr);
		}
		//create empty node after back_
		if (back_->next_ == nullptr) {
			Node* temp = new Node("", data, nullptr, back_);
			back_->next_ = temp;
		}
		return iterator(back_->next_);
	}

	//returns the appropriate iterator to the first node
	const_iterator cbegin() const {
		return const_iterator(front_);
	}

	//return the appropriate iterator to the node after last node
	const_iterator cend() const {
		const T data{};
		if (back_ == nullptr) {
			return const_iterator(nullptr);
		}
		if (back_->next_ == nullptr) {
			Node* temp = new Node("", data, nullptr, back_);
			back_->next_ = temp;
		}
		return const_iterator(back_->next_);
	}
	//creates new node and insert node into the linked list by comparing data 
	void insert(const std::string key,const T& data);

	//The function finds and returns an iterator to node containing a matching key
	iterator search(const std::string& data);

	//The function finds and returns an iterator to node containing a matching key
	const_iterator search(const std::string& data) const;

	//removes a node matches iterator it returns node that follows the removed node
	iterator erase(iterator it);

	//merge two linked list into one linked list
	void merge(SortedList& other);

	//returns true if the list is empty
	bool empty() const;

	//returns size of the list
	int size() const;
};

//constructor
template <typename T>
SortedList<T>::SortedList() {
	front_ = nullptr;
	back_ = nullptr;
}
//destructor
template <typename T>
SortedList<T>::~SortedList() {
	Node* curr = front_;
	while (curr != nullptr) {
		Node* next = curr->next_;
		delete curr;
		curr = next;
	}
	front_ = nullptr;
	back_ = nullptr;
}

//copy-constructor
template <typename T>
SortedList<T>::SortedList(const SortedList& rhs) :front_{ nullptr }, back_{ nullptr } {
	SortedList<T>::const_iterator it;
	//isert from last node to first nodes
	for (it = rhs.cend(); it != rhs.cbegin();) {
		insert(*(--it));
	}
}

//assignment operator
template <typename T>
SortedList<T>& SortedList<T>::operator=(const SortedList& rhs) {
	SortedList<T>::const_iterator it;
	Node* curr = front_;

	//clean up everything before copy the data
	while (curr != nullptr) {
		Node* next = curr->next_;
		delete curr;
		curr = next;
	}
	front_ = nullptr;
	back_ = nullptr;
	//insert from last node to first node
	for (it = rhs.cend(); it != rhs.cbegin();) {
		insert(*(--it));
	}
	return *this;
}

//move constructor
template <typename T>
SortedList<T>::SortedList(SortedList&& rhs) {
	front_ = rhs.front_;
	back_ = rhs.back_;
	//clean up 
	rhs.front_ = nullptr;
	rhs.back_ = nullptr;
}

//move operator
template <typename T>
SortedList<T>& SortedList<T>::operator=(SortedList&& rhs) {
	Node* curr = front_;
	//clean up
	while (curr != nullptr) {
		Node* next = curr->next_;
		delete curr;
		curr = next;
	}
	front_ = rhs.front_;
	back_ = rhs.back_;
	rhs.front_ = nullptr;
	rhs.back_ = nullptr;
	return *this;
}

//< operator overloading
template <typename T>
bool SortedList<T>::operator<(const T& data) {
	return *this < data;
}

//== operator overloading
template <typename T>
bool SortedList<T>::operator==(const T& data) {
	return *this == data;
}


template <typename T>
void SortedList<T>::insert(const std::string key, const T& data) {
	Node* nn = new Node(key, data, front_);
	if (front_ != nullptr) {
		front_->prev_ = nn;
	}
	else {
		back_ = nn;
	}
	front_ = nn;
}

// The function finds and returns an iterator to the node containing a matching key
template <typename T>
typename SortedList<T>::iterator SortedList<T>::search(const std::string& key) {
	SortedList<T>::iterator search_it;
	for (search_it = begin(); search_it != end(); ++search_it) {
		if (search_it.getKey() == key) {
			//if matching key is at front_ no need to adjust the order
			if (search_it.curr_ != front_) {
				//found key in the middle of the list
				if (search_it.curr_ != back_) {
					search_it.curr_->prev_->next_ = search_it.curr_->next_;
					search_it.curr_->next_->prev_ = search_it.curr_->prev_;
				}
				//found key at the back_ of the list
				else {
					back_ = search_it.curr_->prev_;
					search_it.curr_->prev_->next_ = nullptr;
				}
				//set found key to be at the front_ 
				search_it.curr_->next_ = front_;
				search_it.curr_->next_->prev_ = search_it.curr_;
				front_ = search_it.curr_;
				search_it.curr_->prev_ = nullptr;
			}
			return search_it;
		}
	}
	//not found; return end()
	return search_it;
}

template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::search(const std::string& key) const {
	SortedList<T>::iterator search_it;
	for (search_it = begin(); search_it != end(); ++search_it) {
		if (search_it.getKey() == key) {
			//if matching key is at front_ no need to adjust the order
			if (search_it.curr_ != front_) {
				//found key in the middle of the list
				if (search_it.curr_ != back_) {
					search_it.curr_->prev_->next_ = search_it.curr_->next_;
					search_it.curr_->next_->prev_ = search_it.curr_->prev_;
				}
				//found key at the back_ of the list
				else {
					back_ = search_it.curr_->prev_;
					search_it.curr_->prev_->next_ = nullptr;
				}
				//set found key to be at the front_ 
				search_it.curr_->next_ = front_;
				search_it.curr_->next_->prev_ = search_it.curr_;
				front_ = search_it.curr_;
				search_it.curr_->prev_ = nullptr;
			}
			return search_it;
		}
	}
	//not found; return end()
	return search_it;
}

//removes a node matches iterator it returns node that follows the removed node
template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator it) {
	SortedList<T>::iterator temp;
	//used to return the next node.
	temp = it;

	//only node inside of the list 
	if (it.curr_ == front_ && it.curr_ == back_) {
		front_ = nullptr;
		back_ = nullptr;
		delete it.curr_;
		return end();
	}
	else if (it.curr_ == front_) {
		++temp;
		front_ = it.curr_->next_;
		front_->prev_ = nullptr;
		delete it.curr_;
	}
	else if (it.curr_ == back_) {
		back_ = it.curr_->prev_;
		back_->next_ = it.curr_->next_;
		delete it.curr_;
		//no next node, return end()
		return end();
	}
	else {
		++temp;
		it.curr_->prev_->next_ = it.curr_->next_;
		it.curr_->next_->prev_ = it.curr_->prev_;
		delete it.curr_;
	}

	return temp;
}

template <typename T>
void SortedList<T>::merge(SortedList& other) {
	SortedList<T>::iterator curr_it;
	SortedList<T>::iterator other_it = other.begin();
	bool isFound = false;

	if (this->size() == 0) {
		other.front_ = other.front_->next_;
		this->front_ = other_it.curr_;
		this->back_ = other_it.curr_;
	}

	other_it = other.begin();
	while (other_it != other.end()) {
		curr_it = this->end();
		--curr_it;
		// check if biggest 
		if (*curr_it < *other_it || *curr_it == *other_it) {
			if (other.front_ == other.back_) {
				other.front_ = nullptr;
				other.back_ = nullptr;
			}
			else {
				other.front_ = other.front_->next_;
			}
			other_it.curr_->prev_ = curr_it.curr_;
			curr_it.curr_->next_ = other_it.curr_;
			this->back_ = other_it.curr_;
			other_it = other.begin();
		}
		else {
			isFound = false;
			for (curr_it = begin(); curr_it != end() && isFound != true; ++curr_it) {
				if (*other_it < *curr_it) {
					// set front to next node in other list
					if (other.front_ == other.back_) {
						other.front_ = nullptr;
						other.back_ = nullptr;
					}
					else {
						other.front_ = other.front_->next_;
					}
					isFound = true;
					if (curr_it.curr_ == this->front_) {
						other_it.curr_->next_ = curr_it.curr_;
						other_it.curr_->prev_ = nullptr;
						curr_it.curr_->prev_ = other_it.curr_;
						this->front_ = other_it.curr_;
					}
					else {
						other_it.curr_->next_ = curr_it.curr_;
						other_it.curr_->prev_ = curr_it.curr_->prev_;
						curr_it.curr_->prev_->next_ = other_it.curr_;
						curr_it.curr_->prev_ = other_it.curr_;
					}
					other_it = other.begin();
				}
			}
		}
	}
}

// return true if list is empty
template <typename T>
bool SortedList<T>::empty() const {
	return (front_ == nullptr && back_ == nullptr);
}

//return the size of the list
template <typename T>
int SortedList<T>::size() const {
	SortedList<T>::const_iterator it;
	int count = 0;

	for (it = cbegin(); it != cend(); ++it) {
		count++;
	}

	return count;
}
