
#ifndef taxon_hpp
#define taxon_hpp

#include <string>

using namespace std;

class Taxon {
    public:
        Taxon(int taxid, int parentid, const string &name, const string &rank)
            : taxid(taxid), parentid(parentid), name(name), rank(rank) {};
        inline int getID() const { return taxid; };
        inline int getParentID() const { return parentid; }
        inline const string getName() const { return name; }
        inline const string getRank() const { return rank; }
        ~Taxon() {};
        friend std::ostream& operator<<(std::ostream& output, const Taxon& t);

    private:
         const int taxid;
         const int parentid;
         const string name;
         const string rank;
};

std::ostream& operator<<(std::ostream& output, const Taxon& t);

#endif
