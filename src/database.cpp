
#include <duckdb.hpp>
#include <duckdb/main/appender.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "database.hpp"
#include "taxon.hpp"
#include "util.hpp"

using namespace std;
using namespace duckdb;

//Database::Database(const string& dbfile, const string &scoremethod) : db(DuckDB(dbfile)), conn(Connection(db)), scoremethod(scoremethod) {
//}

Database::Database(const string& dbfile) : db(DuckDB(dbfile)), conn(Connection(db))  {
}

void Database::importTaxonomy(const string &taxdir) {

     unordered_map<int, string> id2name;
     vector<tuple<int, int, string>> id2parent; // id, parentid, rank
     unordered_map<int, int> merged;
     unordered_set<int> delnodes;
     //id2name.reserve(2500000);
     id2parent.reserve(2500000);

     #pragma omp parallel sections
     {
         #pragma omp section
         { load_names(id2name, taxdir + "/names.dmp"); }
         #pragma omp section
         { load_delnodes(delnodes, taxdir + "/delnodes.dmp"); }
         #pragma omp section
         { load_merged(merged, taxdir + "/merged.dmp"); }
         #pragma omp section
         { load_nodes(id2parent, taxdir + "/nodes.dmp"); 
           //filter_nodes(id2parent, filtered);
         }
     }

     cerr << "read " << id2name.size() << " NCBI taxonomy nodes." << endl;
     //cerr << "obtained " << filtered.size() << " major NCBI taxonomy nodes." << endl;

     execute_query("CREATE OR REPLACE TABLE taxdata(taxid INTEGER NOT NULL, parent_id INTEGER, name STRING NOT NULL, rank STRING NOT NULL)");

     Appender appender(conn, "taxdata");
     for (const auto &t : id2parent) {

         int taxid = std::get<0>(t);
         int parentid = std::get<1>(t);

         while (merged.contains(taxid)) { taxid = merged[taxid]; }
         while (merged.contains(parentid)) { parentid = merged[parentid]; }

         if (!delnodes.contains(taxid) && !delnodes.contains(parentid)) {
             appender.BeginRow();
             appender.Append<int32_t>(taxid);
             appender.Append<int32_t>(parentid);
             appender.Append<string_t>(id2name[taxid]);
             appender.Append<string_t>(std::get<2>(t));
             appender.EndRow();
         }
     }
     appender.Close();

     // make sure root node does not have a parent
     execute_query("UPDATE taxdata SET parent_id=NULL WHERE taxid=1");

     execute_query("CREATE INDEX id_idx ON taxdata(taxid, parent_id)");
     //execute_query("ALTER TABLE taxdata ALTER COLUMN taxid TYPE INTEGER unique");
}

