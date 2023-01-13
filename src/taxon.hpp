
#ifndef taxon_hpp
#define taxon_hpp

#include <string>

using namespace std;

class Taxon {
    public:
        Taxon(int taxid, int parentid, const string &name, const string &rank)
            : taxid(taxid), parentid(parentid), name(name), rank(rank) {};
        inline int getID() { return taxid; };
        inline int getParentID() { return parentid; }
        inline const string getName() { return name; }
        inline const string getRank() { return rank; }
        Taxon(const Taxon& t) : taxid(t.taxid), parentid(t.parentid), name(t.name), rank(t.rank) {}; // copy
        Taxon& operator=(const Taxon&); // copy
        Taxon(Taxon&& t) = default; // move
        Taxon& operator=(Taxon&& t) = default; // move
        friend std::ostream& operator<<(std::ostream& output, const Taxon& t);

    private:
        int taxid;
        int parentid;
        string name;
        string rank;
};

std::ostream& operator<<(std::ostream& output, const Taxon& t);

#endif
