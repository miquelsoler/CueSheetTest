//
//  MSMediaDataExtractor.hpp
//  BMATMediaDataExtractor
//
//  Created by Miquel Àngel Soler on 23/6/16.
//
//

#ifndef MSMediaDataExtractor_hpp
#define MSMediaDataExtractor_hpp

#include <stdio.h>
#include "ofThread.h"
#include "ofFileUtils.h"

class MSMediaDataExtractor : public ofThread
{

typedef enum
{
    MSMediaDataStatusCounting = 0,
    MSMediaDataStatusProcessing = 1,
    MSMediaDataStatusDone = 2
} MSMediaDataStatus;


public:

    void setup(string cueFilename, string cuePath, string isanPath, string epgPath);
    void threadedFunction();
    string getStatusString();
    float getProcessPercent();

private:

    MSMediaDataStatus status;

    void parseCueLine(string line);

    string cuePath;
    string cueFilename;
    string isanPath;
    string epgPath;

    ofBuffer cueBuffer;
    unsigned int numCueLines;
    unsigned int currentCueLine = 0;
    vector<string> isanFilenames;

    string statusString;
    float processPercent;
};

#endif /* MSMediaDataExtractor_hpp */
