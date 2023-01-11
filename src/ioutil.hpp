#ifndef ioutil_hpp
#define ioutil_hpp

#include <string>
#include <iostream>
#include <functional>

#include <stdio.h>
#include <unistd.h>

//void read_lines(const std::string &fname, void (*cb)(const std::string &s))
void read_lines(const std::string &fname, const std::function<void (const std::string &l)>& callback)
{
    FILE* ifile = fopen(fname.c_str(), "r");
    size_t linesz = 0;
    char* line = NULL;
    std::string s;
    while (getline(&line, &linesz, ifile) > 0) {
        s.assign(line);
        callback(s);
    }
    free(line);
    fclose(ifile);
}

#endif
