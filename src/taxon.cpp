
#include "taxon.hpp"

#include <iostream>

using namespace std;

std::ostream& operator<<(std::ostream &os, const Taxon &t) {
    os << t.name;
    return os;
}

