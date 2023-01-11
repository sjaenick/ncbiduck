
#ifndef database_hpp
#define database_hpp

#include <duckdb.hpp>

#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "taxon.hpp"

using namespace std;
using namespace duckdb;

class Database {
    public:
        //Database(const string &dbfile, const string &scoremethod); // : db(DuckDB(dbfile)), conn(Connection(db)), scoremethod(scoremethod) {};
        Database(const string &dbfile); // : db(DuckDB(dbfile)), conn(Connection(db)), scoremethod(scoremethod) {};
        void importTaxonomy(const string &taxdir);
        vector<Taxon*> get_lineage(const int &taxid);
        Taxon by_id(const int &taxid);
        ~Database();

    private:
         DuckDB db;
         Connection conn;
         //string scoremethod;
         void filter_nodes(unordered_map<int, tuple<int, string>> &id2parent, unordered_map<int, int> &filtered);
         void execute_query(const string &q);
         void load_names(unordered_map<int, string> &ret, const string &namesdump);
         void load_nodes(vector<tuple<int, int, string>> &ret, const string &nodesdump);
         void load_delnodes(unordered_set<int> &ret, const string &nodesdump);
         void load_merged(unordered_map<int, int> &ret, const string &merged);

};

#endif
