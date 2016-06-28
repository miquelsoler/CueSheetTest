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

typedef enum
{
    MSMediaDataStatusProcessing = 0,
    MSMediaDataStatusDone = 1
} MSMediaDataStatus;

class MSMediaDataExtractor : public ofThread
{
public:

    void setup(string cueFilename, string cuePath, string isanPath, string epgPath);
    void threadedFunction();
    string getStatusString();
    float getProcessPercent();

private:

    MSMediaDataStatus status;

    void parseCueLine(string line);
    string findIsanCodeForTitle(string cueTitle);

    string cuePath;
    string cueFilename;
    string isanPath;
    string epgPath;

    vector<string> isanFilenames;

    ofBuffer cueBuffer;
    unsigned int numCueLines;
    unsigned int currentCueLine = 0;

    string statusString;
    float processPercent;
};

#endif /* MSMediaDataExtractor_hpp */
