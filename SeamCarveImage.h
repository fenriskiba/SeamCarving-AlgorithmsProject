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
        int** pixelEnergyMatrix;
        int width;
        int height;
        string maxGreyscale;
        
    public:
        SeamCarveImage(string);
        void seamCarve(int,int);
        void outputCarvedImage();
        
    private:
        void collectImageFromFile(string);
        void generatePixelEnergyMatrix();
        int calculatePixelEnergy(int, int);
        void verticalCarve();
        void horizontalCarve();
        void identifyVerticalSeam();
        void identifyHorizontalSeam();
        void deleteVerticalSeam();
        void deleteHorizontalSeam();
};

#endif
