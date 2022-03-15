#include <iostream>
#include <string>
#include "stack.h"

bool bracketCheck(const std::string& s){
    Stack<char>* st = new Stack<char>();
	// To check if all pairs of brackets match
    int pairs = 0;
    for(int i = 0; i < s.length(); i++){
		// Opening bracket check
        if(s[i] == '(' || s[i] == '{' || s[i] == '['){
            st->push(s[i]);
            pairs++;
		// Closing bracket check
        }else if(s[i] == ')' || s[i] == '}' || s[i] == ']'){
			// Needs to match with opening bracket in stack that was pushed before
            if((st->topStack() == '(' && s[i] == ')') || (st->topStack() == '{' && s[i] == '}') || (st->topStack() == '[' && s[i] == ']')){
                st->pop();
                pairs--;
            }else{
                return false;
            }
		}   
    }
	// Check if pairs match
    if(pairs == 0){
        return true;
    }else{
        return false;
    }
}
