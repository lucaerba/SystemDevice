//
// Created by lucaerba on 3/27/23.
//

#ifndef LAB2_STACK_H
#define LAB2_STACK_H
#include <vector>
using std::vector;

class Stack{
private:
    vector<int> myStack;
public:
    Stack();
    void push(int val);
    int pop();
    void visit();
    int getSize();
    bool empty();
};
#endif //LAB2_STACK_H

