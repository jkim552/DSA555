/*******************************************************************/
/*                                                                 */
/*  A1 Part 2: SortedList starter file                             */
/*                                                                 */
/*  version 1.1: fixed class declaration to include merge          */
/*                                                                 */
/*  Author1 Name: <name here>                                      */
/*      - class/function list                                      */
/*  Author2 Name: <name here>                                      */
/*      - class/function list                                      */
/*                                                                 */
/*******************************************************************/

#include <iostream>
template <typename T>

class SortedList{
	struct Node{
        Node* next_;
        Node* prev_;
        T data_;
		Node(const T& data=T{},Node* nx=nullptr,Node* pr=nullptr){
            data_ = data;
            next_ = nx;
            prev_ = pr;
		}
	};
    Node* front_;
    Node* back_;

public:
	class const_iterator{
        Node* curr_;
	const SortedList* theList_;
	public:
		friend class SortedList;
		const_iterator(){
            curr_ = nullptr;
	    theList_ = nullptr;
        }
		const_iterator(Node* n, const SortedList* list){
	    curr_ = n;
	    theList_ = list;
	}
		const_iterator(Node* n){curr_ = n;}
		const_iterator& operator++(){    // postfix ++
            if(curr_){
                curr_ = curr_->next_;
            }
            return *this;
        }
		const_iterator operator++(int){  // prefix ++
            const_iterator prefix = *this;
            if (curr_){
                curr_ = curr_->next_;
            }
            return prefix;
        }
		const_iterator& operator--(){
            if(curr_){
                curr_ = curr_->prev_;
            }else{
		curr_ = theList_->back_;
	    }
            return *this;
        }
		const_iterator operator--(int){
            const_iterator prefix = *this;
            if(curr_){
                curr_ = curr_->prev_;
            }
            return prefix;
        }
		bool operator==(const_iterator rhs){
            return curr_ == rhs.curr_;
        }
		bool operator!=(const_iterator rhs){
            	return curr_ != rhs.curr_;
        }
		const T& operator*()const{
            return curr_->data_;
        }
	};
	class iterator:public const_iterator{
	public:
		friend class SortedList;
		iterator():const_iterator(){}
		iterator(Node* n, SortedList* list):const_iterator(n, list){}
		iterator(Node* n):const_iterator(n){}
	
		iterator& operator++(){
            if(this->curr_){
                this->curr_ = this->curr_->next_;
            }
            return *this;
        }
		iterator operator++(int){
            iterator prefix = *this;
            if(this->curr_){
                this->curr_ = this->curr_->next_;
            }
            return prefix;
        }
		iterator& operator--(){
            if(this->curr_){
                this->curr_ = this->curr_->prev_;
            }
            return *this;
        }
		iterator operator--(int){
            iterator prefix = *this;
            if(this->curr_){
                this->curr_ = this->curr_->prev_;
            }
            return prefix;    
        }
		T& operator*(){
            return this->curr_->data_;
        }
		const T& operator*()const{
            return this->curr_->data_;
        }
	};
	SortedList();
	~SortedList();
	SortedList(const SortedList& rhs);
	SortedList& operator=(const SortedList& rhs);
	SortedList(SortedList&& rhs);
	SortedList& operator=(SortedList&& rhs);
	iterator begin(){
        return iterator(front_->next_, this);
    }
	iterator end(){
        return iterator(back_, this);
    }
	const_iterator cbegin() const{
        return const_iterator(front_->next_, this);
    }
	const_iterator cend() const{
        return const_iterator(back_, this);
    }
	iterator insert(const T& data);
	iterator search(const T& data);
	const_iterator search(const T& data) const;
	iterator erase(iterator it);
	void merge(SortedList& other);
	bool empty() const;
	int size() const;
	void print() const;
};

// Default constructor
template <typename T>
SortedList<T>::SortedList(){
    Node* front = new Node();
    Node* back = new Node();
    front->next_ = back;
    front->prev_ = nullptr;
    back->prev_ = front;
    back->next_ = nullptr;
    front_ = front;
    back_ = back;
}

// Destructor
template <typename T>
SortedList<T>::~SortedList(){
    iterator i = begin();
    while(i != end()){
	i = erase(i);        
    }
    delete front_;
    delete back_;
}

// Copy constructor
template <typename T>
SortedList<T>::SortedList(const SortedList& rhs){
    Node* front = new Node();
    Node* back = new Node();
    front->next_ = back;
    back->prev_ = front;
    front_ = front;
    back_ = back;

    for(const_iterator i = rhs.cbegin(); i != rhs.cend(); i++){
        Node* nn = new Node(*i, back_, back_->prev_);
	    back_->prev_->next_ = nn;
	    back_->prev_ = nn;
    } 
}

// Assignment operator
template <typename T>
SortedList<T>& SortedList<T>::operator=(const SortedList& rhs){
    // Erase the previous data
    iterator start = begin();
    iterator last = end();
    Node* track = start.curr_->prev_;
    
    while(start != last && start != end()){
        ++start;
        delete start.curr_->prev_;   
    }
    
    Node* nn = ( start == last ? start.curr_ : back_);
    track->next_ = nn;
    nn->prev_ = track;


    for(const_iterator i = rhs.cbegin(); i != rhs.cend(); i++){
        Node* nn = new Node(*i, back_, back_->prev_);
	    back_->prev_->next_ = nn;
	    back_->prev_ = nn;
    }
    return *this;
}
template <typename T>
SortedList<T>::SortedList(SortedList&& rhs){
    Node* front = new Node();
    Node* back = new Node();
    front->next_ = back;
    back->prev_ = front;
    front_ = front;
    back_ = back;
    
    // Since, elements in the middle are all connected
    // Move the first node's prevous node point at this->front_
    // And connect that next node to the rhs.front_->next_
    rhs.front_->next_->prev_ = front_;
    front_->next_ = rhs.front_->next_;

    // Similar concept as above
    rhs.back_->prev_->next_ = back_;
    back_->prev_ = rhs.back_->prev_;

    // Finally, de-link the rhs list as an empty list (only front and back)
    rhs.front_->next_ = rhs.back_;
    rhs.back_->prev_ = rhs.front_;
}

