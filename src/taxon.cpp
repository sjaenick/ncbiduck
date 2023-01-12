
#include "taxon.hpp"

#include <iostream>

using namespace std;

inline const string Taxon::getName() const { return name; } 
inline const string Taxon::getRank() const { return rank; } 

std::ostream& operator<<(std::ostream &os, const Taxon &t) {
    os << t.getName();
    return os;
}

