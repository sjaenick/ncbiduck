
#include <string>
#include <iostream>

#include <scorefunc.hpp>

using namespace std;

ScoreFunc* ScoreFunc::forName(const METHOD &method) {
    switch (method) {
        case MEAN:
            return new MeanScore();
            break;
        case GEOMMEAN:
            return new GeomMeanScore();
            break;
        default:
            cerr << "Invalid scoring method, using mean score." << endl;
            return new MeanScore();
            break;
    }
}