vector<Taxon*> Database::get_lineage(const int &taxid) {
    const string sql = "WITH RECURSIVE hierarchy AS ( "
        "SELECT taxid, parent_id FROM taxdata WHERE taxid=$1 "
        "UNION ALL "
        "SELECT t.taxid, t.parent_id "
        "FROM taxdata t, hierarchy h "
        "WHERE t.taxid=h.parent_id "
        ") "
        "SELECT h.taxid, h.parent_id, t.name, t.rank FROM hierarchy h "
        "LEFT JOIN taxdata t ON h.taxid=t.taxid";

    auto stmt = conn.Prepare(sql);
    auto result = stmt->Execute(taxid);

    if (result->HasError()) {
         [[unlikely]];
         throw result->GetError();
    }

    std::vector<Taxon*> ret;

    for (auto &row : *result) {
        bool parent_isnull = row.GetValue<Value>(1).IsNull();
        Taxon* t = new Taxon(row.GetValue<int32_t>(0), parent_isnull ? -1 : row.GetValue<int32_t>(1), row.GetValue<string>(2), row.GetValue<string>(3));
        ret.push_back(t);
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}

Taxon Database::by_id(const int &taxid) {
    const string sql = "SELECT taxid, parent_id, name, rank FROM taxdata WHERE taxid=$1";

    auto stmt = conn.Prepare(sql);
    auto result = stmt->Execute(taxid);

    if (result->HasError()) {
         [[unlikely]];
         throw result->GetError();
    }

    for (auto &row : *result) {
        bool parent_isnull = row.GetValue<Value>(1).IsNull();
        Taxon t(row.GetValue<int32_t>(0), parent_isnull ? -1 : row.GetValue<int32_t>(1), row.GetValue<string>(2), row.GetValue<string>(3));
        return t;
    }
    throw "No such taxon";
}

void Database::execute_query(const string &q) {
    auto result = conn.Query(q);
    if (result->HasError()) {
         [[unlikely]];
        throw result->GetError();
    }
}

void Database::load_names(unordered_map<int, string> &ret, const string &namesdump) {
    if (!file_exists(namesdump)) {
        cerr << "BUH!!!! " << namesdump << endl;
        return;
    }

    ifstream infile(namesdump);
    string line;
    const string delim = "\t|\t";
    while (std::getline(infile, line)) {
        // avoid processing a line that doesn't contain the term 'scientific';
        // also, we can skip over three separators (9bytes) and at least 3
        // content fields (3bytes min) for the check
        if (line.find("scientific", 12) != string::npos) {
            const size_t pos = line.find(delim);
            const size_t pos2 = line.find(delim, pos+3);

            const string taxid = line.substr(0, pos);
            const string name = line.substr(pos+3, pos2-pos-3);
            ret[fast_stoi(taxid)] = name;
        }
    }
}

void Database::load_nodes(vector<tuple<int, int, string>> &ret, const string &nodesdump) {
    if (!file_exists(nodesdump)) {
        cerr << "BUH!!!! " << nodesdump << endl;
        return;
    }

    ifstream infile(nodesdump);
    string line;
    const string delim = "\t|\t";
    while (std::getline(infile, line)) {
        const size_t pos = line.find(delim);
        const int taxid = fast_stoi(line.substr(0, pos));

        const size_t pos2 = line.find(delim, pos+3);
        const int parent_id = fast_stoi(line.substr(pos+3, pos2-pos-3));

        const size_t pos3 = line.find(delim, pos2+3);
        const string rank = line.substr(pos2+3, pos3-pos2-3);
        ret.push_back(make_tuple(taxid, parent_id, rank));
    }
}

void Database::load_merged(unordered_map<int, int> &ret, const string &merged) {
    if (!file_exists(merged)) {
        cerr << "BUH!!!! " << merged << endl;
        return;
    }

    ifstream infile(merged);
    string line;
    const string delim = "\t|\t";
    while (std::getline(infile, line)) {
        const size_t pos = line.find(delim);
        const int taxid = fast_stoi(line.substr(0, pos));

        const size_t pos2 = line.find(delim, pos+3);
        const int replacement_id = fast_stoi(line.substr(pos+3, pos2-pos-3));

        ret[taxid] = replacement_id;
    }
}

void Database::filter_nodes(unordered_map<int, tuple<int, string>> &id2parent, unordered_map<int, int> &filtered) {

    const unordered_set<string> major = {
         "superkingdom", "phylum", "class", "order", "family",
         "genus", "species"
    };

    // always insert root node
    filtered[1] = std::get<0>(id2parent[1]);

    for (const auto& [key, value] : id2parent) {
        const int taxid = key; 
        int parent_id = std::get<0>(value);
        const string rank = std::get<1>(value);

        // check if it is a major node
        if (major.find(rank) != major.end()) {
            parent_id = std::get<0>(value);
            tuple<int, string> p = id2parent[parent_id];
            string parent_rank = std::get<1>(p);
            while (parent_id != 1 && major.find(parent_rank) == major.end()) {
                p = id2parent[parent_id];
                parent_id = std::get<0>(p);
                parent_rank = std::get<1>(p);
            }
            //cerr << "found " << taxid << " (" << rank << ") -> " << parent_id << " (" << parent_rank << ")" << endl;
            filtered[taxid] = parent_id;
        }
    }
}

void Database::load_delnodes(unordered_set<int> &ret, const string &delnodes) {
    if (!file_exists(delnodes)) {
        cerr << "BUH!!!! " << delnodes << endl;
        return;
    }

    ifstream infile(delnodes);
    string line;
    const string delim = "\t|\t";
    while (std::getline(infile, line)) {
        const size_t pos = line.find(delim);
        const int taxid = fast_stoi(line.substr(0, pos));
        ret.insert(taxid);
    }
}


Database::~Database() {
    execute_query("CHECKPOINT"); // flush WAL
}
