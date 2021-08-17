// Testing inline assembly in C++

#include <iostream>

using namespace std;


int add(int a, int b) {
    return a + b;
}

// http://www.codeproject.com/Articles/15971/Using-Inline-Assembly-in-C-C
// +---+--------------------+
// | r |    Register(s)     |
// +---+--------------------+
// | a |   %eax, %ax, %al   |
// | b |   %ebx, %bx, %bl   |
// | c |   %ecx, %cx, %cl   |
// | d |   %edx, %dx, %dl   |
// | S |   %esi, %si        |
// | D |   %edi, %di        |
// +---+--------------------+
int add_asm(int a, int b) {
    
    __asm__("addl %%ebx, %%eax"  // eax = ebx + abx
            :"=a" (a)            // output values:
                                 //   after run, eax is moved to variable a
            :"a"(a), "b" (b) );  // input values:
                                 //   a -> eax, b -> ebx

    // a contains the sum of a and b
    return a;  
}


int main()
{
    cout << add(1, 1) << endl;
    cout << add_asm(1, 1) << endl;
    return 0;
}
