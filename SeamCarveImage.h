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
        int** cumulativeEnergy;
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
        void deleteVerticalSeam();
        void rotateImage();
        void createEmptyImageMatrix();
        void populateCumulativeEnergyMatrix();
        int calculateCumulativeEnergyForSinglePixel(int, int);
        void deletePixelEnergyMatrix();
        void markVerticalSeam();
};

#endif
