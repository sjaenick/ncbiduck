
//#include "scorefunc.hpp"
#include "database.hpp"
#include "util.hpp"

#include <iostream>
#include <getopt.h>

using namespace std;


int main(int argc, char *argv[]) {
    string taxdir;
    string dbfile;
//    METHOD scoremethod = MEAN;


    int c;
    while ((c = getopt(argc, argv, "t:d:g")) != -1) {
        switch (c) {
            case 't': 
                taxdir.assign(optarg);
                break;
            case 'd': 
                dbfile.assign(optarg);
                break;
//            case 'g':
//                scoremethod = GEOMMEAN;
//                break;
            default:
                cerr << "Usage: createdb -t <taxdir> -d <dbfile> [ -g ]" << endl;
                return -1;
        }
    }

    if (taxdir.empty() || dbfile.empty()) {
        cerr << "Usage: createdb -t <taxdir> -d <dbfile> [ -g ]" << endl;
        return -1;
    }

    if (!directory_exists(taxdir)) {
        cerr << taxdir << " is not a directory." << endl;
        return -1;
    }

    //ScoreFunc* score = ScoreFunc::forName(scoremethod);
    //cerr << "Using " << score->getName() << " scoring." << endl;

    //Database* db = new Database(dbfile, score->getName());
    Database* db = new Database(dbfile);
    db->import_taxonomy(taxdir);
    vector<Taxon*> lin = db->get_lineage(2675216);

    for (auto &t : lin) {
        cerr << "; " << *t;
        delete t;
    }
    cerr << endl;

    delete db;

    return 0;
}

