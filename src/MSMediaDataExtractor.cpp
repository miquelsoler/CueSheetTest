//
//  MSMediaDataExtractor.cpp
//  BMATMediaDataExtractor
//
//  Created by Miquel Àngel Soler on 23/6/16.
//
//

#include "MSMediaDataExtractor.hpp"
#include "ofUtils.h"
#include "ofxXmlSettings.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

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

    cueBuffer = ofBufferFromFile(cuePath + "/" + cueFilename);

    isanFilenames = {
        "ISANDB_Export_SE_01.xml", "ISANDB_Export_SE_02.xml",
        "ISANDB_Export_SE_03.xml", "ISANDB_Export_SE_04.xml",
        "ISANDB_Export_SE_05.xml", "ISANDB_Export_SE_06.xml",
        "ISANDB_Export_SE_07.xml", "ISANDB_Export_SE_08.xml",
        "ISANDB_Export_SH_01.xml", "ISANDB_Export_SW_01.xml",
        "ISANDB_Export_SW_02.xml", "ISANDB_Export_SW_03.xml"
    };
}

void MSMediaDataExtractor::threadedFunction()
{
    ofBuffer::Lines lines = cueBuffer.getLines();
    ofBuffer::Line itLines = lines.begin();

    // Count total number of lines in cue file
    {
        numCueLines = 0;
        for (; itLines != lines.end(); ++itLines) numCueLines++;
    }

    // Process cue file line by line
    {
        status = MSMediaDataStatusProcessing;
        for (itLines = lines.begin(); itLines != lines.end(); ++itLines)
        {
            string strLine = itLines.asString();
            if ((strLine.size() != 0) && (currentCueLine != 0)) // Skip header and empty lines
                parseCueLine(strLine);

            currentCueLine++;
        }

        status = MSMediaDataStatusDone;
    }
}

string MSMediaDataExtractor::getStatusString()
{
    string result;
    switch(status) {
        case MSMediaDataStatusProcessing: result = "Processing " + to_string(currentCueLine) + " of " + to_string(numCueLines) + " cue sheet tracks..."; break;
        case MSMediaDataStatusDone: result = "Done!"; break;
    }

    return result;
}

float MSMediaDataExtractor::getProcessPercent()
{
    float result;
    switch (status) {
        case MSMediaDataStatusProcessing: result = (static_cast<float>(currentCueLine) / static_cast<float>(numCueLines)) * 100; break;
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
    string cueTitle = lineColumns[CueTitle].substr(1, lineColumns[CueTitle].size() - 2); // Remove ""

    string isanCode = findIsanCodeForTitle(cueTitle);
    if (isanCode.empty()) return;
}

string MSMediaDataExtractor::findIsanCodeForTitle(string cueTitle)
{
    string result = "";
    bool found = false;

    for (int i=0; i<isanFilenames.size() && !found; ++i)
    {
        cout << "Processing <" << isanFilenames[i] << ">" << endl;

/* ofXmlSettings */

//        ofxXmlSettings isanXml;
//        float elapsed = ofGetElapsedTimef();
//        isanXml.loadFile(isanPath + "/" + isanFilenames[i]);
//        elapsed = ofGetElapsedTimef() - elapsed;
//        cout << "Took " << elapsed << " seconds to load." << endl;
//
//
//        elapsed = ofGetElapsedTimef();
//        isanXml.pushTag(XML_WORKCATALOGUE);
//        isanXml.pushTag(XML_WORKLIST);
//        int numWorks = isanXml.getNumTags(XML_WORK);
//
//        cout << "Num works: " << numWorks << endl;
//        for (int j=0; j<numWorks; ++j)
//        {
//            isanXml.pushTag(XML_WORK, j);
//            string root = isanXml.getAttribute(XML_ISAN, "root", "");
//            string episode = isanXml.getAttribute(XML_ISAN, "episodeOrPart", "");
//            string check1 = isanXml.getAttribute(XML_ISAN, "check1", "");
//            string version = isanXml.getAttribute(XML_ISAN, "version", "");
//            string check2 = isanXml.getAttribute(XML_ISAN, "check2", "");
////            cout << j << ">>> " << root << "-" << episode << "-" << check1 << "-" << version << "-" << check2 << endl;
//            isanXml.popTag();
//        }
//
//        elapsed = ofGetElapsedTimef() - elapsed;
//        cout << "Took " << elapsed << " seconds to parse." << endl;

/* Boost */

        using boost::property_tree::ptree;
        string dataFolder = "../../../data";

        float elapsed = ofGetElapsedTimef();
        ptree cue;
        read_xml(dataFolder + "/" + isanPath + "/" + isanFilenames[i], cue);
        elapsed = ofGetElapsedTimef() - elapsed;
        cout << "Took " << elapsed << " seconds to load." << endl;

        boost::property_tree::ptree works = cue.get_child(XML_WORKCATALOGUE);



/* Manual */

//        ofBuffer isanBuffer = ofBufferFromFile(isanPath + "/" + isanFilenames[i]);
//        if (isanBuffer.size() == 0) continue;
//
//        cout << "Processing <" << isanFilenames[i] << ">" << endl;
//
//        ofBuffer::Lines lines = isanBuffer.getLines();
//        ofBuffer::Line itLines = lines.begin();
//
//        for (itLines = lines.begin(); itLines != lines.end(); ++itLines)
//        {
//            string strLine = itLines.asString();
//            if (ofIsStringInString(strLine, "<ISAN"))
//            {
//                string isanId = getIsanIdForLine(itLines.asString());
//                if (isanId.empty()) continue;
//
//                cout << "New ISAN: " << isanId << endl;
//            }
//        }
    }

    return result;
}
