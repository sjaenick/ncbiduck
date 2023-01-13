
#include "scorefunc.hpp"
#include "database.hpp"
#include "util.hpp"

#include <iostream>
#include <unordered_set>
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
        cerr << "Usage: test_all -d <dbfile>" << endl;
        return -1;
    }

    Database* db = new Database(dbfile);

    bool error = false;
    vector<Taxon> all = db->get_all();
    unordered_set<int> seen;
    //unordered_set<Taxon> sen;
    //int count = 0;

    #pragma omp parallel for
    for (auto t : all) {

        //count++;
        //cerr << count << "/" << all.size() << endl;

        if (!seen.contains(t.getID())) {

            vector<Taxon> lin = db->get_lineage(t.getID());

            if (lin.empty()) {
                cerr << "No such tax id: " << t.getID() << endl;
                #pragma omp critical
                error = true;
            }

            Taxon root = lin.front();
            if (root.getID() != 1) {
                cerr << "lineage for " << t.getName() << " with ID " << t.getID() << " not rooted!" << endl;
                #pragma omp critical
                error = true;
            }

            #pragma omp critical
            for (auto x : lin) { seen.insert(x.getID()); }

        }
    }

    delete db;

    if (error) { return 1; }
    return 0;
}

