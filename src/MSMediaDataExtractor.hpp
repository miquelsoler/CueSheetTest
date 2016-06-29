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

#include "MSCueItem.hpp"
#include "MSIsanItem.hpp"

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

    vector<MSCueItem> cueItems;
    vector<MSIsanItem> isanItems;

    void buildCueCache();
    void buildIsanCacheForFile(string isanFilename);
    void findCueIdsForIsanFile();

    string cuePath;
    string cueFilename;
    string isanPath;
    string epgPath;

    vector<string> isanFilenames;

    string statusString;
    float processPercent;
};

#endif /* MSMediaDataExtractor_hpp */
