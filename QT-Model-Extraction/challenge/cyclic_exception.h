#include <iostream>
using namespace std;

class CustomCyclicException : public std::exception
{
    public:
		const char * what () const throw () { return "Cyclic SIG/SLOT behavior detected\n"; }

};

