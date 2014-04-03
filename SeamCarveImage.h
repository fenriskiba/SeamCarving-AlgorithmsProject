#ifndef SEAMCARVE_H
#define	SEAMCARVE_H

#include <iostream>
#include <cstdlib>
#include <fstream>

using namespace std;

class SeamCarveImage
{
    private:
        int** image;
        int** pixelWeights;
        int width;
        int height;
        
    public:
        Maze(string);
        
        void horizontalCarve(int);
        void verticalCarve(int);
        
    private:
        void horizontalSingleCarve();
        void verticalSingleCarve();
        int calculatePixelWeight(int, int);
        int calculateRowWeight(int);
        int calculateColumnWeight(int);
};

#endif
