#pragma once

#include "ofMain.h"
#include "ofxUST.h"

#include "ofxTiming.h"

#include "oneLine.h"

class ofApp : public ofBaseApp
{
public:
  void setup();
  void update();
  void draw();
  void mouseScrolled( int _x, int _y, float _scrollX, float _scrollY );
  void keyPressed( int _key );
  
    void getBackground();
  ofxUST ust;
  bool   bMirror;
  float  scale;
    
    vector<oneLine> allLines;
    vector<ofPolyline> pLines;
};
