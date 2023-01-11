
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "scorefunc.hpp"
#include "refseeker.hpp"
#include "util.hpp"

using namespace std;

string RSResult::GetAssemblyID() { return assembly; }
float RSResult::GetScore() { return score; }
int RSResult::GetTaxID() { return taxid; }
void RSResult::Print() {
    cout << "Assembly: " << GetAssemblyID() << endl
         << "Score: " << GetScore() << endl
         << "tax. ID: " << GetTaxID() << endl;
}

vector<RSResult> RefSeeker::parse(ScoreFunc &scoref, const string &fname) {

    if (!file_exists(fname)) {
        cerr << "No such file: " << fname << endl;
    }

    if (file_is_empty(fname)) {
        cerr << "Empty file: " << fname << endl;
    }

    vector<RSResult> ret;

    ifstream infile(fname);
    string line;
    const string delim = "\t";

    std::getline(infile, line); // skip header

    while (std::getline(infile, line)) {
        const size_t pos = line.find(delim);
        const string asmid = line.substr(0, pos);

        const size_t pos2 = line.find(delim, pos+1);
        //ignore mash dist

        const size_t pos3 = line.find(delim, pos2+1);
        const float qr_ani= stof(line.substr(pos2+1, pos3-pos2-1));

        const size_t pos4 = line.find(delim, pos3+1);
        const float qr_condna = stof(line.substr(pos3+1, pos4-pos3-1));

        const size_t pos5 = line.find(delim, pos4+1);
        const float rq_ani = stof(line.substr(pos4+1, pos5-pos4-1));

        const size_t pos6 = line.find(delim, pos5+1);
        const float rq_condna = stof(line.substr(pos5+1, pos6-pos5-1));

        const size_t pos7 = line.find(delim, pos6+1);
        const int taxid = stoi(line.substr(pos6+1, pos7-pos6-1));

        const float score = scoref.getCombinedScore(qr_ani, qr_condna, rq_ani, rq_condna);
        const RSResult r(asmid, score, taxid);
        ret.push_back(r);

    }

    return ret;
}

