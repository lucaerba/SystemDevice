#include <vector>
#include <iostream>
#include "Stack.h"

using std::vector;
using namespace std;


int main(int argc, char const *argv[])
{
    Stack s;

    cout << "Is empty? " << s.empty() << endl;
    for(int i=0; i<10; i++)
        s.push(i);

    s.visit();
    cout << "Is empty? " << s.empty() << endl;
    cout << "Extracted: " << s.pop() << endl;
    cout << "Extracted: " << s.pop() << endl;
    s.visit();
    cout << "Size: " <<s.getSize() << endl;
    return 0;
}
