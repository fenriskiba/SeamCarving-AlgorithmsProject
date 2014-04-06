#ifndef SEAMCARVE_H
#define	SEAMCARVE_H

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class SeamCarveImage
{
    private:
        int** image;
        int** pixelWeights;
        int width;
        int height;
        
    public:
        SeamCarveImage(string);
        void seamCarve(int,int);
        void outputCarvedImage();
        
    private:
        void collectImageFromFile(string);
        void generatePixelMatrix();
        int calculatePixelWeight(int, int);
        int calculateRowWeight(int);
        int calculateColumnWeight(int);
        void horizontalCarve();
        void verticalCarve();
};

#endif
