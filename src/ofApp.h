#pragma once

#include "ofMain.h"
#include "MSMediaDataExtractor.hpp"

class ofApp : public ofBaseApp
{
public:

    void setup();
    void update();
    void draw();
    void exit();

private:

    MSMediaDataExtractor mediaDataExtractor;
};
