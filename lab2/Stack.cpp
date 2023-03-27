//
// Created by lucaerba on 3/27/23.
//

#include "Stack.h"
#include <iostream>
using std::cout;
using std::endl;

Stack::Stack() {

}
void Stack::push(int val){
    myStack.push_back(val);
}
int Stack::pop(){
    int ret = myStack.back();
    myStack.pop_back();
    return ret;
}
void Stack::visit(){
    for(auto x: myStack)
        cout << x << " ";
    cout << endl;
}
int Stack::getSize(){
    return myStack.size();
}
bool Stack::empty(){
    return getSize()==0;
}