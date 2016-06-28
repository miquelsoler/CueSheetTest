#include "ofApp.h"

static const int PERCENT_MARGIN = 10;
static const int PERCENT_HEIGHT = 20;

void ofApp::setup()
{
    ofBackground(ofColor::white);
    mediaDataExtractor.setup("umpgworks.table.csv", "cuesheet", "isan", "epg");
    mediaDataExtractor.startThread();
}

void ofApp::update()
{
}

void ofApp::draw()
{
    float processPercent = mediaDataExtractor.getProcessPercent();

    // Draw status
    ofPushStyle();
    {
        ofSetColor(ofColor::black);
        ofDrawBitmapString(mediaDataExtractor.getStatusString(), 10, 20);
    }
    ofPopStyle();

    // Draw percent
    ofPushStyle();
    {
        ofSetColor(ofColor::orangeRed);

        // Draw rectangle edges
        ofNoFill();
        int pctX = PERCENT_MARGIN;
        int pctY = ofGetHeight() - 1 - PERCENT_MARGIN - PERCENT_HEIGHT;

        // Draw percent rectangle
        ofDrawRectangle(pctX, pctY, ofGetWidth() - 2*PERCENT_MARGIN, PERCENT_HEIGHT);
        ofFill();
        float pctWidth = ofMap(processPercent, 0.0, 100.0, PERCENT_MARGIN, ofGetWidth() - (2*PERCENT_MARGIN));
        ofDrawRectangle(pctX, pctY, pctWidth, PERCENT_HEIGHT);

        ofSetColor(ofColor::black);
        string pctMessage = ofToString(int(processPercent)) + "%";
        ofDrawBitmapString(pctMessage, (ofGetWidth()/2) - 1 - (pctMessage.length() - 1) * 8/2, pctY + 14);
    }
    ofPopStyle();
}
