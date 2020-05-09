#ifndef assets_h
#define assets_h
#include "components.h"
#include <complex>
#include <vector>
using namespace std;

class Assets{
    public:
        // list of all game assets
        vector<complex<double>> plrAsset;
        vector<complex<double>> gunAsset;
        vector<complex<double>> box;

        //where all the assets are acutally defined
        void init();
};

#endif