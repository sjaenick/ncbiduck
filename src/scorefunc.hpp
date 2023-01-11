
#ifndef scorefunc_hpp
#define scorefunc_hpp

#include <string>
#include <cmath>

using namespace std;

enum METHOD { MEAN = 1, GEOMMEAN= 2 };

class ScoreFunc {
    public:
        virtual METHOD getMethod() = 0;
        virtual const string getName() = 0;
        virtual float getMaxScore() = 0;
        virtual float getCombinedScore(float i1, float i2, float i3, float i4) = 0;
        static ScoreFunc* forName(const METHOD &method);
};

class MeanScore : public ScoreFunc {
    public:
        constexpr METHOD getMethod() { return MEAN; }
        inline const string getName() { return "mean"; }
        constexpr float getMaxScore() { return 100.0; }
        float getCombinedScore(float i1, float i2, float i3, float i4) {
            return (i1 + i2 + i3 + i4)/4;
        }
};

class GeomMeanScore : public ScoreFunc {
    public:
        //using ScoreFunc::ScoreFunc;
        constexpr METHOD getMethod() { return GEOMMEAN; }
        inline const string getName() { return "geometric_mean"; }
        constexpr float getMaxScore() { return pow(100.0*100.0*100.0*100.0, 0.25); }
        float getCombinedScore(float i1, float i2, float i3, float i4) {
            return pow(i1 * i2 * i3 * i4, 0.25);
        }
};

#endif
