
#include "scorefunc.hpp"
#include "database.hpp"
#include "util.hpp"

#include <iostream>
#include <getopt.h>

using namespace std;


int main(int argc, char *argv[]) {
    string dbfile;

    int c;
    while ((c = getopt(argc, argv, "d:")) != -1) {
        switch (c) {
            case 'd': 
                dbfile.assign(optarg);
                break;
        }
    }

    if (dbfile.empty()) {
        cerr << "Usage: getlineage -d <dbfile> taxid" << endl;
        return -1;
    }

    int taxid = fast_atoi(argv[argc-1]);

    Database* db = new Database(dbfile);
    vector<Taxon*> lin = db->get_lineage(taxid);

    if (lin.empty()) {
        cerr << "No such tax id: " << taxid << endl;
        return 1;
    }

    for (auto &t : lin) {
        cerr << "; " << *t;
        delete t;
    }
    cerr << endl;

    delete db;

    return 0;
}

