//
//  MSMediaDataExtractor.cpp
//  BMATMediaDataExtractor
//
//  Created by Miquel Ã€ngel Soler on 23/6/16.
//
//

#include "MSMediaDataExtractor.hpp"
#include "ofUtils.h"
#include "ofxXmlSettings.h"

static const string XML_WORKCATALOGUE = "ISANWorkCatalogue";
static const string XML_WORKLIST = "WorkMetadataList";
static const string XML_WORK = "WorkMetadata";
static const string XML_ISAN = "ISAN";

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


void MSMediaDataExtractor::setup(string _cueFilename, string _cuePath, string _isanPath, string _epgPath)
{
    status = MSMediaDataStatusProcessing;

    cueFilename = _cueFilename;

    cuePath = _cuePath;
    isanPath = _isanPath;
    epgPath = _epgPath;

    isanFilenames = {
//        "test.xml",
        "ISANDB_Export_SW_03.xml",
//        "ISANDB_Export_SE_01.xml", "ISANDB_Export_SE_02.xml",
//        "ISANDB_Export_SE_03.xml", "ISANDB_Export_SE_04.xml",
//        "ISANDB_Export_SE_05.xml", "ISANDB_Export_SE_06.xml",
//        "ISANDB_Export_SE_07.xml", "ISANDB_Export_SE_08.xml",
//        "ISANDB_Export_SH_01.xml", "ISANDB_Export_SW_01.xml",
//        "ISANDB_Export_SW_02.xml",
    };
}

void MSMediaDataExtractor::threadedFunction()
{
    status = MSMediaDataStatusProcessing;

    // Put Cue data into memory
    buildCueCache();

    for (int i=0; i<isanFilenames.size(); ++i)
    {
        // Put 1 ISAN XML file into memory.
        buildIsanCacheForFile(isanFilenames[i]);

        // Try to match ISAN titles with the Cue file
        findCueIdsForIsanFile();

        // Aqu’ Žs on agafo isanItems, i he de fer match amb cue i EPG.



        // I quan acabi, fer net d'isanItems (isanItems.clear()) i passar al segŸent

        isanItems.clear();
    }

    status = MSMediaDataStatusDone;
    statusString = "Done!";
}

string MSMediaDataExtractor::getStatusString()
{
    string result = statusString;
    return result;
}

float MSMediaDataExtractor::getProcessPercent()
{
    float result = 0.0;
    switch (status) {
        case MSMediaDataStatusProcessing: result = processPercent; break;
        case MSMediaDataStatusDone: result = 100.0; break;
    }

    return result;
}

void MSMediaDataExtractor::buildCueCache()
{
    cout << "Importing <" << cueFilename << ">" << endl;

    ofBuffer cueBuffer = ofBufferFromFile(cuePath + "/" + cueFilename);

    ofBuffer::Lines lines = cueBuffer.getLines();
    ofBuffer::Line itLines = lines.begin();

    unsigned int numCueLines;
    unsigned int currentCueLine;

    // Count total number of lines in cue file

    numCueLines = 0;
    for (; itLines != lines.end(); ++itLines) numCueLines++;

    // Process cue file line by line

    status = MSMediaDataStatusProcessing;
    for (itLines = lines.begin(); itLines != lines.end(); ++itLines)
    {
        string strLine = itLines.asString();
        if ((strLine.size() != 0) && (currentCueLine != 0)) // Skip header and empty lines
        {
            vector<string> lineColumns = ofSplitString(strLine, "|"); // Split line

            if (lineColumns.size() != CUE_NUM_COLUMNS) return;
            if (lineColumns[CueId].empty() || lineColumns[CueTitle].empty()) return;

            MSCueItem cueItem;
            cueItem.ID = lineColumns[CueId];
            cueItem.title = lineColumns[CueTitle].substr(1, lineColumns[CueTitle].size() - 2); // Remove ""

            cueItems.push_back(cueItem);
        }

        currentCueLine++;
    }

    cout << "cueItems.size() = " << cueItems.size() << endl;
}

void MSMediaDataExtractor::buildIsanCacheForFile(string isanFilename)
{
    processPercent = 0;
    statusString = "Loading ISAN file <" + isanFilename + ">";

    cout << "Processing <" << isanFilename << ">" << endl;

    ofxXmlSettings isanXml;
    float elapsed = ofGetElapsedTimef();
    isanXml.loadFile(isanPath + "/" + isanFilename);
    elapsed = ofGetElapsedTimef() - elapsed;

    isanXml.pushTag(XML_WORKCATALOGUE);
    isanXml.pushTag(XML_WORKLIST);
    int numWorks = isanXml.getNumTags(XML_WORK);

    cout << "Num works: " << numWorks << endl;
    cout << "Took " << elapsed << " seconds to load." << endl;

    elapsed = ofGetElapsedTimef();

    for (int j=0; j<numWorks; ++j)
    {
        processPercent = ofMap(j, 0, numWorks-1, 0, 100);
        statusString = "Building ISAN cache <" + isanFilename + ">\n" + ofToString(numWorks) + " ISAN works";

        isanXml.pushTag(XML_WORK, j);
        {
            MSIsanItem isanItem;

            string root = isanXml.getAttribute(XML_ISAN, "root", "");
            string episode = isanXml.getAttribute(XML_ISAN, "episodeOrPart", "");
            string check1 = isanXml.getAttribute(XML_ISAN, "check1", "");
            string version = isanXml.getAttribute(XML_ISAN, "version", "");
            string check2 = isanXml.getAttribute(XML_ISAN, "check2", "");

            isanItem.ID = root + "-" + episode + "-" + check1 + "-" + version + "-" + check2;

            isanXml.pushTag("TitleList");
            {
                int numTitles = isanXml.getNumTags("title:TitleDetail");
                for (int k=0; k<numTitles; ++k)
                {
                    isanXml.pushTag("title:TitleDetail", k);
                    isanItem.titles.push_back(isanXml.getValue("title:Title", ""));
                    isanXml.popTag();
                }
            }
            isanXml.popTag();

            isanItems.push_back(isanItem);

        }
        isanXml.popTag();
    }

    isanXml.popTag();
    isanXml.popTag();

    isanXml.clear();

    elapsed = ofGetElapsedTimef() - elapsed;

    cout << "Took " << elapsed << " seconds to parse." << endl;
}

void MSMediaDataExtractor::findCueIdsForIsanFile()
{
    statusString = "Trying to match ISAN with Cue works";

    int numCueItems = cueItems.size();
    for (int i=0; i<numCueItems; ++i)
    {
        cout << "Trying to match <" << cueItems[i].title << ">" << endl;
        processPercent = ofMap(i, 0, numCueItems-1, 0, 100);

        bool found = false;
        for (int j=0; j<isanItems.size() && !found; ++j)
        {
            for (int k=0; k<isanItems[j].titles.size() && !found; ++k)
            {
                if (ofToLower(isanItems[j].titles[k]) == ofToLower(cueItems[i].title)) {
                    found = true;
                    cout << "Found " << isanItems[j].titles[k] << endl;
                }
            }
        }
    }
}
