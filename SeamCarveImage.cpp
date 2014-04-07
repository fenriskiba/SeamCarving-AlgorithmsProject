#include "SeamCarveImage.h"

using namespace std;

SeamCarveImage::SeamCarveImage(string fileName)
{
    collectImageFromFile(fileName);
    pixelEnergyMatrix = NULL;
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
                if(temp[i] == ' ' || temp[i] == '\t' || temp[i] == '\n')
                {
                    contentArray.push_back(numberString);
                    numberString = "";
                }
                else
                {
                    numberString += temp[i];
                }
            }
            
            /*if(temp != "")
            {
                contentArray.push_back(numberString);
            }*/
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
    //Delete old
    if(pixelEnergyMatrix != NULL)
    {
        for(int i = 0; i < width - 1; i++)
        {
            delete [] pixelEnergyMatrix[i];
        }
        delete [] pixelEnergyMatrix;
    }
    
    //Create new
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
    rotateImage();
    verticalCarve();
    rotateImage();
}

void SeamCarveImage::identifyVerticalSeam()
{
    //Generate cumulative energy matrix
    int** cumulativeEnergy = new int*[width];
    for(int i = 0; i < width; i++)
    {
        cumulativeEnergy[i] = new int[height];
    }
    
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {        
            if(y == 0)
            {
                cumulativeEnergy[x][y] = pixelEnergyMatrix[x][y];
            }
            else
            {
                int minLastNeighbor;
                if(x == 0)
                {
                    minLastNeighbor = min(cumulativeEnergy[x+1][y-1], cumulativeEnergy[x][y-1]);
                }
                else if(x == (width - 1))
                {
                    minLastNeighbor = min(cumulativeEnergy[x-1][y-1], cumulativeEnergy[x][y-1]);
                }
                else
                {
                    minLastNeighbor = min(min(cumulativeEnergy[x-1][y-1], cumulativeEnergy[x+1][y-1]), cumulativeEnergy[x][y-1]);
                }
                cumulativeEnergy[x][y] = pixelEnergyMatrix[x][y] + minLastNeighbor;
            }
        }
    }
    
    //Identify Seam
    int smallestIndex = 0;
    for(int i = 0; i < width; i++)
    {
        if(cumulativeEnergy[i][height-1] < cumulativeEnergy[smallestIndex][height-1])
        {
            smallestIndex = i;
        }
    }
    image[smallestIndex][height-1] = -1;
    
    for(int i = 1; i < height; i++)
    {
        int currentHeight = height - 1 - i;
        int rightIndex = smallestIndex + 1;
        int centerIndex = smallestIndex;
        int leftIndex = smallestIndex - 1;
        
        if(rightIndex < 0 || rightIndex >= width)
            rightIndex = centerIndex;
        if(leftIndex < 0 || leftIndex >= width)
            leftIndex = centerIndex;
        
        int smallestValue = min(min(cumulativeEnergy[rightIndex][currentHeight], cumulativeEnergy[leftIndex][currentHeight]), 
                                cumulativeEnergy[centerIndex][currentHeight]);
        
        
        if(cumulativeEnergy[leftIndex][currentHeight] == smallestValue)
            smallestIndex = leftIndex;
        else if(cumulativeEnergy[centerIndex][currentHeight] == smallestValue)
            smallestIndex = centerIndex;
        else if(cumulativeEnergy[rightIndex][currentHeight] == smallestValue)
            smallestIndex = rightIndex;
        
        image[smallestIndex][currentHeight] = -1;
    }
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
    
    //Delete old Image
    for(int i = 0; i < width; i++)
    {
        delete [] image[i];
    }
    delete [] image;
    
    //Create new Image
    image = new int*[width];
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

void SeamCarveImage::rotateImage()
{
    for(int i = 0; i < width - 1; i++)
    {
        delete [] pixelEnergyMatrix[i];
    }
    delete [] pixelEnergyMatrix;
    pixelEnergyMatrix = NULL;
    
    int preRotateWidth = width;
    int preRotateHeight = height;
    
    height = preRotateWidth;
    width = preRotateHeight;
    
    int** oldImage = image;
    image = new int*[width];
    for(int i = 0; i < width; i++)
    {
        image[i] = new int[height];
    }
    
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            image[x][y] = oldImage[y][x];
        }
    }
    
    generatePixelEnergyMatrix();
    
    for(int i = 0; i < height; i++)
    {
        delete [] oldImage[i];
    }
    delete [] oldImage;
}


