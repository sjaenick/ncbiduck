
#ifndef refseeker_hpp
#define refseeker_hpp

#include <string>
#include <vector>
#include "scorefunc.hpp"

using namespace std;

class RSResult {
    public:
        RSResult(const string &asmid, float score, int taxid)
            : assembly(asmid), score(score), taxid(taxid) {};
        string GetAssemblyID();
        float GetScore();
        int GetTaxID();
        void Print();
    private:
        const string assembly;
        const float score;
        const int taxid;
};

class RefSeeker {
    public:
        static vector<RSResult> parse(ScoreFunc &score, const string &fname);
};


#endif
