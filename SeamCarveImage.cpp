#include "SeamCarveImage.h"

using namespace std;

SeamCarveImage::SeamCarveImage(string fileName)
{
    collectImageFromFile(fileName);
    generatePixelEnergyMatrix();
}

void SeamCarveImage::collectImageFromFile(string fileName)
{
    string temp;
    ifstream infile;
    int stepCount = 1;
    vector<string> contentArray;
    
    //Open the file
    infile.open(fileName.c_str());
    if(!infile)
    {
        cout << "cannot be opened" << endl;
    }
    
    //Collect the file into a string
    while(getline(infile,temp))
    {
        if(temp[0] == '#')
        {
            //Comment line
        }
        else if(stepCount == 1)
        {
            //P2 literal
            stepCount++;
        }
        else if(stepCount == 2)
        {
            //Size of the image
            int space = temp.find(" ");
            string xString = temp.substr(0, space);
            string yString = temp.substr(space + 1);
            
            width = atoi(xString.c_str());
            height = atoi(yString.c_str());
            
            image = new int*[width];
            for(int i = 0; i < width; i++)
            {
                image[i] = new int[height];
            }
            
            stepCount++;
        }
        else if(stepCount == 3)
        {
            //Maximum greyscale value
            maxGreyscale = temp;
            stepCount++;
        }
        else
        {
            //Content of the image
            string numberString = "";
            int stringLen = temp.length();
            for(int i = 0; i < stringLen; i++)
            {
                if(temp[i] == ' ' || temp[i] == '\t')
                {
                    contentArray.push_back(numberString);
                    numberString = "";
                }
                else
                {
                    numberString += temp[i];
                }
            }
            //contentArray.push_back(numberString);
        }
    }
    
    infile.close();
    
    //Fill image with content from contentArray
    int vectorIterator = 0;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            string currentIteration = contentArray.at(vectorIterator);
            image[x][y] = atoi(currentIteration.c_str());
            vectorIterator++;
        }
    }
}

void SeamCarveImage::outputCarvedImage()
{
    ofstream outputFile("image_processed.pgm");
    
    outputFile << "P2" << endl
                << width << " " << height << endl
                << maxGreyscale << endl;
    
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            outputFile << image[x][y] << " ";
        }
        outputFile << endl;
    }
    
    outputFile.close();
}

void SeamCarveImage::generatePixelEnergyMatrix()
{
    //Delete the old pixel energy matrix if there is one
    if(pixelEnergyMatrix != NULL)
    {
        for(int i = 0; i < width; i++)
        {
            delete [] pixelEnergyMatrix[i];
        }
        delete [] pixelEnergyMatrix;
    }
    
    pixelEnergyMatrix = new int*[width];
    for(int i = 0; i < width; i++)
    {
        pixelEnergyMatrix[i] = new int[height];
    }
    
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            pixelEnergyMatrix[x][y] = calculatePixelEnergy(x,y);
        }
    }
}

int SeamCarveImage::calculatePixelEnergy(int x, int y)
{
    int current = image[x][y];
    int above;
    int below;
    int right;
    int left;
    
    if(x == 0)
        left = image[x][y];
    else
        left = image[x-1][y];
    
    if(y == 0)
        above = image[x][y];
    else
        above = image[x][y-1];
        
    if(x == width - 1)
        right = image[x][y];
    else
        right = image[x+1][y];
        
    if(y == height - 1)
        below = image[x][y];
    else
        below = image[x][y+1];
    
    int result = abs(current - above) +
                abs(current - below) +
                abs(current - right) +
                abs(current - left);
    
    return result;
}

void SeamCarveImage::seamCarve(int verticalCarves, int horizontalCarves)
{
    for(int i = 0; i < verticalCarves; i++)
    {
        verticalCarve();
        generatePixelEnergyMatrix();
    }
    
    for(int i = 0; i < horizontalCarves; i++)
    {
        horizontalCarve();
        generatePixelEnergyMatrix();
    }
}

void SeamCarveImage::verticalCarve()
{
    identifyVerticalSeam();
    deleteVerticalSeam();
}

void SeamCarveImage::horizontalCarve()
{
    identifyHorizontalSeam();
    deleteHorizontalSeam();
}

void SeamCarveImage::identifyVerticalSeam()
{

}

void SeamCarveImage::identifyHorizontalSeam()
{

}

void SeamCarveImage::deleteVerticalSeam()
{
    int oldWidth = width;
    width--;
    vector<int> contentArray;
    
    //Collect non-highlighted image
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < oldWidth; x++)
        {
            if(image[x][y] != -1)
                contentArray.push_back(image[x][y]);
        }
    }
    
    //Create new Image
    for(int i = 0; i < width; i++)
    {
        image[i] = new int[height];
    }
    
    int vectorIterator = 0;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            image[x][y] = contentArray.at(vectorIterator);
            vectorIterator++;
        }
    }
}

void SeamCarveImage::deleteHorizontalSeam()
{
    int oldHeight = height;
    height--;
    vector<int> contentArray;
    
    //Collect non-highlighted image
    for(int y = 0; y < oldHeight; y++)
    {
        for(int x = 0; x < width; x++)
        {
            if(image[x][y] != -1)
                contentArray.push_back(image[x][y]);
        }
    }
    
    //Create new Image
    for(int i = 0; i < width; i++)
    {
        image[i] = new int[height];
    }
    
    int vectorIterator = 0;
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            image[x][y] = contentArray.at(vectorIterator);
            vectorIterator++;
        }
    }
}

