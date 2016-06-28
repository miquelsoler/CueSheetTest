//
//  MSMediaDataExtractor.cpp
//  BMATMediaDataExtractor
//
//  Created by Miquel Àngel Soler on 23/6/16.
//
//

#include "MSMediaDataExtractor.hpp"
#include "ofUtils.h"

typedef enum
{
    CueId = 0,
    CueTitle = 1,
    CuePips = 2,
    CueTerritory = 3,
    CueCatalogue = 4,
    CuePDFPath = 5,
    CuePDFId = 6,
    CueCreatedAt = 7,
    CUE_NUM_COLUMNS = 8
} MSCueColumns;

// id|title|pips|territory|catalogue|pdf_path|pdf_id|created_at


void MSMediaDataExtractor::setup(string _cueFilename, string _cuePath, string _isanPath, string _epgPath)
{
    status = MSMediaDataStatusCounting;

    cueFilename = _cueFilename;

    cuePath = _cuePath;
    isanPath = _isanPath;
    epgPath = _epgPath;

    cueBuffer = ofBufferFromFile(cuePath + "/" + cueFilename);
}

void MSMediaDataExtractor::threadedFunction()
{
    ofBuffer::Lines lines = cueBuffer.getLines();
    ofBuffer::Line itLines = lines.begin();

    // Count total number of lines in cue file
    {
        numCueLines = 0;
        for (; itLines != lines.end(); ++itLines) numCueLines++;
        sleep(100); // TODO: Remove
    }

    // Process cue file line by line
    {
        status = MSMediaDataStatusProcessing;
        for (itLines = lines.begin(); itLines != lines.end(); ++itLines)
        {
            string strLine = itLines.asString();
            if ((strLine.size() == 0) || (currentCueLine == 0))
            { // Skip header and empty lines
                cout << "Line " << currentCueLine << " empty" << endl;
            }
            else
            {
                parseCueLine(strLine);
            }
            currentCueLine++;
        }

        status = MSMediaDataStatusDone;
    }
}

string MSMediaDataExtractor::getStatusString()
{
    string result;
    switch(status) {
        case MSMediaDataStatusCounting: result = "Counting number of cue sheet tracks."; break;
        case MSMediaDataStatusProcessing: result = "Processing " + to_string(currentCueLine) + " of " + to_string(numCueLines) + " cue sheet tracks"; break;
        case MSMediaDataStatusDone: result = "Done!"; break;
    }

    return result;
}

float MSMediaDataExtractor::getProcessPercent()
{
    float result;
    switch (status) {
        case MSMediaDataStatusCounting: result = 0.0; break;
        case MSMediaDataStatusProcessing: result = (static_cast<float>(currentCueLine) / static_cast<float>(numCueLines)) * 100;
        case MSMediaDataStatusDone: result = 100.0; break;
    }

    return result;
}

void MSMediaDataExtractor::parseCueLine(string line)
{
    vector<string> lineColumns = ofSplitString(line, "|"); // Split line

    if (lineColumns.size() != CUE_NUM_COLUMNS) return;
    if (lineColumns[CueId].empty() || lineColumns[CueTitle].empty()) return;

    string cueId = lineColumns[CueId];
    string cueTitle = lineColumns[CueTitle];
}
