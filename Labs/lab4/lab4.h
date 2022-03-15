#include <iostream>

template <typename T>
class DList{
	struct Node{
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data=T{},Node* next=nullptr, Node* prev=nullptr){
			data_=data;
			next_=next;
			prev_=prev;
		}
	};
	Node* front_;
	Node* back_;
public:
	DList(){
		front_=nullptr;
		back_=nullptr;
	}
	void push_front(const T& data);
	~DList();

	class const_iterator{
		friend class Dlist;
		const DList* theList_;
	protected:
		Node* curr_;
	public:
		const_iterator(Node* n, const DList* list){
			curr_=n;
			theList_ = list;
		}

		const_iterator(){
			curr_=nullptr;
			theList_ = nullptr;
		}
		const_iterator operator++(){
			
			curr_ = curr_->next_;
			
			return *this;
		}
		const_iterator operator++(int){
			const_iterator prefix = *this;
			if (curr_) {
				curr_ = curr_->next_;
			}
			return prefix;
		}
		const_iterator operator--(){
			if (curr_) {
				curr_ = curr_->prev_;
			}else{
				curr_ = theList_->back_;
			}
			return *this;
		}
		const_iterator operator--(int){
			const_iterator prefix = *this;
			if (curr_) {
				curr_ = curr_->prev_;
			}else{
				curr_ = theList_->back_;
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
		friend class Dlist;
	public:
		iterator(Node* n, DList* list) : const_iterator(n, list){};
		iterator():const_iterator(){};
		iterator operator++(){
			if (this->curr_) {
				this->curr_ = this->curr_->next_;
			}
			return *this;
		}
		iterator operator++(int){
			iterator prefix = *this;
			if (this->curr_) {
				this->curr_ = this->curr_->next_;
			}
			return prefix;
		}
		iterator operator--(){
			if (this->curr_) {
				this->curr_ = this->curr_->prev_;
			}
			return *this;
		}
		iterator operator--(int){
			iterator prefix = *this;
			if (this->curr_) {
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
	const_iterator cbegin() const{
		return const_iterator(front_, this);
	}
	iterator begin(){
		return iterator(front_, this);
	}
	const_iterator cend() const{
		
		return const_iterator(nullptr, this);		
	}
	iterator end(){
		return iterator(nullptr, this);
	}
};


template <typename T>
void DList<T>::push_front(const T& data){
	Node* nn = new Node(data, front_);
	if(front_ != nullptr){
		front_->prev_ = nn;
	}else{
		back_ = nn;
	}
	front_ = nn;
}


template <typename T>
DList<T>::~DList(){
	Node* curr = front_;
	while(curr){
		Node* rm = curr;
		curr = curr->next_;
		delete rm;
	}
}
template <typename T>
class Sentinel{
	struct Node{
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data=T{},Node* next=nullptr, Node* prev=nullptr){
			data_=data;
			next_=next;
			prev_=prev;
		}
	};
	Node* front_;
	Node* back_;
public:
	Sentinel(){
		front_=new Node();
		back_=new Node();
		front_->next_=back_;
		back_->prev_=front_;
	}
	void push_front(const T& data);
	~Sentinel();

	class const_iterator{
	protected:
		Node* curr_;
	public:
		const_iterator(Node* n){
			curr_ = n;
		}
		const_iterator() {
			curr_ = nullptr;
		}
		const_iterator operator++() {
			if (curr_) {
				curr_ = curr_->next_;
			}
			return *this;
		}
		const_iterator operator++(int) {
			const_iterator prefix = *this;
			if (curr_) {
				curr_ = curr_->next_;
			}
			return prefix;
		}
		const_iterator operator--() {
			if (curr_) {
				curr_ = curr_->prev_;
			}
			return *this;
		}
		const_iterator operator--(int) {
			const_iterator prefix = *this;
			if (curr_) {
				curr_ = curr_->prev_;
			}
			return prefix;
		}
		bool operator==(const_iterator rhs) {
			return curr_ == rhs.curr_;
		}
		bool operator!=(const_iterator rhs) {
			return curr_ != rhs.curr_;
		}
		const T& operator*()const {
			return curr_->data_;
		}
	};
	class iterator:public const_iterator{
	public:
		iterator(Node* n):const_iterator(n){};
		iterator() :const_iterator() {};
		iterator operator++() {
			if (this->curr_) {
				this->curr_ = this->curr_->next_;
			}
			return *this;
		}
		iterator operator++(int) {
			iterator prefix = *this;
			if (this->curr_) {
				this->curr_ = this->curr_->next_;
			}
			return prefix;
		}
		iterator operator--() {
			if (this->curr_) {
				this->curr_ = this->curr_->prev_;
			}
			return *this;
		}
		iterator operator--(int) {
			iterator prefix = *this;
			if (this->curr_) {
				this->curr_ = this->curr_->prev_;
			}
			return prefix;
		}
		T& operator*() {
			return this->curr_->data_;
		}
		const T& operator*()const {
			return this->curr_->data_;
		}
	};
	const_iterator cbegin() const{
		return const_iterator(front_->next_);
	}
	iterator begin(){
		return iterator(front_->next_);
	}
	const_iterator cend() const{
		return const_iterator(back_);
	}
	iterator end(){
		return iterator(back_);
	}
};


template <typename T>
void Sentinel<T>::push_front(const T& data){
	Node* nn = new Node(data, front_->next_, front_);
	front_->next_->prev_ = nn;
	front_->next_ = nn;
}


template <typename T>
Sentinel<T>::~Sentinel(){
	Node* curr = front_;
	while(curr){
		Node* rm = curr;
		curr = curr->next_;
		delete rm;
	}
}
