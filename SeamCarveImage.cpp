#include "SeamCarveImage.h"

using namespace std;

SeamCarveImage::SeamCarveImage(string fileName)
{
    collectImageFromFile(fileName);
    //generatePixelMatrix();
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
            contentArray.push_back(numberString);
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


