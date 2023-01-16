#ifndef util_hpp
#define util_hpp

#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

inline bool file_exists(const std::string& name) {
    struct stat buffer;   
    return (stat(name.c_str(), &buffer) == 0 && (buffer.st_mode & S_IFREG) == S_IFREG); 
}

inline bool file_is_empty(const std::string& name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0 && buffer.st_size == 0);
}

inline bool directory_exists(const std::string& name) {
    struct stat buf;   
    int ret = stat(name.c_str(), &buf);
    if (ret != 0 || (buf.st_mode & S_IFDIR) != S_IFDIR) {
        return false;
    }
    return true;
}

//https://stackoverflow.com/questions/16826422/c-most-efficient-way-to-convert-string-to-int-faster-than-atoi
inline int fast_atoi(const char *p) {
    int x = 0;
    while (*p != '\0') {
        x = (x*10) + (*p - '0');
        ++p;
    }
    return x;
}

inline int fast_stoi(const string &s) {
    return fast_atoi(s.c_str());
}

inline int fast_stoi(const string &s, const size_t pos, const size_t len) {
    int ret = 0;
    const char *p = s.c_str();
    for (size_t i=pos; i<pos+len; i++) {
        int x = *(p+i) - '0';
        ret = ret*10 + x;
    }
    return ret;
}


#endif
