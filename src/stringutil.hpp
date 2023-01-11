#ifndef stringutil_hpp
#define stringutil_hpp

#include <string>
#include <vector>

using namespace std;

static void join(const vector<string>& v, const string &delim, string& dest) {

   dest.clear();

   for (vector<string>::const_iterator p = v.begin(); p != v.end(); ++p) { 
      dest += *p;
      if (p != v.end() - 1)
        dest += delim;
   }
}

#endif
