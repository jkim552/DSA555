template <typename T>
class Stack{
    T* stackArr;
    int max;
    int top;
public:
	// Public constructor
    Stack(){
        max = 100;
        stackArr = new T[max];
        top = 0;
    }
	
	// Destructor
    ~Stack(){
        delete [] stackArr;
    }
	
	// push (insert) the data from the beginning
    void push(const T& data){
        if(top != max){
            stackArr[top] = data;
            top++;
        }
    }
    
	// pop (delete) the data from the end
    void pop(){
        if (top > 0){
            top--;
        }
    }

	// Returns the last element of an array
    T topStack() const{
        return stackArr[top - 1];
    }
};
