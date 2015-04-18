#include <iostream>
#include <string.h>
#include <sstream>
#include <algorithm>
#include <iterator>

int main() {
    using namespace std;
    string sentence = "Love is always bad";
    char *str;  
    memcpy(str, sentence.c_str(), sentence.length());
    char *last;
    char *pos;
    while(pos = strsep(&str, " ")) {
        std::cout<<pos<<std::endl;
    }
}


