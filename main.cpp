#include "SeamCarveImage.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if(argc == 4)
    {
        string fileName = argv[1];
        int verticalCarves = atoi(argv[2]);
        int horizontalCarves = atoi(argv[3]);
        
        SeamCarveImage image(fileName);
        
        image.verticalCarve(verticalCarves);
        image.horizontalCarve(horizontalCarves);
    }
    else
    {
        cout << "Invalid Input" << endl;
    }
    
    return 0;
}
