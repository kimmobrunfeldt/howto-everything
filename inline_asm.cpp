// Testing inline assembly in C++ part 2

#include <iostream>

using namespace std;

// Check if string contains given character
bool find(const char* s, char c) {

    while (*s != '\0') {
        if (*s == c) {
            return true;
        }
        ++s;
    }
    return false;
}

// Check if string contains given character
bool find_asm(const char* s, char c) {

    bool found = false;
    __asm__("find_loop:          \n"
            "movb   (%%eax), %%cl\n"  // Load character to %cl
            "addl   $1, %%eax    \n"  // Move to next character
            "cmpb   $0, %%cl     \n"  // Check if character == '\0'
            "je     finish       \n"  // If it is, go to finish
            "cmpb   %%bl, %%cl   \n"  // Did we find the character?
            "jne    find_loop    \n"  // If not, go to next character
            "movb   $1, %%cl     \n"  // We found the character, %cl = 1
            "finish:             \n"

            :"=c"(found)      // %cl == 1 if found
            :"a"(s), "b" (c)  // address of string -> %eax, char to find -> %bl
            );

    return found;
}


int main()
{
    const char* s = "Test string.";

    // Check all characters in test string.
    const char* i = s;
    while (*i != '\0') {
        cout << find(s, *i) << " == ";
        cout << find_asm(s, *i) << endl;
        ++i;
    }

    return 0;
}