template <typename T>
SortedList<T>& SortedList<T>::operator=(SortedList&& rhs){
    iterator start = begin();
    iterator last = end();
    Node* track = start.curr_->prev_;
    
    while(start != last && start != end()){
        ++start;
        delete start.curr_->prev_;   
    }
    
    Node* nn = ( start == last ? start.curr_ : back_);
    track->next_ = nn;
    nn->prev_ = track;

    // Since, elements in the middle are all connected
    // Move the first node's prevous node point at this->front_
    // And connect that next node to the rhs.front_->next_
    rhs.front_->next_->prev_ = front_;
    front_->next_ = rhs.front_->next_;

    // Similar concept as above
    rhs.back_->prev_->next_ = back_;
    back_->prev_ = rhs.back_->prev_;

    // Finally, de-link the rhs list as an empty list (only front and back)
    rhs.front_->next_ = rhs.back_;
    rhs.back_->prev_ = rhs.front_;

    return *this;
}

// Adds data to the linked list and returns iterator to newly added node
template <typename T>
typename SortedList<T>::iterator SortedList<T>::insert(const T& data){
    Node* nn = new Node(data);
    // If the linked list is not empty
    if(front_->next_ != back_){
        // Using the iterator, keep track on the iterator to sort the list
        for(iterator i = begin(); i != end(); i++){
            // If the node has to be inserted between the list
            if(nn->data_ < i.curr_->data_){
                i.curr_->prev_->next_ = nn;
                nn->prev_ = i.curr_->prev_;
                nn->next_ = i.curr_;
                i.curr_->prev_ = nn;
                return nn;   // Return so that the iterator won't loop more
            }
        }
        // If the data is the largest in the list
        back_->prev_->next_ = nn;
        nn->prev_ = back_->prev_;
        nn->next_ = back_;
        back_->prev_ = nn;
    }
    // If the linked list is empty
    else{
        front_->next_ = nn;
        nn->prev_ = front_;
        back_->prev_ = nn;
        nn->next_ = back_;
    }
    return nn;
}

template <typename T>
typename SortedList<T>::iterator SortedList<T>::search(const T& data){
    // Using the iterator, search for the data
    for(iterator i = begin(); i != end(); i++){
        if(*i == data){
            return i;
        }
    }
    return end();
}

template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::search(const T& data) const{
    // Using the iterator, search for the data
    for(const_iterator i = cbegin(); i != cend(); i++){
        if(*i == data){
            return i;
        }
    }
    return cend();
}

// Removes the node referred to by it and returns iterator to node after the node removed
template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator it){
    Node* rm = it.curr_;
    Node* B = rm;    // This node is used for return
    // The erase method should not delete the front_ and back_ node
    if((rm != front_) && (rm != back_)){
        
        rm->prev_->next_ = rm->next_;
        rm->next_->prev_ = rm->prev_;
	B = rm->next_;
        delete rm;
    }
    return B;
}

// Merges to sorted lists
template <typename T>
void SortedList<T>::merge(SortedList& other){

	iterator i = begin();
	iterator i2 = other.begin();
	
	// if other list is empty, return
	if(i2  == other.end()){
		return;
	}
	
	while(i != end() && i2 != other.end()){
		if(*i2 < *i){
			// Keep track of next_ node from other list
        		iterator temp = i2;
            		++i2;
            		Node* backup = temp.curr_;
            		i.curr_->prev_->next_ = backup;
            		backup->prev_ = i.curr_->prev_;
            		backup->next_ = i.curr_;
            		i.curr_->prev_ = backup;		
		}else{
            		++i;
		}
	}
	
	// If the other list is not empty,
	if (i2 != other.end()){
		// connect the first node to the back of orignial node 
		// and last node to the front of back_ sentinel
		end().curr_->prev_->next_ = i2.curr_;
		i2.curr_->prev_ = end().curr_->prev_;
		other.end().curr_->prev_->next_ = end().curr_;
		end().curr_->prev_ = other.end().curr_->prev_;
	}

	// connect the sentinels to each other
	other.front_->next_ = other.back_;
	other.back_->prev_ = other.front_;
}

// Function returns true if the list is empty, false otherwise
template <typename T>
bool SortedList<T>::empty() const{
    return (size() == 0 ? true : false);
}

// Function returns number of pieces of data stored in the list
template <typename T>
int SortedList<T>::size() const{
    int size = 0;
    for(const_iterator i = cbegin(); i != cend(); i++){
        size++;
    }
    return size;
}

template <typename T>
void SortedList<T>::print() const
{
	int count = 0;
    for (const_iterator i = cbegin(); i != cend(); i++) {
        std::cout << *i << " -> ";
	if(count > 30){
		break;
	}
	count++;
    }
    std::cout << std::endl;
}
